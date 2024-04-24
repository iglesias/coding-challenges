#include <stdexcept>

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
  const auto& [root, _] = make_tree({3, 9, 20, 15, 7});
  EXPECT_EQ(sumOfLeftLeaves(root.get()), 24);
}

TEST(make_tree, EmptyVector) {
  EXPECT_THROW(static_cast<void>(make_tree({})), std::out_of_range);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
