/**
* @file Network.cpp
 * @brief Implementation of the Network class for TCP client socket communication
 *        between the PC and the Raspberry Pi.
 * @author evan
 * @date 2026-03-21
 */
#include "Network.h"

#include <functional>
#include <thread>

/**
 * @brief Constructs a Network object with no active socket or connection.
 *
 * Initializes the socket descriptor to -1 and the connected flag to false.
 * No connection is attempted until connect() is explicitly called.
 */
Network::Network()
    : socket_(-1)        // -1 means no socket yet
    , connected_(false)  // not connected yet
{
}

/**
 * @brief Destructs the Network object, disconnecting if still connected.
 *
 * Calls disconnect() to close the socket cleanly before the object
 * is destroyed.
 */
Network::~Network() {
    if (connected_) {
        disconnect();
    }
}

/**
 * @brief Attempts to establish a non-blocking TCP connection to the Raspberry Pi.
 *
 * Performs the full non-blocking connect sequence:
 * -# Creates a TCP socket.
 * -# Sets the socket to non-blocking mode via fcntl().
 * -# Calls connect() which returns immediately without blocking.
 * -# Uses select() with a 3-second timeout to wait for the connection to complete.
 * -# Checks SO_ERROR via getsockopt() to confirm the connection succeeded.
 * -# Restores the socket to blocking mode on success.
 *
 * If any step fails or the timeout expires, the socket is closed and
 * -1 is restored, leaving the object in a disconnected state.
 *
 * @param ip   The IP address string of the Raspberry Pi to connect to.
 * @param port The TCP port number to connect on.
 * @return true if the connection was established within the 3-second timeout,
 *         false on any socket error or timeout.
 */
bool Network::connect(const std::string &ip, int port) {
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        return false;
    }

    int flags = fcntl(socket_, F_GETFL, 0);
    fcntl(socket_, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    ::connect(socket_, (sockaddr*)&addr, sizeof(addr));

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(socket_, &fdset);
    timeval tv{3, 0};

    if (select(socket_ + 1, nullptr, &fdset, nullptr, &tv) == 1) {
        int err;
        socklen_t len = sizeof(err);
        getsockopt(socket_, SOL_SOCKET, SO_ERROR, &err, &len);
        if (err == 0) {
            fcntl(socket_, F_SETFL, flags);
            connected_ = true;
            return true;
        }
    }

    close(socket_);
    socket_ = -1;
    return false;
}

/**
 * @brief Closes the TCP socket and marks the connection as inactive.
 *
 * If a valid socket exists, closes it and resets the descriptor to -1.
 * Always clears the connected flag regardless of socket state.
 */
void Network::disconnect() {
    if (socket_ >= 0) {
        close(socket_);
        socket_ = -1;
    }
    connected_ = false;

}
/**
 * @brief Returns whether the network is currently connected.
 *
 * @return true if a connection is active, false otherwise.
 */
bool Network::isConnected() const {
    return connected_;
}
/**
 * @brief Sends raw bytes to the Raspberry Pi, blocking until all bytes are sent.
 *
 * Loops over send() calls until the entire buffer has been transmitted.
 * If send() returns zero or an error at any point, the connection is marked
 * as inactive and the method returns early without completing the send.
 *
 * @param data Pointer to the data buffer to send.
 * @param size Number of bytes to send.
 */
void Network::sendRaw(const void* data, size_t size) {
    size_t sent = 0;
    const uint8_t* ptr = (const uint8_t*)data;
    while (sent < size) {
        int n = ::send(socket_, ptr + sent, size - sent, 0);
        if (n <= 0) { connected_ = false; return; }
        sent += n;
    }
}

/**
 * @brief Sends a structured packet (header + payload) to the Raspberry Pi.
 *
 * Transmits the fixed-size PacketHeader first, followed by the variable-length
 * payload if non-empty. Does nothing if not currently connected.
 *
 * @param header  The PacketHeader describing the system, command, and payload size.
 * @param payload The raw payload bytes to append after the header.
 */
