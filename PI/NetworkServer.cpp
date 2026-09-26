/**
 * @file NetworkServer.cpp
 * @brief Implementation of the NetworkServer class for TCP client-server communication
 *        between the Raspberry Pi and a connected PC.
 * @author evan
 * @date 2026-03-21
 */
#include "NetworkServer.h"
/**
 * @brief Constructs a NetworkServer and resolves the device's local IP address.
 *
 * Retrieves the host's IP address by running `hostname -I` via popen() and
 * stores it for later retrieval via getIP(). The server is not started until
 * start() is explicitly called.
 *
 * @param port The TCP port number the server will listen on.
 */
NetworkServer::NetworkServer(int port) : port(port) {
    FILE* pipe = popen("hostname -I", "r");
    char buf[64];
    fgets(buf, sizeof(buf), pipe);
    pclose(pipe);
    ip = std::string(buf);
}
/**
 * @brief Destructs the NetworkServer and ensures the server is stopped cleanly.
 *
 * Calls stop() to shut down sockets and join any running threads before
 * the object is destroyed.
 */

NetworkServer::~NetworkServer() {
    stop();
}

/**
 * @brief Creates the server socket, binds it to the configured port, and begins
 *        accepting client connections.
 *
 * Performs the full TCP server startup sequence:
 * -# Creates a TCP socket with SO_REUSEADDR to allow rapid restarts.
 * -# Binds to INADDR_ANY on the configured port.
 * -# Begins listening with a backlog of 3.
 * -# Spawns the acceptLoop thread to handle incoming connections asynchronously.
 *
 * @return true if the server started successfully, false if socket creation
 *         or binding failed.
 */
bool NetworkServer::start() {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to bind to port " << port << std::endl;
        return false;
    }

    listen(serverFd, 3);
    running = true;
    acceptThread = std::thread(&NetworkServer::acceptLoop, this);

    std::cout << "Server IP: " << getIP() << std::endl;
    std::cout << "Listening on port " << port << std::endl;
    return true;

}
/**
 * @brief Stops the server and releases all network resources.
 *
 * Clears the running flag, forcefully shuts down and closes both the server
 * and client sockets using SHUT_RDWR to unblock any threads blocked in
 * accept() or recv(), then joins the accept and receive threads.
 *
 * @note Does nothing if the server is not currently running.
 */
void NetworkServer::stop() {
    if (!running) return;
    running = false;

    if (serverFd >= 0) {
        shutdown(serverFd, SHUT_RDWR);  // more forceful than just close
        close(serverFd);
        serverFd = -1;
    }

    if (clientFd >= 0) {
        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
        clientFd = -1;
    }

    if (acceptThread.joinable()) acceptThread.join();
    if (receiveThread.joinable()) receiveThread.join();
}
/**
 * @brief Registers a callback to be invoked when a command packet is received.
 *
 * The callback is fired by the receive loop each time a complete packet arrives
 * from the connected client, passing the parsed command and any associated payload.
 *
 * @param callback A callable that accepts a Command and a byte payload vector.
 */
void NetworkServer::onCommand(std::function<void(Command,const std::vector<uint8_t>&)> callback) {
    commandCallback = callback;
}
/**
 * @brief Sends raw bytes to the connected client, blocking until all bytes are sent.
 *
 * Loops over send() calls until the entire buffer has been transmitted.
 * If send() returns an error or zero bytes, the client is marked as disconnected
 * and the method returns early.
 *
 * @param data Pointer to the data buffer to send.
 * @param size Number of bytes to send.
 *
 * @warning This method is not thread-safe. Concurrent calls from multiple threads
 *          may interleave data on the socket.
 */
void NetworkServer::sendRaw(const void *data, size_t size) {
    size_t sent = 0;
    const uint8_t* ptr = (const uint8_t*)data;

    while (sent < size) {
        int n = send(clientFd, ptr + sent, size - sent, 0);
        if (n <= 0) {
            clientConnected = false;
            return;
        }
        sent += n;
    }
}
/**
 * @brief Sends a structured packet (header + payload) to the connected client.
 *
 * Transmits the fixed-size PacketHeader first, followed by the variable-length
 * payload if non-empty. Does nothing if no client is currently connected.
 *
 * @param header  The PacketHeader describing the system, command, and payload size.
 * @param payload The raw payload bytes to append after the header.
 */
