//
// Created by evan on 2026-03-10.
//

#include <gtest/gtest.h>
#include "../SecuritySystem.h"

/**
 * @brief Test fixture for SecuritySystem tests.
 *
 * Creates a fresh SecuritySystem instance before each test
 * and tears it down after, ensuring test isolation.
 */
class SecuritySystemTest : public ::testing::Test {
protected:
    /**
     * @brief Initializes a new SecuritySystem before each test.
     */
    void SetUp() override {
        system = new SecuritySystem(true);
    }

    /**
     * @brief Destroys the SecuritySystem after each test.
     */
    void TearDown() override {
        delete system;
    }



    SecuritySystem* system; ///< The system under test


};

/**
 * @brief System should be disarmed on construction.
 */
TEST_F(SecuritySystemTest, InitialStateIsDisarmed) {
    EXPECT_FALSE(system->isArmed());
}

/**
 * @brief Storage pointer should be valid on construction.
 */
TEST_F(SecuritySystemTest, InitialStorageIsNotNull) {
    EXPECT_NE(system->getStorage(), nullptr);
}


/**
 * @brief Calling arm() should put the system in armed state.
 */
TEST_F(SecuritySystemTest, ArmSetsArmedState) {
    system->arm();
    EXPECT_TRUE(system->isArmed());
}

/**
 * @brief Calling disarm() after arm() should return system to disarmed state.
 */
TEST_F(SecuritySystemTest, DisarmAfterArmSetsDisarmedState) {
    system->arm();
    system->disarm();
    EXPECT_FALSE(system->isArmed());
}


/**
 * @brief Calling disarm() on an already disarmed system should be a no-op.
 */
TEST_F(SecuritySystemTest, DisarmWhenAlreadyDisarmedIsNoOp) {
    system->disarm();
    EXPECT_FALSE(system->isArmed());
}

/**
 * @brief Calling arm() on an already armed system should remain armed.
 */
TEST_F(SecuritySystemTest, ArmWhenAlreadyArmedRemainsArmed) {
    system->arm();
    system->arm();
    EXPECT_TRUE(system->isArmed());
}

/**
 * @brief System should handle multiple arm/disarm cycles correctly.
 */
TEST_F(SecuritySystemTest, MultipleArmDisarmCycles) {
    for (int i = 0; i < 5; i++) {
        system->arm();
        EXPECT_TRUE(system->isArmed());
        system->disarm();
        EXPECT_FALSE(system->isArmed());
    }
}

/**
 * @brief Correct PIN should return true.
 */
TEST_F(SecuritySystemTest, ValidatePINCorrectPIN) {
    EXPECT_TRUE(system->validatePIN("123"));
}

/**
 * @brief Wrong PIN should return false.
 */
TEST_F(SecuritySystemTest, ValidatePINWrongPIN) {
    EXPECT_FALSE(system->validatePIN("0000"));
}

/**
 * @brief Empty string should not validate as a correct PIN.
 */
TEST_F(SecuritySystemTest, ValidatePINEmptyString) {
    EXPECT_FALSE(system->validatePIN(""));
}


/**
 * @brief PIN with correct digits but extra characters should not validate.
 */
TEST_F(SecuritySystemTest, ValidatePINExtraCharacters) {
    EXPECT_FALSE(system->validatePIN("12345"));
    EXPECT_FALSE(system->validatePIN(" 1234"));
    EXPECT_FALSE(system->validatePIN("1234 "));
}









