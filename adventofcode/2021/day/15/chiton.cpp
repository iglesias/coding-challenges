#include <array>
#include <climits>
#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

using std::string;
using std::vector;

int shortest_path(const vector<string>&);
void enlarge(vector<string>&);

int main() {
  vector<string> G;
  { string line;
  while (std::getline(std::cin, line)) G.push_back(std::move(line)); }
  std::cout << "Part one: " << shortest_path(G) << "\n";
  enlarge(G);
  std::cout << "Part two: " << shortest_path(G) << "\n";
}

int shortest_path(const vector<string>& G) {
  using state = std::tuple<int, int, int>;
  std::priority_queue<state, vector<state>, std::greater<state>> pq;

  const int R = static_cast<int>(G.size());
  const int C = static_cast<int>(G.at(0).size());

  auto dist = vector(R, vector(C, INT_MAX/2));
  dist[1][0] = G[1][0] - '0';
  dist[0][1] = G[0][1] - '0';

  pq.emplace(dist[1][0], 1, 0);
  pq.emplace(dist[0][1], 0, 1);

  while(!pq.empty()){
    const auto [cost, row, col] = pq.top();
    pq.pop();

    if (row == R - 1 and col == C - 1) return cost;

    constexpr std::array<std::pair<int, int>, 4> deltas = {std::array{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& [dr, dc] : deltas) {
      const int r = row + dr;
      const int c = col + dc;
      if (not(0 <= r and r < R and 0 <= c and  c < C)) continue;
      const int try_cost = cost + G[r][c] - '0';
      if (try_cost >= dist[r][c]) continue;
      dist[r][c] = try_cost;
      pq.emplace(try_cost, r, c);
    }
  }

  return -1;
}

void enlarge(vector<string>& G) {
  // initial dimensions
  size_t R0 = G.size();
  size_t C0 = G.at(0).length();
  // allocate extending current data
  constexpr int scaling{5};
  G.resize(G.size() * scaling);
  for (string& row : G)
    if (row.length()) row.resize(row.length() * scaling);
    else              row.append(scaling * C0, ' ');
  // extend columns in the first row starting from the second column
  for (size_t j = 1; j < scaling; j++)
    for (size_t row = 0; row < R0; row++) for (size_t col = 0; col < C0; col++)
      if (G[row][(j - 1) * C0 + col] == '9')
        G[row][j * C0 + col] = '1';
      else
        G[row][j * C0 + col] = char(G[row][(j - 1) * C0 + col] + 1);
  // extend all other cells start from the second row
  for (size_t i = 1; i < scaling; i++) for (size_t j = 0; j < scaling; j++)
      for (size_t row = 0; row < R0; row++) for (size_t col = 0; col < C0; col++)
        if (G[(i - 1) * R0 + row][j * C0 + col] == '9')
          G[i * R0 + row][j * C0 + col] = '1';
        else
          G[i * R0 + row][j * C0 + col] = char(G[(i - 1) * R0 + row][j * C0 + col] + 1);
}
