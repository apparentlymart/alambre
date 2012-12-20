
#include "gtest/gtest.h"

TEST(TestTest, OnePlusOneIsTwo) {
    EXPECT_EQ(1 + 1, 2) << "one plus one is two";
}

TEST(TestTest, OnePlusTwoIsTwelve) {
    EXPECT_EQ(1 + 2, 12) << "one plus two is twelve";
}
