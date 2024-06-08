#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using std::vector;

void fill_sierpinski(vector<vector<char>>& g, size_t rs, size_t re, size_t cs, size_t ce) {
  if (rs == re) {
    assert(cs == ce);
    g[rs][cs] = '#';
    return;
  }

  assert((re - rs + 1) == (ce - cs + 1));
  const size_t size = re - rs + 1;

  for (size_t i = rs + size/3; i < rs + (2*size)/3; i++)
    for (size_t j = cs + size/3; j < cs + (2*size)/3; j++)
      g[i][j] = '.';

  for (size_t i = 0; i < 3; i++) for (size_t j = 0; j < 3; j++) {
      if ((i == 1) and (j == 1)) continue;
      fill_sierpinski(g, rs + (i*size)/3, rs + ((i + 1)*size)/3 - 1,
                         cs + (j*size)/3, cs + ((j + 1) *size)/3 - 1);
    }
}

int main() {
  int N;
  std::cin >> N;

  if (N == 0) {
    std::cout << "#\n";
    return 0;
  }

  const size_t grid_size = static_cast<size_t>(std::pow(3, N));
  vector<vector<char>> g(grid_size, vector<char>(grid_size, 'o'));

  fill_sierpinski(g, 0, grid_size - 1, 0, grid_size - 1);

  for (size_t i = 0; i < grid_size; i++) {
    for (size_t j = 0; j < grid_size; j++)
      std::cout << g[i][j];
    std::cout << '\n';
  }
}
