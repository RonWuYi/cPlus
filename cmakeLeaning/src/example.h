#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <array>

#include <gtest/gtest.h>

namespace leet
{
    struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
  };

    class Solution
    {
    private:
        /* data */
    public:
        Solution(/* args */);
        virtual ~Solution();

        int countUnivalSubtrees(TreeNode* root);
    };
    
    int Factorial(int n);  // Returns the factorial of n

    int runTest();
} // namespace leet

#endif