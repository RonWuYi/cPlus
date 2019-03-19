//
// Created by hdc on 3/18/19.
//
#include "max.h"
#include <gtest/gtest.h>

namespace {
    class maxTest : public testing::Test{
    protected:
        int i = 23;
        int j = 232;

        float f1 = 324.234;
        float f2 = 23.33;
    };

    TEST_F(maxTest, DefaultConstructor) {
        // You can access data in the test fixture here.
        EXPECT_EQ(j, Max(i, j));
        EXPECT_EQ(f1, Max(f2, f1));
    }
}
