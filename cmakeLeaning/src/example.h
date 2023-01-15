#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <array>
#include <stack>
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

    class SolutionisValidBST {
    public:
        void update(TreeNode* root, TreeNode* low, TreeNode* high);
        bool isValidBST(TreeNode* root);
    private:
        std::stack<TreeNode*> stk, lower_limits, upper_limits;
    };

    class BSTIterator {
    public:
        BSTIterator(TreeNode* root);
        int next(); 
        bool hasNext();
    };

} // namespace leet

#endif