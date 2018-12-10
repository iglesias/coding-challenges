// g++ -std=c++11 -Wall -Wextra -o harry harry.cpp
//
// Grab data from bapc website and play:
// for file in `ls -v *.in`; do echo $file && ./harry < $file > ${file::-3}.out && diff -q ${file::-3}.ans ${file::-3}.out; done

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

using namespace std;

using uint = unsigned int;

const uint inf = numeric_limits<uint>::max();

/// Priority queue (via std::set binary search tree).
using cost = uint;

enum class objective : bool { bed = 0, run = 1 };

constexpr size_t BED = static_cast<size_t>(objective::bed);
constexpr size_t RUN = static_cast<size_t>(objective::run);

size_t other(objective obj)
{
  switch (obj)
  {
  case objective::bed:
    return RUN;
    break;
  case objective::run:
    return BED;
    break;
  default:
    assert(false);
    return 0;
  }
}

namespace {
  const std::function<uint(uint, uint)> minmax[2] = {
    [](uint a, uint b)
    {
      return std::min(a, b);
    },

    [](uint a, uint b) {
      return std::max(a, b);
    }
  };
};

struct node {

  struct identifier {
    identifier(uint _n, objective _obj) : n(_n), obj(_obj) {}

    uint n;
    objective obj;
  };

  node(uint _n, objective _obj) : id(_n, _obj)
  {
    switch (id.obj)
    {
    case objective::bed:
      c = inf;
      break;
    case objective::run:
      c = 0;
      break;
    default:
      assert(false);
    }
  }

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

  array<vector<bool>, 2> visited;
  visited[BED] = visited[RUN] = vector<bool>(n, false);

  array<vector<cost>, 2> costs;
  costs[BED] = vector<cost>(n, inf);
  costs[RUN] = vector<cost>(n, 0);

  set<node> bst;
  costs[BED][t] = costs[RUN][t] = 0;

  bst.emplace(t, objective::run, 0);
  bst.emplace(t, objective::bed, 0);

  while (!bst.empty())
  {
    auto cur_it = bst.begin();
    node cur = *cur_it;
    bst.erase(cur_it);

    size_t dst_obj = other(cur.id.obj);
    size_t org_obj = (dst_obj+1)%2;

    if (visited[org_obj][cur.id.n]) continue;

    for (auto&& e : g[cur.id.n]) {

      costs[dst_obj][e.dst] = (costs[org_obj][cur.id.n] == inf) ? inf :
        ::minmax[dst_obj](costs[dst_obj][e.dst], costs[org_obj][cur.id.n] + e.w);

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
        bst.emplace(e.dst, objective::bed, costs[BED][e.dst]);
      }
      else
      {
        bst.insert(std::move(next));
      }
    }

    visited[org_obj][cur.id.n] = true;
  }

  if (s != t && (costs[RUN][s] == 0))
  {
    cout << "infinity\n";
    return 0;
  }

  if (costs[RUN][s] == inf) cout << "infinity\n";
  else                      cout << costs[RUN][s] << endl;
}
