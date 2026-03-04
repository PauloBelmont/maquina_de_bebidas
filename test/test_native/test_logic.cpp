#include <unity.h>
#include "MachineLogic.h"

// Required by Unity
void setUp(void) {}
void tearDown(void) {}

void test_coin_logic() {
    // Your code says < 25mm is a coin
    TEST_ASSERT_TRUE(MachineLogic::isCoinDetected(10));
    TEST_ASSERT_TRUE(MachineLogic::isCoinDetected(24));
    
    // 25mm and above should fail
    TEST_ASSERT_FALSE(MachineLogic::isCoinDetected(25));
    TEST_ASSERT_FALSE(MachineLogic::isCoinDetected(100));
}

void test_cup_placement_logic() {
    // Your code says > 20 and <= 60
    TEST_ASSERT_TRUE(MachineLogic::isCupCorrectlyPlaced(21));
    TEST_ASSERT_TRUE(MachineLogic::isCupCorrectlyPlaced(60));
    
    // Boundaries
    TEST_ASSERT_FALSE(MachineLogic::isCupCorrectlyPlaced(20)); // Too close
    TEST_ASSERT_FALSE(MachineLogic::isCupCorrectlyPlaced(61)); // Too far
}

void test_relay_logic() {
    // Test your RELE_ATIVO_BAIXO = true logic
    bool activeLow = true;
    
    // To Turn ON (true), result should be LOW (0)
    TEST_ASSERT_EQUAL(0, MachineLogic::getRelayLevel(true, activeLow));
    
    // To Turn OFF (false), result should be HIGH (1)
    TEST_ASSERT_EQUAL(1, MachineLogic::getRelayLevel(false, activeLow));
}

void test_distance_math() {
    // Formula: (duration * 10) / 58
    // If duration is 580, distance should be 100mm
    TEST_ASSERT_EQUAL(100, MachineLogic::calculateDistanceMM(580));
    
    // If duration is 290, distance should be 50mm
    TEST_ASSERT_EQUAL(50, MachineLogic::calculateDistanceMM(290));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_coin_logic);
    RUN_TEST(test_cup_placement_logic);
    RUN_TEST(test_relay_logic);
    RUN_TEST(test_distance_math);
    UNITY_END();
    return 0;
}
