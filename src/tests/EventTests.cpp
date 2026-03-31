//
// Created by Braeden Patierno-Barker on 3/31/2026.
//

#include <gtest/gtest.h>
#include "../Event.h"

class EventTests : public ::testing::Test {
protected:
    void SetUp() override {
        event = new Event("2026-03-31 12:00", "Motion detected");
    }

    void TearDown() override {
        delete event;
    }

    Event* event;
};

TEST_F(EventTests, GetTimeReturnsCorrectTime) {
    EXPECT_EQ(event->getTime(), "2026-03-31 12:00");
}

TEST_F(EventTests, GetTypeReturnsCorrectType) {
    EXPECT_EQ(event->getType(), "Motion detected");
}

TEST_F(EventTests, ConstructorWithTimeOnly) {
    Event e("2026-01-01 00:00");
    EXPECT_EQ(e.getTime(), "2026-01-01 00:00");
}

TEST_F(EventTests, ConstructorWithTimeOnlyTypeIsEmpty) {
    Event e("2026-01-01 00:00");
    EXPECT_EQ(e.getType(), "");
}

TEST_F(EventTests, ConstructorWithTimeAndType) {
    Event e("2026-03-31 15:00", "Alarm triggered");
    EXPECT_EQ(e.getTime(), "2026-03-31 15:00");
    EXPECT_EQ(e.getType(), "Alarm triggered");
}