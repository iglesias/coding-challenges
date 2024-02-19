#define FMT_HEADER_ONLY

#include <iterator>
#include <random>
#include <ranges>
#include <set>
#include <vector>

#include <fmt/core.h> 
#include <fmt/ranges.h>

auto main(int argc, char* argv[]) -> int {
  int N = 3;
  if (argc > 1) {
    //TODO handle exception on bad input.
    N = std::stoi(argv[1]);
  }

  std::vector<int> v(2*N+1);
  std::ranges::iota(v, -N);

  std::vector<int> w((2*N+1)*(2*N+1));
  std::set<int> idxs;
  std::ranges::iota(w, 0);
  int const D = 7, P = 4, Q = 15; // FIXME randomize.
  std::ranges::sample(w, std::inserter(idxs, idxs.end()), D+P,
                      std::mt19937 {std::random_device{}()});
  // FIXME consider adding shuffle so that pickups are everywhere.
  fmt::println("{} {} {}", D, P, Q);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> capacity_distribution(1, 3); // FIXME magic numbers
  for (int i{0}; auto const& item : std::views::cartesian_product(v, v)) {
    int const q = capacity_distribution(gen);
    if (idxs.contains(i++)) fmt::println("{} {}", item, q);
  }

  return 0;
}
