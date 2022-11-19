#include "example.h"

int leet::Solution::countUnivalSubtrees(TreeNode* root) {
    return 0;
    
}

int leet::runTest() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

int leet::Factorial(int n) {
    
    return 1;
}

void leet::SolutionisValidBST::update(TreeNode* root, TreeNode* low, TreeNode* high) {
    stk.push(root);
    lower_limits.push(low);
    upper_limits.push(high);
}

bool leet::SolutionisValidBST::isValidBST(TreeNode* root) {
    TreeNode* low = nullptr;
    TreeNode* high = nullptr;

    update(root, low, high);

    while (!stk.empty())
    {
        root = stk.top();
        stk.pop();
        low = lower_limits.top();
        lower_limits.pop();
        high = upper_limits.top();
        upper_limits.pop();
        if (root == nullptr)
        {
            continue;
        }
        TreeNode* val_node = root;
        if (low != nullptr && val_node->val <= low->val)
        {
            return false;
        }
        if (high != nullptr && val_node->val >= high->val)
        {
            return false;
        }
        update(root->right, val_node, high);
        update(root->left, low, val_node);
    }
    return true;
}
