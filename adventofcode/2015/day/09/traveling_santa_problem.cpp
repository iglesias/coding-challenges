// TODO

#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

auto getin() -> void {
  using std::operator""sv;
  constexpr auto words{"London to Dublin = 464"sv};

  for (const auto word : std::views::split(words, " to "sv))
#if GCC_VERSION > 120000
      // with string_view's C++23 range constructor:
      std::cout << std::quoted(std::string_view(word)) << ' ';
#endif
  std::cout << '\n';

  auto r = std::views::split(words, " to "sv);
  //compilation error
  //std::cout << r[0] << std::endl;
}

auto main() -> int {
  getin();
}
