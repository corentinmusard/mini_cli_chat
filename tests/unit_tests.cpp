#include <gtest/gtest.h>

class ReverseTests : public ::testing::Test
{
};

TEST_F(ReverseTests, simple)
{
    std::string res = "Hello";

    EXPECT_EQ(res, "Hello");

}
