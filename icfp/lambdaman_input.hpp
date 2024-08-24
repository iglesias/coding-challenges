#pragma once

#include <array>
#include <fstream>
#include <string>
#include <string_view>

constexpr size_t num_rows_limit = 25;
constexpr size_t num_cols_limit = 25;

constexpr bool within_bounds(const int row, const int col) {
  return row >= 0 && col >= 0 &&
    row < static_cast<int>(num_rows_limit) &&
    col < static_cast<int>(num_cols_limit);
}

using grid_t = std::array<std::array<char, num_cols_limit>, num_rows_limit>;
struct point { int r, c; };

constexpr void fill_blank(grid_t& G) {
  for (auto& g : G)
    for (char& gi : g)
      gi = '_';
}

inline grid_t read_input(std::string_view fname="lambdaman.in") {
  grid_t G{};
  fill_blank(G);
  std::ifstream fin(fname.data());
  std::string line;
  size_t i = 0;
  for (;;) {
    std::getline(fin, line);
    if (line.empty()) break;
    if (line.length() > num_cols_limit)
      throw std::runtime_error("Input line length larger than the maximum number of columns.");
    if (i == num_rows_limit)
      throw std::runtime_error("Input number of lines larger than the maximum number of rows.");
    for (size_t j = 0; j < line.length(); j++)
      G[i][j] = line[j];
    i++;
  }
  return G;
}

constexpr void pad_row_by_row(grid_t& G) {
  for (auto& g : G) {
    if (g.at(0) == '_') break;
    if (g.size() < 2 or g[g.size() - 2] != '_')
      throw std::runtime_error("Unavailable space for padding.");
    char to_shift = g[0];
    g[0] = '#';
    size_t i = 1;
    while (i < g.size()) {
      const char next = g[i];
      g[i++] = to_shift;
      if (next == '_') break;
      to_shift = next;
    }
    g[i] = '#';
  }
}

constexpr void pad_col_by_col(grid_t& G) {
  for (size_t j = 0; j < num_cols_limit; j++) {
    if (G.at(0)[j] == '_') break;
    char to_shift = G[0][j];
    G[0][j] = '#';
    size_t i = 1;
    while (i < num_rows_limit) {
      const char next = G[i][j];
      G[i++][j] = to_shift;
      if (next == '_') break;
      to_shift = next;
    }
    if (i == num_rows_limit)
      throw std::runtime_error("Unavailable space for padding.");
    G[i][j] = '#';
  }
}

constexpr void pad_input(grid_t& G) {
  pad_row_by_row(G);
  pad_col_by_col(G);
}

constexpr int count_num_pills(const grid_t& G) {
  int ans = 0;
  for (const auto& g : G)
    for (size_t i = 0; i < g.size(); i++)
      ans += g[i] == '.';
  return ans;
}

constexpr point find_starting_position(const grid_t& G) {
  point c;
  bool found = false;
  for (size_t i = 0; i < G.size(); i++)
    for (size_t j = 0; j < G[i].size(); j++)
      if (G[i][j] == 'L') {
        if (found)
          throw std::runtime_error("More than one L (starting position) found.");
        found = true;
        c.r = static_cast<int>(i), c.c = static_cast<int>(j);
      }
  if (!found)
    throw std::runtime_error("No L (starting position) found.");
  return c;
}
