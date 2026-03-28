//
// Created by evan on 2026-03-25.
//

#include "Camera_PI.h"



Camera_PI::Camera_PI(NetworkServer &server) : recording(false),server(server) {
    initCamera();
}

Camera_PI::~Camera_PI() {
    delete allocator;
}

int Camera_PI::initCamera() {
    cm = std::make_unique<CameraManager>();
    cm->start();
    auto cameras = cm->cameras();
    if (cameras.empty()) {
        std::cout << "No cameras were identified on the system."
                  << std::endl;
        cm->stop();
        return EXIT_FAILURE;
    }

    std::string cameraId = cameras[0]->id();

    camera = cm->get(cameraId);
    if (!camera) {
        std::cerr << "Failed to get camera." << std::endl;
        cm->stop();
        return EXIT_FAILURE;
    }
    if (camera->acquire()<0) {
        std::cerr << "Failed to acquire camera." << std::endl;
        return EXIT_FAILURE;
    }
    config = camera->generateConfiguration( { StreamRole::Viewfinder } );
    if (!config) {
        std::cerr << "Failed to generate camera configuration." << std::endl;
        camera->release();
        return EXIT_FAILURE;
    }


    StreamConfiguration &streamCfg = config->at(0);
    streamCfg.pixelFormat = formats::MJPEG;
    streamCfg.size        = { 1280, 720 };
    streamCfg.bufferCount = 4;

    if (config->validate() == CameraConfiguration::Invalid) {
        std::cerr << "Camera configuration invalid." << std::endl;
        camera->release();
        return EXIT_FAILURE;
    }

    if (camera->configure(config.get()) < 0) {
        std::cerr << "Failed to configure camera." << std::endl;
        camera->release();
        return EXIT_FAILURE;
    }

    allocator = new FrameBufferAllocator(camera);



    for (StreamConfiguration &cfg : *config) {
        int ret = allocator->allocate(cfg.stream());
        if (ret < 0) {
            std::cerr << "Can't allocate buffers" << std::endl;
            camera->release();
            return EXIT_FAILURE;
        }

        size_t allocated = allocator->buffers(cfg.stream()).size();
        std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
    }

    // Create requests and attach buffers
    Stream *stream = config->at(0).stream();
    for (const auto &buffer : allocator->buffers(stream)) {
        std::unique_ptr<Request> request = camera->createRequest();
        if (!request) {
            std::cerr << "Failed to create request." << std::endl;
            camera->release();
            return EXIT_FAILURE;
        }
        if (request->addBuffer(stream, buffer.get()) < 0) {
            std::cerr << "Failed to add buffer to request." << std::endl;
            camera->release();
            return EXIT_FAILURE;
        }
        requests.push_back(std::move(request));
    }

    std::cout << "Camera initialised successfully." << std::endl;
    return EXIT_SUCCESS;
}

