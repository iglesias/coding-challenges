#include <bits/stdc++.h>

#include "solver.h"

auto main() -> int {
  int D, P, capacity;
  std::cin >> D >> P >> capacity;

  std::vector<Event> deliveries(D);
  std::vector<Event> pickups(P);

  for (int i = 0; i < D; ++i) {
    char lparen, rparen, sep[2];
    std::cin >> lparen >> deliveries[i].x >> sep >> deliveries[i].y >> rparen >> deliveries[i].capacity;
  }

  std::cout << "Vehicle's capacity: " << capacity << '\n';
  std::cout << "Deliveries (x, y, capacity):\n";
  for (auto delivery : deliveries)
    std::cout << "  " << std::setw(3) << delivery.x << ' '
                      << std::setw(3) << delivery.y << ' '
                      << std::setw(3) << delivery.capacity << '\n';

  for (int i = 0; i < P; ++i) {
    char lparen, rparen, sep[2];
    std::cin >> lparen >> pickups[i].x >> sep >> pickups[i].y >> rparen >> pickups[i].capacity;
  }

  std::cout << "Pickups:\n";
  for (auto pickup : pickups)
    std::cout << "  " << std::setw(3) << pickup.x << ' '
                      << std::setw(3) << pickup.y << ' '
                      << std::setw(3) << pickup.capacity << '\n';

  auto [max_deliveries, route] = solve(deliveries, pickups, capacity);

  std::cout << "Maximum number of deliveries: " << max_deliveries << std::endl;
  std::cout << "Route: ";
  for (auto loc : route) {
    std::cout << "(" << loc.first << ", " << loc.second << ") ";
  }
  std::cout << std::endl;

  return 0;
}
