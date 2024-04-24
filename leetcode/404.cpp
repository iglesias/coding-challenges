#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "tree.h"

int sumOfLeftLeaves(TreeNode *root) {
  if (!root or (!root->left and !root->right))
    return 0;
  int ans = 0;
  if (root->left) {
    if (!root->left->left and !root->left->right)
      ans += root->left->val;
    else
      ans += sumOfLeftLeaves(root->left);
  }
  return ans + sumOfLeftLeaves(root->right);
}

TEST(sumOfLeftLeaves, SampleInput) {
  auto root{std::make_unique<TreeNode>(3)};

  // FIXME const construction.
  std::vector<std::unique_ptr<TreeNode>> otherNodes;
  otherNodes.push_back(std::make_unique<TreeNode>(9));
  otherNodes.push_back(std::make_unique<TreeNode>(20));
  otherNodes.push_back(std::make_unique<TreeNode>(15));
  otherNodes.push_back(std::make_unique<TreeNode>(7));

  root->left = otherNodes[0].get();
  root->right = otherNodes[1].get();
  root->right->left = otherNodes[2].get();
  root->right->right = otherNodes[3].get();

  EXPECT_EQ(sumOfLeftLeaves(root.get()), 24);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