void NetworkServer::sendPacket(const PacketHeader& header, const std::vector<uint8_t>& payload) {
    if (!clientConnected) return;
    sendRaw(&header, sizeof(header));
    if (!payload.empty())
        sendRaw(payload.data(), payload.size());
}
/**
 * @brief Sends a JPEG-encoded video frame to the connected client.
 *
 * Constructs a PacketHeader tagged as System::Camera / Command::Frame,
 * sets the payload size to the frame length, and sends the complete packet
 * via sendPacket(). Does nothing if no client is currently connected.
 *
 * @param data   Pointer to the JPEG-encoded frame buffer.
 * @param length Size of the frame buffer in bytes.
 */
void NetworkServer::sendFrame(const uint8_t *data, size_t length) {
    if (!clientConnected) return;

    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::Frame;
    header.payloadSize = static_cast<uint32_t>(length);

    std::vector<uint8_t> payload(data, data + length);
    sendPacket(header, payload);
}
/**
 * @brief Registers a callback to be invoked when the client disconnects.
 *
 * The callback is fired by the accept loop after the receive loop exits,
 * allowing other subsystems (e.g. Camera_PI) to react to a lost connection.
 *
 * @param callback A callable with no parameters invoked on client disconnect.
 */
void NetworkServer::onDisconnect(std::function<void()> callback) {
    disconnectCallback = callback;
}
/**
 * @brief Continuously accepts incoming client connections in a loop.
 *
 * Blocks on accept() waiting for a client. On a successful connection,
 * marks the client as connected and spawns the receiveLoop thread to handle
 * incoming packets. After the receive thread exits (client disconnected),
 * fires the disconnectCallback and resumes waiting for the next connection.
 *
 * @note Runs on the acceptThread. Exits when running is set to false and
 *       the server socket is shut down, causing accept() to return an error.
 */
void NetworkServer::acceptLoop() {
    while(running) {
        clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) {
            if (!running) {
                break;
            }
            continue;
        }
        clientConnected = true;
        std::cout << "Client connected!" << std::endl;
        receiveThread = std::thread(&NetworkServer::receiveLoop, this);
        receiveThread.join();
        std::cout << "Client disconnected. Waiting for reconnect..." << std::endl;
        if (disconnectCallback) disconnectCallback();
	}
}
/**
 * @brief Continuously receives packets from the connected client.
 *
 * Reads a fixed-size PacketHeader using MSG_WAITALL, then reads the variable-length
 * payload if payloadSize is non-zero. On a complete packet, fires the commandCallback
 * with the parsed command and payload. If recv() returns zero or an error at any point,
 * the client is marked as disconnected and the loop exits.
 *
 * @note Runs on the receiveThread, spawned per client connection by acceptLoop().
 */
void NetworkServer::receiveLoop() {
    while(running && clientConnected) {
        PacketHeader header{};
        int n = recv(clientFd, &header, sizeof(header), MSG_WAITALL);

        if (n <= 0) {
            clientConnected = false;
            break;
        }

        std::vector<uint8_t> payload(header.payloadSize);
        if (header.payloadSize > 0) {
            n = recv(clientFd, payload.data(), header.payloadSize, MSG_WAITALL);
            if (n <= 0) {
                clientConnected = false;
                break;
            }
        }

        // Fire the callback with the command
        if (commandCallback) {
            commandCallback(header.command,payload);
        }
    }
}
/**
 * @brief Returns the local IP address of the server.
 *
 * @return The IP address string retrieved from `hostname -I` at construction time.
 */
std::string NetworkServer::getIP() {
    return ip;
}
/**
 * @brief Returns the port number the server is listening on.
 *
 * @return The TCP port number passed to the constructor.
 */
int NetworkServer::getPort() {
    return port;
}
