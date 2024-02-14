//g++ -g -std=c++23 -Wall -Wextra -pedantic -Wconversion -fsanitize=undefined,address

#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

auto getin() -> void {
  using std::operator""sv;
  constexpr auto words{"London to Dublin = 464"sv};

  for (const auto word : std::views::split(words, " to "sv))
      // with string_view's C++23 range constructor:
      std::cout << std::quoted(std::string_view(word)) << ' ';
  std::cout << '\n';

  auto r = std::views::split(words, " to "sv);
  //compilation error
  //std::cout << r[0] << std::endl;
}

auto main() -> int {
  getin();
}
