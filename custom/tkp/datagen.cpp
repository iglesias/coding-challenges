#define FMT_HEADER_ONLY

#include <iterator>
#include <random>
#include <ranges>
#include <set>
#include <stdexcept>
#include <vector>

#include <fmt/core.h> 
#include <fmt/ranges.h>

auto main(int argc, char* argv[]) -> int {
  int N = 3;
  if (argc > 1) {
    try {
      N = std::abs(std::stoi(argv[1]));
    } catch (std::invalid_argument const&) {
      N = 3;
    }
  }

  std::vector<int> v(2*N+1);
  std::ranges::iota(v, -N);

  std::vector<int> w((2*N+1)*(2*N+1));
  std::set<int> idxs;
  std::ranges::iota(w, 0);
  int const D = 7, P = 4, Q = 9; // TODO randomize.

  if (D+P > (int)w.size()) {
    throw std::invalid_argument("Not enough space (f N) for the specified D and P.");
  }

  std::ranges::sample(w, std::inserter(idxs, idxs.end()), D+P,
                      std::mt19937 {std::random_device{}()});
  // TODO consider adding shuffle so that pickups are everywhere.
  fmt::println("{} {} {}", D, P, Q);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> capacity_distribution(1, 3); // TODO magic numbers
  for (int i{0}; auto const& item : std::views::cartesian_product(v, v)) {
    int const q = capacity_distribution(gen);
    if (idxs.contains(i++)) fmt::println("{} {}", item, q);
  }

  return 0;
}
