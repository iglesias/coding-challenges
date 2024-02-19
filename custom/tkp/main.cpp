#include <bits/stdc++.h>

struct Event {
  int x, y;
  int capacity;
};

// Forward declaration.
std::pair<std::vector<std::pair<int, int>>, int> output_route(
    const std::vector<Event>& deliveries, const Event& pickup, int capacity);

std::pair<int, std::vector<std::pair<int, int>>> solve(const std::vector<Event>& deliveries,
    const std::vector<Event>& pickups, int capacity) {
  const int D = (int)(deliveries.size());

  // Initialize dynamic programming table.
  std::vector<std::vector<int>> dp(D + 1, std::vector<int>(capacity + 1, 0));

  // Fill the table.
  for (int i = 1; i <= D; ++i) {
    for (int c = 1; c <= capacity; ++c) {
      // Check if current capacity allows delivery.
      if (deliveries[i - 1].capacity <= c) {
        // Consider visiting delivery.
        dp[i][c] = std::max(dp[i - 1][c], dp[i - 1][c - deliveries[i - 1].capacity] + 1);
      } else {
        // No delivery possible, maintain previous value.
        dp[i][c] = dp[i - 1][c];
      }
    }
  }

  std::vector<Event> picked_deliveries;
  int i = D, c = capacity;
  while (i > 0 && c > 0) {
      if (dp[i][c] != dp[i - 1][c]) {
          picked_deliveries.push_back(deliveries[i - 1]);
          c -= deliveries[i - 1].capacity;
      }
      --i;
  }

  // Calculate route.
  std::vector<std::pair<int, int>> best_route;
  int best_cost = 100000;
  const int P = (int)(pickups.size());
  for (int p = 0; p < P; p++) {
    auto [route, cost] = output_route(picked_deliveries, pickups[p], capacity);
    if(cost < best_cost) {
      best_route = route;
      best_cost = cost;
    }
  }

  return {dp[D][capacity], best_route};
}

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
  route.insert(route.begin(), {0, 0});
  std::cout << "Route: ";
  for (auto loc : route) {
      std::cout << "(" << loc.first << ", " << loc.second << ") ";
  }
  std::cout << std::endl;

  return 0;
}

std::pair<std::vector<std::pair<int, int>>, int> output_route(const std::vector<Event>& deliveries, const Event& pickup, int capacity) {
  std::vector<std::pair<int, int>> best_route;
  int best_cost = 1000000;
  const int D = (int)(deliveries.size());
  const int DC = std::accumulate(deliveries.cbegin(), deliveries.cend(), 0, [](int c, const Event& e){ return c + e.capacity; });
  // Description queueable:
  // magic index,         deliveries to go, cost, capacity, route, picked-up
  // [0,D-1] -> delivery
  // -1      -> depot 
  // D       -> pickup
  using queueable = std::tuple<int, std::set<int>, int,
                               int, std::vector<std::pair<int, int>>, bool>;
  std::queue<queueable> q;
  std::vector<int> v(D);
  std::ranges::iota(v, 0);
  for (int i = 0; i < D; i++) {
    std::set<int> togo(v.cbegin(), v.cend());
    (void)togo.erase(i);
    q.emplace(i, togo, std::abs(deliveries[i].x) + std::abs(deliveries[i].y),
              capacity-DC+deliveries[i].capacity,
              std::vector<std::pair<int, int>>({{deliveries[i].x, deliveries[i].y}}),
              false);
  }
  while(!q.empty()) {
    auto [i, togo, cost, cap, route, picked] = q.front();
    q.pop();
    if (togo.empty() and picked and i == -1 and best_cost > cost) {
      best_route = route;
      best_cost = cost;
    } else if (togo.empty() and picked and i != -1) {
      Event e = i < D ? deliveries[i] : pickup;
      route.push_back({0,0});
      q.emplace(-1, togo, cost + std::abs(e.x) + std::abs(e.y),
                capacity-pickup.capacity,
                route,
                picked);
    } else if (togo.empty() and not picked) {
      route.push_back({pickup.x, pickup.y});
      q.emplace(D, togo, cost + std::abs(pickup.x) + std::abs(pickup.y),
                capacity-pickup.capacity,
                route,
                not picked);
    } else if (not togo.empty()) {
      for (const int j : togo) {
        auto ntogo = togo;
        auto nroute = route;
        nroute.push_back({deliveries[j].x, deliveries[j].y});
        (void)ntogo.erase(j);
        q.emplace(j, ntogo,
                  cost + std::abs(deliveries[j].x-deliveries[i].x) + std::abs(deliveries[j].y-deliveries[i].y),
                  cap+deliveries[j].capacity,
                  nroute,
                  picked);
      }
      //TODO extension, handle picked in this branch, ATM it finds
      //a reasonably short route with the pickup at the end.
    }
  }
  return {best_route, best_cost};
}

