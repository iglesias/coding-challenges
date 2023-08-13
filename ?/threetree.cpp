#include <bits/stdc++.h>

using std::begin;
using std::end;
using std::for_each;
using std::make_pair;
using std::max;
using std::transform;

using std::pair;
using std::queue;
using std::set;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

using node = unsigned int;

namespace {
  // Adjacency list.
  using tree = vector<vector<node>>;

  void bfs_from_root(tree t)
  {
    vector<size_t> heights(t.size(), 0);
    queue<pair<node, size_t>> q;
    q.emplace(make_pair(1, 0));
    while (!q.empty())
    {
      auto [n, h] = q.front();
      q.pop();

      // Because of addition of two edges for undirected.
      if (heights[n] != 0) continue;

      heights[n] = h+1;
      for_each(begin(t[n]), end(t[n]), [&q, h](node m) { q.emplace(make_pair(m, h+1)); });
    }
  }
}

using ::tree;


void mark_heights(tree t, node root, const vector<bool>& is_leave, vector<size_t>& heights, node n, set<node> ancestors={}, size_t height=0)
{
  // Because of undirected representation with two entries per edge.
  ancestors.insert(n);

  for (node i : t[n])
  {
    if (is_leave[i]) continue;
    // The deepest leave defines height.
    if (heights[i] > height) continue;
    if (ancestors.contains(i)) continue;
    heights[i] = height+1;
    if (i == root) continue;
    mark_heights(t, root, is_leave, heights, i, ancestors, height+1);
  }
}

size_t diameter(tree t, node root, const vector<bool>& is_leave, const vector<size_t>& heights, node prev_root=0)
{
  cout << "diameter(" << root << ")" << endl;
  // Tentative diameter passing by root.
  set<node> tallest_children;
  for (auto it{begin(t[root])}; it != end(t[root]); advance(it, 1))
  {
    if (*it == prev_root) continue;

    if (tallest_children.size() < 2)
    {
      tallest_children.insert(*it);
      continue;
    }

    auto cur_small{ begin(tallest_children) };
    if (heights[*it] > heights[*cur_small])
    {
      tallest_children.erase(cur_small);
      tallest_children.insert(*it);
    }
  }

  assert(tallest_children.size() <= 2);

  // Height of two tallest children plus 2 becausewe consider leaves' height zero.
  const size_t try_d{ (tallest_children.size() == 2 ?
      heights[*tallest_children.begin()] + heights[*tallest_children.rbegin()] + 2
      : 0 ) };

  // Try with diameter of children.
  size_t max_child_d{0};
  for (node n : t[root])
  {
    if (is_leave[n]) continue;
    if (n == prev_root) continue;
    max_child_d = max(max_child_d, diameter(t, n, is_leave, heights, root));
  }

  return max(try_d, max_child_d);
}

int main()
{
  tree t;

  size_t n;
  cin >> n;

  t.resize(n+1);

  for (size_t i{0}; i < n-1; i++)
  {
    node a, b;
    cin >> a >> b;
    t[a].push_back(b);
    t[b].push_back(a);
  }

  vector<bool> is_leave(n+1, false);
  transform(begin(t), end(t), begin(is_leave), [](auto&& adj_list){ return adj_list.size() == 1; });

  const node root{ static_cast<node>(find(next(begin(is_leave)), end(is_leave), false) - begin(is_leave)) };

  for (bool b : is_leave) cout << b << ' ';
  cout << endl;

  cout << root << endl;

  vector<size_t> heights(n+1, 0);
  for (node i{1}; i < n+1; i++)
  {
    if (is_leave[i]) mark_heights(t, root, is_leave, heights, i);
  }

  for (auto&& h : heights) cout << h << ' ';
  cout << endl;

  size_t d{ diameter(t, root, is_leave, heights) };
  cout << d << endl;
}

/*
8
1 2
2 3
3 4
4 5
4 6
3 7
*/

// g++ -std=c++20 -Wall -Wextra -pedantic -o threetree_refactored threetree.cpp