void Network::send(const PacketHeader& header, const std::vector<uint8_t>& payload) {
    if (!connected_) return;
    sendRaw(&header, sizeof(header));
    if (!payload.empty()) {
        sendRaw(payload.data(), payload.size());
    }
}
/**
 * @brief Registers a callback handler for packets from a specific system.
 *
 * When a packet arrives in receiveLoop() with a matching System value,
 * the registered callback is invoked with the command and payload.
 * Only one handler per System value is supported; registering again
 * overwrites the previous handler.
 *
 * @param system   The System identifier to listen for.
 * @param callback A callable invoked with the Command and payload on packet arrival.
 */
void Network::onPacket(System system, std::function<void(Command, const std::vector<uint8_t> &)> callback) {
    handlers[system] = callback;
}
/**
 * @brief Reads a single PacketHeader from the socket, blocking until all bytes arrive.
 *
 * Uses MSG_WAITALL to ensure the full header is received in one call.
 *
 * @return The received PacketHeader.
 *
 * @note Return value should be checked — if the socket closes mid-read,
 *       the returned header will be zero-initialized and may trigger
 *       incorrect dispatch in the caller.
 */
PacketHeader Network::receiveHeader() {
    PacketHeader header;
    // MSG_WAITALL blocks until all bytes arrive
    recv(socket_, &header, sizeof(header), MSG_WAITALL);
    return header;
}
/**
 * @brief Reads a complete packet (header + payload) from the socket.
 *
 * Calls receiveHeader() to get the header, then reads payloadSize bytes
 * into a vector using MSG_WAITALL if the payload is non-empty.
 *
 * @return A byte vector containing the packet payload.
 *         Returns an empty vector if payloadSize is zero.
 */
std::vector<uint8_t> Network::receive() {
    PacketHeader header = receiveHeader();

    std::vector<uint8_t> payload(header.payloadSize);
    if (header.payloadSize > 0) {
        recv(socket_, payload.data(), header.payloadSize, MSG_WAITALL);
    }

    return payload;
}
/**
 * @brief Starts the background receive loop on a dedicated thread.
 *
 * Spawns receiveThread to run receiveLoop() asynchronously, allowing
 * incoming packets to be dispatched without blocking the calling thread.
 *
 * @note Call this after a successful connect(). Call stopReceiving() to
 *       shut the thread down cleanly.
 */
void Network::startReceiving() {
    receiveThread = std::thread(&Network::receiveLoop, this);
}
/**
 * @brief Continuously receives and dispatches packets from the Raspberry Pi.
 *
 * Runs on receiveThread. For each packet:
 * -# Reads the PacketHeader via recv() with MSG_WAITALL.
 * -# If recv() returns zero or an error, marks the connection as inactive,
 *    logs a disconnect message, and exits the loop.
 * -# Reads the variable-length payload if payloadSize is non-zero.
 * -# Looks up the registered handler for the packet's System value and
 *    invokes it with the command and payload.
 * -# If no handler is registered for the system, logs a warning to stderr.
 *
 * @note Exits automatically when connected_ is set to false, either by a
 *       recv() failure or an external call to stopReceiving().
 */
void Network::receiveLoop() {
    while (connected_) {
        // Read header
        PacketHeader header{};
        int n = recv(socket_, &header, sizeof(header), MSG_WAITALL);
        if (n <= 0) {
            connected_ = false;
            std::cout << "Disconnected from Pi." << std::endl;
            break;
        }

        std::vector<uint8_t> payload(header.payloadSize);
        if (header.payloadSize > 0) {
            n = recv(socket_, payload.data(), header.payloadSize, MSG_WAITALL);
            if (n <= 0) {
                connected_ = false;
                break;
            }
        }
        auto it = handlers.find(header.system);
        if (it != handlers.end())
            it->second(header.command, payload);
        else
            std::cerr << "No handler registered for system: "
                      << static_cast<int>(header.system) << std::endl;
    }
}

/**
 * @brief Stops the receive loop and joins the receive thread.
 *
 * Clears the connected_ flag, closes the socket to unblock any recv()
 * call currently blocking in receiveLoop(), then joins the thread.
 *
 * @note After this call the Network object can be safely destroyed or
 *       reconnected via connect().
 */
void Network::stopReceiving() {
    connected_ = false;
    if (socket_ >= 0) close(socket_);
    if (receiveThread.joinable())
        receiveThread.join();
}
