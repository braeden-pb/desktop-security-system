//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Camera.h"
#include "../Network.h"

class CameraTests : public ::testing::Test {
protected:
    void SetUp() override {
        network = new Network();
        camera = new Camera(*network);
    }

    void TearDown() override {
        delete camera;
        delete network;
    }

    Network* network;
    Camera* camera;
};

TEST_F(CameraTests, InitiallyNotRecording) {
    EXPECT_FALSE(camera->isRecording());
}

TEST_F(CameraTests, InitiallyNotStreaming) {
    EXPECT_FALSE(camera->isStreaming());
}

TEST_F(CameraTests, StartRecordingSetsRecordingTrue) {
    camera->startRecording();
    EXPECT_TRUE(camera->isRecording());
}

TEST_F(CameraTests, StopRecordingSetsRecordingFalse) {
    camera->startRecording();
    camera->stopRecording();
    EXPECT_FALSE(camera->isRecording());
}

TEST_F(CameraTests, StartStreamSetsStreamingTrue) {
    camera->startStream();
    EXPECT_TRUE(camera->isStreaming());
}

TEST_F(CameraTests, StopStreamSetsStreamingFalse) {
    camera->startStream();
    camera->stopStream();
    EXPECT_FALSE(camera->isStreaming());
}

TEST_F(CameraTests, CapturePhotoRunsWithoutCrash) {
    EXPECT_NO_THROW(camera->capturePhoto());
}