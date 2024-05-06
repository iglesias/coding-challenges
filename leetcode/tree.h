#pragma once

#include <memory>
#include <utility>
#include <vector>

using std::pair;
using std::make_unique;
using std::unique_ptr;
using std::vector;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

pair<unique_ptr<TreeNode>, vector<unique_ptr<TreeNode>>>
make_tree(const vector<int>& v) {
  auto root{std::make_unique<TreeNode>(v.at(0))};

  // FIXME const construction.
  vector<unique_ptr<TreeNode>> otherNodes;
  //TODO use actual data in v to construct the tree.
  otherNodes.push_back(make_unique<TreeNode>(9));
  otherNodes.push_back(make_unique<TreeNode>(20));
  otherNodes.push_back(make_unique<TreeNode>(15));
  otherNodes.push_back(make_unique<TreeNode>(7));

  root->left = otherNodes[0].get();
  root->right = otherNodes[1].get();
  root->right->left = otherNodes[2].get();
  root->right->right = otherNodes[3].get();

  return std::make_pair(std::move(root), std::move(otherNodes));
}
