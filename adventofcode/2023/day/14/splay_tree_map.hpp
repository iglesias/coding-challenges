#include <memory>
#include <functional>
#include <utility>

namespace dts {

//FIXME design
namespace splay_tree {

using std::pair;

// Initial version result of fixing memory and refactoring wikipedia's splay tree ¯\_(ツ)_/¯
template<typename K, typename V> class map {

  //FIXME generic
  std::less<K> comp;

  struct node {
    node *left, *right;
    node *parent;
    pair<K, V> key_value;
    K key;
    node(const pair<K, V>& init = pair<K, V>()) :
      left(nullptr), right(nullptr), parent(nullptr), key_value(init) {
        key = key_value.first;
      }
    ~node() { }
  } *root;

  void left_rotate(node *x) {
    node *y = x->right;
    if (y) {
      x->right = y->left;
      if (y->left) y->left->parent = x;
      y->parent = x->parent;
    }

    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    if (y) y->left = x;
    x->parent = y;
  }

  void right_rotate(node *x) {
    node *y = x->left;
    if (y) {
      x->left = y->right;
      if (y->right) y->right->parent = x;
      y->parent = x->parent;
    }
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    if (y) y->right = x;
    x->parent = y;
  }

  void splay(node *x) {
    while (x->parent) {
      if (!x->parent->parent) {
        if (x->parent->left == x) right_rotate(x->parent);
        else left_rotate(x->parent);
      } else if (x->parent->left == x && x->parent->parent->left == x->parent) {
        right_rotate(x->parent->parent);
        right_rotate(x->parent);
      } else if (x->parent->right == x && x->parent->parent->right == x->parent) {
        left_rotate(x->parent->parent);
        left_rotate(x->parent);
      } else if (x->parent->left == x && x->parent->parent->right == x->parent) {
        right_rotate(x->parent);
        left_rotate(x->parent);
      } else {
        left_rotate(x->parent);
        right_rotate(x->parent);
      }
    }
  }

  void free(node **z) {
    if (!(*z)) return;
    if ((*z)->left or (*z)->right) {
      free(&((*z)->left));
      free(&((*z)->right));
    }
    delete *z;
    *z = nullptr;
  }

public:
  map() : root(nullptr) { }
  ~map() {
    free(&root);
  }

  void emplace(const K& key, const V& value) {
    node *z = root;
    node *p = nullptr;

    while (z) {
      p = z;
      if (comp(z->key, key)) z = z->right;
      else z = z->left;
    }

    // TODO fix leak
    z = new node(std::make_pair(key, value));
    z->parent = p;

    if (!p) root = z;
    else if (comp(p->key, z->key)) p->right = z;
    else p->left = z;

    splay(z);
  }

  bool contains(const K& key) {
    node *z = root;
    while (z) {
      if (comp(z->key, key)) z = z->right;
      else if (comp(key, z->key)) z = z->left;
      else { splay(z); return true; }
    }
    return false;
  }

  //FIXME precondition: contains(key) (or insert(key)) was called last,
  // with no other insertion or lookup after it
  V at([[maybe_unused]] const K& key) {
    return root->key_value.second;
  }
};

}  // namespace splay_tree

}  // namespace dts
