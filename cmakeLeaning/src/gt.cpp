#include "gt.hpp"

// using namespace leet;
using namespace gt;

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(leet::Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(leet::Factorial(1), 1);
  EXPECT_EQ(leet::Factorial(2), 2);
  EXPECT_EQ(leet::Factorial(3), 6);
  EXPECT_EQ(leet::Factorial(8), 40320);
}

// bool gt::MyCircularQueue::enQueue(T t) {
    
//     return false;
// }
