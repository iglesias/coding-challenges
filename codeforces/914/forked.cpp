// g++ -std=c++2b

#include <algorithm>
#include <array>
#include <iostream>
#include <ranges> // C++23 for views::cartesian_product
#include <set>
#include <utility>
#include <vector>

using cell = std::pair<int, int>;

std::set<cell> get_attacking_cells(int a, int b, int x, int y)
{
  using std::views::cartesian_product;
  using std::array;
  std::set<cell> attacking_cells;
  for(auto [c, d] : cartesian_product(array{a,-a}, array{b,-b})) attacking_cells.emplace(x+c, y+d);
  for(auto [c, d] : cartesian_product(array{b,-b}, array{a,-a})) attacking_cells.emplace(x+c, y+d);
  return attacking_cells;
}

auto get_case_in()
{
  int a, b, x_k, y_k, x_q, y_q;
  std::cin >> a >> b >> x_k >> y_k >> x_q >> y_q;
  return std::make_tuple(a, b, x_k, y_k, x_q, y_q);
}

int main()
{
  int T;
  std::cin >> T;
  for(int t{0}; t<T; t++){
    auto const [a, b, x_k, y_k, x_q, y_q] = get_case_in();
    auto const cells_k{get_attacking_cells(a, b, x_k, y_k)};
    auto const cells_q{get_attacking_cells(a, b, x_q, y_q)};

    std::vector<cell> intersection;
    std::set_intersection(std::cbegin(cells_k), std::cend(cells_k),
                          std::cbegin(cells_q), std::cend(cells_q),
                          std::back_inserter(intersection));

    std::cout << intersection.size() << "\n";
  }
}