std::string Camera_PI::capturePhoto() {
    if (!camera) {
        std::cerr << "Camera not initialised." << std::endl;
        return "";
    }
    if (recording) {
        std::cerr << "Cannot capture photo while recording." << std::endl;
        return "";
    }
    if (requests.empty()) {
        std::cerr << "No requests available." << std::endl;
        return "";
    }

    camera->stop();
    camera->requestCompleted.disconnect();

    bool wasStreaming = streaming;
    if (wasStreaming) stopStreaming();

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "/tmp/photo_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".jpg";
    std::string outPath = oss.str();


    int warmupFrames = 5;
    int frameCount = 0;
    bool done = false;
    std::vector<uint8_t> encodedPhoto;
    Stream *stream = config->at(0).stream();

    camera->requestCompleted.connect(this, [&](libcamera::Request *req) {
        if (req->status() == libcamera::Request::RequestCancelled) return;

        frameCount++;
        if (frameCount <= warmupFrames) {
            req->reuse(libcamera::Request::ReuseBuffers);
            camera->queueRequest(req);
            return;
        }

        const libcamera::FrameBuffer *buf = req->buffers().at(stream);
        const libcamera::FrameBuffer::Plane &plane = buf->planes()[0];

        void *mem = mmap(nullptr, plane.length, PROT_READ,
                         MAP_SHARED, plane.fd.get(), 0);
        if (mem != MAP_FAILED) {
            cv::Mat yuyv(720, 1280, CV_8UC2, mem);
            cv::Mat bgr;
            cv::cvtColor(yuyv, bgr, cv::COLOR_YUV2BGR_YUYV);
            cv::imencode(".jpg", bgr, encodedPhoto);
            munmap(mem, plane.length);
        }
        done = true;
    });

    for (auto &r : requests)
        r->reuse(libcamera::Request::ReuseBuffers);

    camera->start();
    for (auto &r : requests)
        camera->queueRequest(r.get());

    const auto timeout = std::chrono::seconds(5);
    const auto t0 = std::chrono::steady_clock::now();
    while (!done) {
        if (std::chrono::steady_clock::now() - t0 > timeout) {
            std::cerr << "Photo capture timed out." << std::endl;
            camera->stop();
            camera->requestCompleted.disconnect();
            if (wasStreaming) startStreaming();
            return "";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    camera->stop();
    camera->requestCompleted.disconnect();
    lastCaptureAt = std::chrono::system_clock::now();

    std::ofstream ofs(outPath, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(encodedPhoto.data()), encodedPhoto.size());
    ofs.close();

    PacketHeader header{};
    header.system = System::Camera;
    header.command = Command::TakePhoto;
    header.payloadSize = static_cast<uint32_t>(encodedPhoto.size());
    server.sendPacket(header, encodedPhoto);
    std::cout << "Photo sent to PC (" << encodedPhoto.size() << " bytes)" << std::endl;

    // Resume streaming if it was active
    if (wasStreaming) startStreaming();

    return outPath;
}

void Camera_PI::startRecording() {
    if (recording) {
        std::cout << "Already recording." << std::endl;
        return;
    }
    if (!camera) {
        std::cerr << "Camera not initialised." << std::endl;
        return;
    }

    libcamera::Stream *stream = config->at(0).stream();

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "/tmp/video_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".mjpeg";
    devicePath = oss.str();

    videoFile = std::make_unique<std::ofstream>(devicePath, std::ios::binary);
    if (!videoFile->is_open()) {
        std::cerr << "Failed to open output file: " << devicePath << std::endl;
        return;
    }

    camera->requestCompleted.connect(this, [this, stream](libcamera::Request *req) {
        if (!recording || req->status() == libcamera::Request::RequestCancelled) return;

        const libcamera::FrameBuffer *buf = req->buffers().at(stream);
        for (const auto &plane : buf->planes()) {
            void *mem = mmap(nullptr, plane.length, PROT_READ,
                             MAP_SHARED, plane.fd.get(), 0);
            if (mem != MAP_FAILED) {
                videoFile->write(static_cast<const char *>(mem), plane.length);
                munmap(mem, plane.length);
            }
        }

        req->reuse(libcamera::Request::ReuseBuffers);
        camera->queueRequest(req);
    });

    camera->start();
    recording = true;

    for (auto &req : requests) {
        req->reuse(libcamera::Request::ReuseBuffers);
        camera->queueRequest(req.get());
    }

    std::cout << "Recording started -> " << devicePath << std::endl;
}

void Camera_PI::stopRecording() {
    if (!recording) {
        std::cout << "Not currently recording." << std::endl;
        return;
    }

    recording = false;
    camera->stop();
    camera->requestCompleted.disconnect();

    if (videoFile) {
        videoFile->flush();
        videoFile->close();
        videoFile.reset();
    }

    lastCaptureAt = std::chrono::system_clock::now();
    std::cout << "Recording stopped. File saved to: " << devicePath << std::endl;
}

void Camera_PI::startStreaming() {
    if (streaming) return;

    camera->stop();
    camera->requestCompleted.disconnect();

    Stream *stream = config->at(0).stream();

    camera->requestCompleted.connect(this, [this, stream](Request *req) {
        // Check BOTH flags — streaming might have been cleared mid-callback
        if (!streaming) return;
        if (req->status() == Request::RequestCancelled) return;

        const FrameBuffer *buf = req->buffers().at(stream);
        const FrameBuffer::Plane &plane = buf->planes()[0];

        void *mem = mmap(nullptr, plane.length, PROT_READ,
                         MAP_SHARED, plane.fd.get(), 0);
        if (mem != MAP_FAILED) {
            cv::Mat yuyv(720, 1280, CV_8UC2, mem);
            cv::Mat bgr;
            cv::cvtColor(yuyv, bgr, cv::COLOR_YUV2BGR_YUYV);

            std::vector<uint8_t> encoded;
            std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};
            cv::imencode(".jpg", bgr, encoded, params);
            server.sendFrame(encoded.data(), encoded.size());

            munmap(mem, plane.length);
        }

        if (!streaming) return;
        req->reuse(Request::ReuseBuffers);
        camera->queueRequest(req);
    });

    for (auto &r : requests)
        r->reuse(libcamera::Request::ReuseBuffers);

    camera->start();
    streaming = true;

    for (auto &r : requests)
        camera->queueRequest(r.get());

    std::cout << "Streaming started." << std::endl;
}

void Camera_PI::stopStreaming() {
    if (!streaming) return;
    camera->requestCompleted.disconnect();
    streaming = false;
    camera->stop();
    for (auto &r : requests) {
        r->reuse(libcamera::Request::ReuseBuffers);
    }
    std::cout << "Streaming stopped." << std::endl;
}

bool Camera_PI::isRecording() {
    return recording;
}
