#include "gtest/gtest.h"

TEST(TestAddInt, test_add_int_1)
{
    int res = 1 + 1;
    EXPECT_EQ(res, 2);
}