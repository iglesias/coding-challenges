// clang++ -std=c++23 -stdlib=libc++ -o lambdaman lambdaman.cpp

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <mdspan>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "lambdaman_input.hpp"

using std::string;
using std::unordered_set;
using std::vector;

string solve(const grid_t&, const point&, const int);

int main(int argc, char **argv) {
  //const grid_t G = read_input("./lambdaman.2.in");
  const grid_t G = (argc > 1 ? read_input(argv[1]) : read_input());
  for (const auto& g : G) {
    for (size_t i = 0; i < g.size(); i++)
      std::cout << g[i];
    std::cout << '\n';
  }

  const point starting_position = find_starting_position(G);
  const int num_pills = count_num_pills(G);
  std::cout << "starting_position.r=" << starting_position.r << 
               " starting_position.c=" << starting_position.c <<
               " num_pills=" << num_pills << '\n';
  std::cout << solve(G, starting_position, num_pills) << '\n';
}

struct state {
  std::bitset<num_rows_limit * num_cols_limit> bitmask{};
  string path{};
  int num_pills_consumed = 0;
  point coord{};
  state(const point& p) : coord{p} {}
  bool operator<(const state& other) const {
    return this->num_pills_consumed < other.num_pills_consumed;
    //return this->path.length() < other.path.length();
  }
};

