// g++ trebuchet.cpp -std=c++23 -fsanitize=address,pointer-overflow,signed-integer-overflow,undefined -Wall -Wextra -pedantic -O3

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std::literals;

namespace cc {

template <typename key, typename value, std::size_t size> struct array_map {
  std::array<std::pair<key, value>, size> data;

  [[nodiscard]] constexpr auto at(const key &k) const -> value {
    const auto it = std::ranges::find_if(
        data, [&k](const auto &item) { return item.first == k; });
    if (it != std::end(data)) {
      return it->second;
    } else {
      throw std::range_error("cc::array_map::at: key not found.");
    }
  }
};

} // namespace cc

static constexpr std::array<std::pair<std::string_view, char>, 9>
    digitsCharsArray{{{"one"sv, '1'},
                      {"two"sv, '2'},
                      {"three"sv, '3'},
                      {"four"sv, '4'},
                      {"five"sv, '5'},
                      {"six"sv, '6'},
                      {"seven"sv, '7'},
                      {"eight"sv, '8'},
                      {"nine"sv, '9'}}};

constexpr char strDigit2Char(std::string_view digit) {
  static constexpr auto array_map =
      cc::array_map<std::string_view, char, digitsCharsArray.size()>{
          {digitsCharsArray}};
  return array_map.at(digit);
}

// anonymous
namespace {

template <typename T1, typename T2, std::size_t N>
constexpr std::array<T1, N>
extract_firsts(std::array<std::pair<T1, T2>, N> in) {
  std::array<T1, N> out;
  for (std::size_t i{0}; i < N; i++)
    out[i] = in[i].first;
  return out;
}

}; // namespace

constexpr auto digits{::extract_firsts(digitsCharsArray)};

constexpr bool is_digit_prefix(std::string_view try_digit) {
  for (unsigned i{0}; i < digits.size(); i++) {
    unsigned j{0};
    while (j < try_digit.length() && digits[i][j] == try_digit[j])
      j++;
    if (j == try_digit.length())
      return true;
  }

  return false;
}

constexpr bool is_digit(std::string_view try_digit) {
  for (unsigned i{0}; i < digits.size(); i++)
    if (try_digit == digits[i])
      return true;

  return false;
}

// parser and converter from spelled-out digits
constexpr std::string transform(std::string_view in) {
  std::string out, try_digit;
  int start_try_digit{};
  unsigned i{0};
  while (i < in.length()) {
    if (is_digit_prefix(try_digit + in[i])) {
      if (try_digit.length() == 0)
        start_try_digit = i;

      try_digit += in[i];
      if (is_digit(try_digit)) {
        out += strDigit2Char(try_digit);
        try_digit = "\0";
      } else // without else branch for considering non-overlapping.
      {
        i++;
      }
    } else {
      if (try_digit.length() > 0) {
        out += in[start_try_digit];
        i = start_try_digit + 1;
        try_digit = "\0";
      } else {
        out += in[i];
        i++;
      }
    }
  }

  return out;
}
static_assert(transform("one"sv) == "1");
static_assert(transform("onex"sv) == "1ex");
static_assert(transform("nineightwone"sv) == "9821");

constexpr int find_first(std::string_view in, bool reverse = false) {
  int const start = reverse ? static_cast<int>(in.length()) - 1 : 0;
  int const delta = reverse ? -1 : +1;
  int const end = reverse ? -1 : static_cast<int>(in.length());
  int i{start};
  do {
    if (in[i] >= '0' && in[i] <= '9')
      return in[i] - '0';
    i += delta;
  } while (i != end);

  return -1;
}
static_assert(find_first("abc3"sv) == 3);
static_assert(find_first("abc3"sv, true) == 3);
static_assert(find_first("abc"sv) == -1);
static_assert(find_first("l10O"sv) == 1);
static_assert(find_first("l10O"sv, true) == 0);

constexpr std::pair<int, int> find_first_and_last_digits(std::string_view in) {
  return std::make_pair(find_first(in), find_first(in, true));
}

int main() {
  std::string line;
  int ans_a{0}, ans_b{0};
  for (;;) {
    std::getline(std::cin, line);
    if (line.empty())
      break;

    auto a{find_first_and_last_digits(line)};
    auto b{find_first_and_last_digits(transform(line))};

    ans_a += a.first * 10 + a.second;
    ans_b += b.first * 10 + b.second;
  }

  std::cout << "Part one: " << ans_a << '\n';
  std::cout << "Part two: " << ans_b << '\n';
}
