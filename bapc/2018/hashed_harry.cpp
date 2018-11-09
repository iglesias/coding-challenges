$ cat harry_test_2018_H.cpp
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

using uint = unsigned int;

const uint inf = numeric_limits<uint>::max();

/// Priority queue (via std::set binary search tree).
using cost = uint;

enum class objective : bool { bed = 0, run = 1 };

objective other(objective obj)
{
  switch (obj)
  {
  case objective::bed:
    return objective::run;
    break;
  case objective::run:
    return objective::bed;
    break;
  default:
    assert(false);
    return objective::bed;
  }
}

namespace {
  const unordered_map<objective, std::function<uint(uint, uint)>> minmax = {
    {
      objective::bed,     /* key */
      [](uint a, uint b)  /* value */
      {
        return std::min(a, b);
      }
    },
    {
      objective::run,
      [](uint a, uint b)
      {
        return std::max(a, b);
      }
    }
  }; /* brace-initializer */
} /* namespace */

struct node {

  struct identifier {
    identifier(uint _n, objective _obj) : n(_n), obj(_obj) {}

    uint n;
    objective obj;
  };

  node(uint _n, objective _obj, cost _c) : id(_n, _obj), c(_c) {}

  bool operator<(const node& that) const
  {
    if (id.obj != that.id.obj)
      return id.obj == objective::run;
    else if (c == that.c)
      return id.n < that.id.n;
    else
      return c < that.c;
  }

  identifier id;
  cost c;
};
///

/// Graph.
struct out_edge {
  out_edge(uint _dst, uint _w) : dst(_dst), w(_w) {}

  uint dst;
  uint w;
};

using graph = std::vector<std::vector<out_edge>>;
///

int main()
{
  uint n, m, s, t;
  cin >> n >> m >> s >> t;

  graph g;
  vector<uint> num_in(n, 0);
  g.resize(n);

  for (uint i = 0; i < m; i++)
  {
    uint a, b, c;
    cin >> a >> b >> c;
    // Build reversed graph.
    g[b].emplace_back(a, c);
    num_in[a]++;
  }

  if (s == t)
  {
    cout << 0 << endl;
    return 0;
  }

  assert(num_in[t] == 0);

  unordered_map<objective, vector<bool>> visited;
  visited.emplace(objective::bed, vector<bool>(n, false));
  visited.emplace(objective::run, vector<bool>(n, false));

  unordered_map<objective, vector<cost>> costs;
  costs.emplace(objective::bed, vector<cost>(n, inf));
  costs.emplace(objective::run, vector<cost>(n, 0));

  set<node> bst;
  costs[objective::bed][t] = costs[objective::run][t] = 0;

  bst.emplace(t, objective::run, 0);
  bst.emplace(t, objective::bed, 0);

  while (!bst.empty())
  {
    auto cur_it = bst.begin();
    node cur = *cur_it;
    bst.erase(cur_it);

    objective dst_obj = other(cur.id.obj);

    if (visited[cur.id.obj][cur.id.n]) continue;

    for (auto&& e : g[cur.id.n])
    {
      costs[dst_obj][e.dst] = (costs[cur.id.obj][cur.id.n] == inf) ? inf :
        ::minmax.at(dst_obj)(costs[dst_obj][e.dst], costs[cur.id.obj][cur.id.n] + e.w);

      node next = node(e.dst, objective(dst_obj), costs[dst_obj][e.dst]);

      // To update cost if it is already to visit,
      // then we remove it first.
      auto it = bst.find(next);
      if (it != bst.end()) bst.erase(it);

      // Run or blue nodes are only added when all bed or red in-nodes
      // have been relaxed.
      if (cur.id.obj == objective::bed)
      {
        if (! --num_in[e.dst]) bst.insert(std::move(next));
        bst.emplace(e.dst, objective::bed, costs[objective::bed][e.dst]);
      }
      else
      {
        bst.insert(std::move(next));
      }
    }

    visited[cur.id.obj][cur.id.n] = true;
  }

  if (s != t && (costs[objective::run][s] == 0))
  {
    cout << "infinity\n";
    return 0;
  }

  if (costs[objective::run][s] == inf) cout << "infinity\n";
  else                                 cout << costs[objective::run][s] << endl;
}
