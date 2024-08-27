#include <cmath>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::vector;
using std::unordered_map;

double max_probability([[maybe_unused]] int n, const vector<vector<int>>& edges, const vector<double>& probs, int start_node, int end_node) {
  std::priority_queue<std::pair<double, int>, vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;

  unordered_map<int, double> dist;
  dist[start_node] = .0;
  pq.emplace(0., start_node);

  double ans = .0;

  unordered_map<int, unordered_map<int, double>> edges_map;
  for (size_t i = 0; i < edges.size(); i++) {
    edges_map[edges[i][0]].emplace(edges[i][1], -std::log(probs[i]));
    edges_map[edges[i][1]].emplace(edges[i][0], -std::log(probs[i]));
  }

  while(!pq.empty()) {
    auto const top{pq.top()};
    pq.pop();
    for (auto const& [dst, cost] : edges_map[top.second]) {
        if (dst == end_node) ans = std::max(ans, std::exp(-top.first-cost));
        if (!dist.contains(dst) or dist[dst] > top.first + cost) {
            dist[dst] = top.first + cost;
            pq.emplace(top.first + cost, dst);
        }
    }
  }

  return ans;
}

TEST(path_with_maximum_probability, a) {
    const vector<vector<int>> edges{{0, 1}, {1, 2}, {0, 2}};
    const vector<double> probs{.5, .5, .2};
    EXPECT_EQ(max_probability(3, edges, probs, 0, 2), .25);
}

TEST(path_with_maximum_probability, b) {
    const vector<vector<int>> edges{{0, 1}, {1, 2}, {0, 2}};
    const vector<double> probs{.5, .5, .3};
    EXPECT_EQ(max_probability(3, edges, probs, 0, 2), .3);
}

TEST(path_with_maximum_probability, c) {
    const vector<vector<int>> edges{{0, 1}};
    const vector<double> probs{.5};
    EXPECT_EQ(max_probability(3, edges, probs, 0, 2), .0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
