#include "gt.hpp"

// using namespace leet;
using namespace gt;

// Tests factorial of 0.
TEST(FactorialTest, DISABLED_HandlesZeroInput) {
  EXPECT_EQ(leet::Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, DISABLED_HandlesPositiveInput) {
  EXPECT_EQ(leet::Factorial(1), 1);
  EXPECT_EQ(leet::Factorial(2), 2);
  EXPECT_EQ(leet::Factorial(3), 6);
  EXPECT_EQ(leet::Factorial(8), 40320);
}

TEST(SolutionisValidBST, test02) {
  TreeNode* root = new TreeNode(5);
  TreeNode* left = new TreeNode(1);
  TreeNode* right = new TreeNode(4);
  TreeNode* subleft = new TreeNode(3);
  TreeNode* subright = new TreeNode(6);
  root->left = left;
  root->right = right;
  root->right->left = subleft;
  root->right->right = subright;
  leet::SolutionisValidBST s{};
  EXPECT_EQ(s.isValidBST(root), false);
}

TEST(SolutionisValidBST, test01) {
  TreeNode* root = new TreeNode(2);
  TreeNode* left = new TreeNode(1);
  TreeNode* right = new TreeNode(3);
  root->left = left;
  root->right = right;
  leet::SolutionisValidBST s{};
  EXPECT_EQ(s.isValidBST(root), true);
}
// bool gt::MyCircularQueue::enQueue(T t) {
    
//     return false;
// }
