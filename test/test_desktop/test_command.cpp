#include <unity.h>
#include "command.hpp"


void test_no_argument() {
    Command cmd("help\n");
    TEST_ASSERT_EQUAL_STRING("help", cmd.getCommand());
    TEST_ASSERT_EQUAL(0, cmd.argumentCount());
    TEST_ASSERT_EQUAL(false, cmd.hasArgument());
}

void test_one_argument() {
    Command cmd("wifi-conf MySSID\n");
    TEST_ASSERT_EQUAL_STRING("wifi-conf", cmd.getCommand());
    TEST_ASSERT_EQUAL(1, cmd.argumentCount());
    TEST_ASSERT_EQUAL(true, cmd.hasArgument());
    TEST_ASSERT_EQUAL_STRING("MySSID", cmd.nextArgument());
    TEST_ASSERT_EQUAL(0, cmd.argumentCount());
    TEST_ASSERT_EQUAL(false, cmd.hasArgument());
}

void test_two_arguments() {
    Command cmd("wifi-conf MySSID MyPassword\n");
    TEST_ASSERT_EQUAL_STRING("wifi-conf", cmd.getCommand());
    TEST_ASSERT_EQUAL(2, cmd.argumentCount());
    TEST_ASSERT_EQUAL(true, cmd.hasArgument());
    TEST_ASSERT_EQUAL_STRING("MySSID", cmd.nextArgument());
    TEST_ASSERT_EQUAL(1, cmd.argumentCount());
    TEST_ASSERT_EQUAL(true, cmd.hasArgument());
    TEST_ASSERT_EQUAL_STRING("MyPassword", cmd.nextArgument());
    TEST_ASSERT_EQUAL(0, cmd.argumentCount());
    TEST_ASSERT_EQUAL(false, cmd.hasArgument());
}

void test_extra_spaces() {
    Command cmd("  wifi-conf   MySSID    MyPassword   \n");
    TEST_ASSERT_EQUAL_STRING("wifi-conf", cmd.getCommand());
    TEST_ASSERT_EQUAL(2, cmd.argumentCount());
    TEST_ASSERT_EQUAL(true, cmd.hasArgument());
    TEST_ASSERT_EQUAL_STRING("MySSID", cmd.nextArgument());
    TEST_ASSERT_EQUAL(1, cmd.argumentCount());
    TEST_ASSERT_EQUAL(true, cmd.hasArgument());
    TEST_ASSERT_EQUAL_STRING("MyPassword", cmd.nextArgument());
    TEST_ASSERT_EQUAL(0, cmd.argumentCount());
    TEST_ASSERT_EQUAL(false, cmd.hasArgument());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_no_argument);
    RUN_TEST(test_one_argument);
    RUN_TEST(test_two_arguments);
    RUN_TEST(test_extra_spaces);
    UNITY_END();
}

