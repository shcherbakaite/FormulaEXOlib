#include "library.h"

#include <gtest/gtest.h>

#include <string>

TEST(LibraryTest, HelloPrintsExpectedMessage) {
    testing::internal::CaptureStdout();
    hello();
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Hello, World!\n");
}
