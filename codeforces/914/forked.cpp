#include <bits/stdc++.h>

#define   ALL(a)          (a).begin(), (a).end()

using ii = std::pair<int, int>;

std::set<ii> get_attacking_cells(int a, int b, int x, int y)
{
  std::set<ii> attacking_cells;
  attacking_cells.emplace(std::make_pair(x+a, y+b));
  attacking_cells.emplace(std::make_pair(x+a, y-b));
  attacking_cells.emplace(std::make_pair(x-a, y+b));
  attacking_cells.emplace(std::make_pair(x-a, y-b));
  attacking_cells.emplace(std::make_pair(x+b, y+a));
  attacking_cells.emplace(std::make_pair(x+b, y-a));
  attacking_cells.emplace(std::make_pair(x-b, y+a));
  attacking_cells.emplace(std::make_pair(x-b, y-a));
  return attacking_cells;
}

int main()
{
  std::cin.tie(0)->sync_with_stdio(0);
  int T;
  std::cin >> T;
  for(int t{0}; t<T; t++)
  {
    int a, b; std::cin >> a >> b;
    int x_k, y_k; std::cin >> x_k >> y_k;
    int x_q, y_q; std::cin >> x_q >> y_q;

    auto const cells_k{get_attacking_cells(a, b, x_k, y_k)};
    auto const cells_q{get_attacking_cells(a, b, x_q, y_q)};

    std::vector<ii> intersection;
    std::set_intersection(ALL(cells_k), ALL(cells_q), std::back_inserter(intersection));
    std::cout << intersection.size() << '\n';
  }
}
