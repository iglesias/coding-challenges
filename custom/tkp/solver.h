#include <bits/stdc++.h>

struct Event {
  int x, y;
  int capacity;
};

// Forward declaration.
std::pair<std::vector<std::pair<int, int>>, int> output_route(
    const std::vector<Event>& deliveries, const Event& pickup, int capacity);

std::pair<int, std::vector<std::pair<int, int>>> solve(const std::vector<Event>& deliveries,
    const std::vector<Event>& pickups, int capacity);
