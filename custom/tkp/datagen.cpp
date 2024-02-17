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
  int const D = 15, P = 3;
  std::ranges::sample(w, std::inserter(idxs, idxs.end()), D+P,
                      std::mt19937 {std::random_device{}()});
  // FIXME consider adding shuffle so that pickups aren't forced to appear
  // on the right since the sampled indices are ordered and the pickups
  // are defined in the input after the deliveries.
  fmt::println("{} {}", D, P);
  for (int i{0}; auto const& item : std::views::cartesian_product(v, v)) {
    if (idxs.contains(i++)) fmt::println("{}", item);
  }

  return 0;
}
