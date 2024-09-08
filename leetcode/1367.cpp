#include <memory>
#include <stdexcept>

struct list_node {
    int val;
    list_node *next;
    list_node(int x) : val(x), next(nullptr) {}
    list_node(int x, list_node *next) : val(x), next(next) {}
};

struct tree_node {
    int val;
    tree_node *left;
    tree_node *right;
    tree_node(int x) : val(x), left(nullptr), right(nullptr) {}
    tree_node(int x, tree_node *left, tree_node *right) : val(x), left(left), right(right) {}
};

bool is_subpath_impl(list_node*, tree_node*);
bool is_subpath(list_node* head, tree_node* root) {
    if (!root) return false;
    return is_subpath_impl(head, root) or is_subpath(head, root->left) or is_subpath(head, root->right);
}
bool is_subpath_impl(list_node* listNode, tree_node* treeNode) {
    if (!listNode) return true;
    if (!treeNode) return false;
    if (listNode->val != treeNode->val) return false;
    return is_subpath_impl(listNode->next, treeNode->right) or is_subpath_impl(listNode->next, treeNode->left);
}

using std::unique_ptr;
using std::make_unique;

int main() {
    unique_ptr<list_node> ln0  = make_unique<list_node>(1);
    unique_ptr<list_node> ln1  = make_unique<list_node>(2, ln0.get());
    unique_ptr<list_node> list = make_unique<list_node>(2, ln1.get());

    unique_ptr<tree_node> leaf = make_unique<tree_node>(1);
    unique_ptr<tree_node> l1   = make_unique<tree_node>(2, leaf.get(), nullptr);
    unique_ptr<tree_node> l2   = make_unique<tree_node>(2, nullptr, l1.get());
    unique_ptr<tree_node> tree = make_unique<tree_node>(2, l2.get(), nullptr);

    if (!is_subpath(list.get(), tree.get())) throw std::domain_error("Test failed.");
}