const std::array<point, 4> deltas{point{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

char delta_to_move(const point& delta) {
  if (delta.r == 0) {
    if (delta.c == -1) {
      return 'L';
    } else if (delta.c == 1) {
      return 'R';
    } else throw std::runtime_error("Unexpected delta row and col combination");
  } else if (delta.r == 1) {
    if (delta.c) throw std::runtime_error("Unexpected delta col.");
    return 'D';
  } else if (delta.r == -1) {
    if (delta.c) throw std::runtime_error("Unexpected delta col.");
    return 'U';
  } else throw std::runtime_error("Unexpected delta row.");
  std::unreachable();
}

// BFS that keeps track of ongoing paths storing them as new cells are reached
vector<string> find_all_pairs_shortest_path(const grid_t& G, const point& starting_position) {
  vector<string> paths(num_rows_limit * num_cols_limit * num_rows_limit * num_cols_limit, "");
  auto pathsmds = std::mdspan(paths.data(), num_rows_limit, num_cols_limit, num_rows_limit, num_cols_limit);
  std::queue<point> nextQ;
  const auto find_shortest_paths_from = [&pathsmds, &nextQ](const grid_t& G, const point& position){
    static bool first_call = true;
    std::queue<std::pair<string, point>> Q;
    std::bitset<num_rows_limit * num_cols_limit> Qed{};
    Qed.set(static_cast<size_t>(position.c * static_cast<int>(num_rows_limit) + position.r));
    Q.emplace("", position);
    while (!Q.empty()) {
      std::pair<string, point> current = Q.front();
      Q.pop();
      for (const auto& delta : deltas) {
        const point tentative{current.second.r + delta.r, current.second.c + delta.c};
        if (!within_bounds(tentative.r, tentative.c)) continue;
        if (G[tentative.r][tentative.c] == '.' and !Qed.test(tentative.c * num_rows_limit + tentative.r)) {
          pathsmds[position.r, position.c, tentative.r, tentative.c] = current.first + delta_to_move(delta);
          if (first_call) nextQ.push(tentative);
        }
        if (G[tentative.r][tentative.c] == '#') continue;
        if (!Qed.test(tentative.c * num_rows_limit + tentative.r)) {
          Q.emplace(current.first + delta_to_move(delta), tentative);
          Qed.set(tentative.c * num_rows_limit + tentative.r);
        }
      }
    }
    first_call = false;
  };
  find_shortest_paths_from(G, starting_position);
  while (!nextQ.empty()) {
    find_shortest_paths_from(G, nextQ.front());
    nextQ.pop();
  }
  return paths;
}

// return the paths to some closest unconsumed pills and their positions in the grid
vector<std::pair<string, point>> search(const grid_t& G, state& s, auto pathsmds) {
  vector<std::pair<string,point>> ans;
  for (size_t i = 0; i < G.size(); i++) for (size_t j = 0; j < G[i].size(); j++) {
    if (G[i][j] == '.' and !s.bitmask.test(j * num_rows_limit + i)) {
      ans.emplace_back(pathsmds[s.coord.r, s.coord.c, i, j], point(static_cast<int>(i), static_cast<int>(j)));
      if (ans.size() > 1) {
        size_t worst_length = ans[0].first.length();
        size_t worst_index = 0;
        for (size_t k = 1; k < ans.size(); k++)
          if (worst_length < ans[k].first.length()) {
            worst_length = ans[k].first.length();
            worst_index = k;
          }
        ans.erase(ans.begin() + worst_index);
      }
    }
  }

  if (ans.empty()) throw std::runtime_error("Unexpected: no unconsumed pills could be found.");
  return ans;
}

void console_output(const point& starting_position, const grid_t& G, const state& current, const unordered_set<string>& PQed) {
  /*
  std::cout << "PQ.size()=" << PQ.size() << " num_pills_consumed=" << current.num_pills_consumed
            << " path.length=" << current.path.length() << " best path length=" << ans.length() << "\n";
  */
  grid_t canvas = G;
  point coord = starting_position;
  for (const char c : current.path) {
    canvas[coord.r][coord.c] = c;
    switch (c) {
      case 'L':
        coord.c--;
        break;
      case 'R':
        coord.c++;
        break;
      case 'U':
        coord.r--;
        break;
      case 'D':
        coord.r++;
        break;
      default:
        throw std::runtime_error("Invalid value in path while printing.");
    }
  }
  for (const auto& g : canvas) {
    for (size_t i = 0; i < g.size(); i++) std::cout << g[i];
    std::cout << '\n';
  }
  std::cout << (current.path.length() >= 150 ? current.path.substr(current.path.length() - 150) : current.path) << "\n";
  std::cout << PQed.size() << "\n";
  std::cout << "\033[" << (num_rows_limit + 2) << "A\033[" << num_cols_limit << "D";
}

string solve(const grid_t& G, const point& starting_position, const int num_pills) {
  const vector<string> paths = find_all_pairs_shortest_path(G, starting_position);
  auto pathsmds = std::mdspan(paths.data(), num_rows_limit, num_cols_limit, num_rows_limit, num_cols_limit);
  std::priority_queue<state, vector<state>> PQ;
  state starting{starting_position};
  PQ.push(starting);
  unordered_set<string> PQed;
  string ans;
  while (!PQ.empty()) {
    state current = PQ.top();
    PQ.pop();
    console_output(starting_position, G, current, PQed);
    // finish when the number of consumed . (aka pills) is equal to
    // the number initially counted (the number of . in the grid G)
    if (current.num_pills_consumed == num_pills) {
      if (ans.empty() || ans.length() > current.path.length())
        ans = current.path;
      continue;
    }
    // pruning
    if (ans.length() and current.path.length() >= ans.length()) continue;
    if (ans.length() and (static_cast<int>(ans.length() - current.path.length()) <= (num_pills - current.num_pills_consumed))) continue;
    //
    // 1st search for adjacent cells with a pill
    vector<int> distances_to_adjacent;
    for (const auto& delta : deltas) {
      const point tentative{current.coord.r + delta.r, current.coord.c + delta.c};
      if (!within_bounds(tentative.r, tentative.c)) continue;
      if (G.at(tentative.r).at(tentative.c) == '.' and !current.bitmask.test(tentative.c * num_rows_limit + tentative.r)) {
        distances_to_adjacent.push_back(static_cast<int>(
                    pathsmds[starting_position.r, starting_position.c, tentative.r, tentative.c].length()));
      }
    }
    const bool expanded = distances_to_adjacent.size() > 0;
    const int min_distance = expanded ? *std::ranges::min_element(distances_to_adjacent) : -1;
    for (const auto& delta : deltas) {
      const point tentative{current.coord.r + delta.r, current.coord.c + delta.c};
      if (!within_bounds(tentative.r, tentative.c)) continue;
      if (static_cast<int>(pathsmds[starting_position.r, starting_position.c, tentative.r, tentative.c].length()) > min_distance) continue;
      if (PQed.contains(current.path + delta_to_move(delta))) continue;
      if (G.at(tentative.r).at(tentative.c) == '.' and !current.bitmask.test(tentative.c * num_rows_limit + tentative.r)) {
        state next = current;
        next.bitmask.set(tentative.c * num_rows_limit + tentative.r);
        next.coord = tentative;
        next.num_pills_consumed += 1;
        next.path += delta_to_move(delta);
        PQ.push(next);
        PQed.insert(next.path);
#ifdef LOGGER
        std::cout << ">> moving " << next.path.at(next.path.length() - 1) << " to adjacent cell with unconsumed pill"
                  << " having moved " << next.path.length() << " times so far"
                  << " and with " << num_pills - next.num_pills_consumed << " pills remaining\n";
#endif
      }
    }
    // 2nd then, when there are no adjacent cells with a pill (and there still are remaining pills),
    // then do a new search for the "closest"
    // moving at once all the LRUD... needed to arrive at that pill, consume it and queue:
    if (expanded) continue;
    const auto& search_results = search(G, current, pathsmds);
    if (search_results.empty())
      throw std::runtime_error("There are unconsumed pills, but no path to an unconsumed pill could be found.");
    for (const auto& [path_to_closest_unconsumed, closest] : search_results) {
      state next = current;
      next.bitmask.set(closest.c * num_rows_limit + closest.r);
      next.coord = closest;
      next.num_pills_consumed += 1;
      next.path += path_to_closest_unconsumed;
      if (PQed.contains(next.path)) continue;
      PQ.push(next);
      PQed.insert(next.path);
#ifdef LOGGER
      std::cout << ">> moving " << path_to_closest_unconsumed << " to closest cell with unconsumed pill"
                << " having moved " << next.path << " so far\n";
#endif
    }
  }
  return ans;
}
