#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

//using positive = uint8_t;
//using positive = uint16_t;
//using positive = uint32_t;
//using positive = uint64_t;
using positive = __int128;
//
//129
//32769
//2147483649
//9223372036854775809
//85070591730234615865843651857942052865

std::string to_binary(positive n)
{
  using namespace std::string_literals;
  if(n == 0) return "0"s;
  std::string b;
  while(n > 0) {
    b.insert(0, 1, n%2+'0');
    n = n/2;
  }
  return b;
}

auto find_all(std::string_view b, char c) -> std::vector<std::string::size_type>
{
  std::vector<std::string::size_type> pos;
  std::string::size_type n = 0;
  for(;;){
    n = b.find(c, n);
    if(n == std::string::npos) break;
    pos.push_back(n);
    n++;
  }
  return pos;
}

//https://stackoverflow.com/a/59914665/18893304
std::ostream& operator<<(std::ostream& o, const __int128& x) {
    if (x == std::numeric_limits<__int128>::min()) return o << "-170141183460469231731687303715884105728";
    if (x < 0) return o << "-" << -x;
    if (x < 10) return o << (char)(x + '0');
    return o << x / 10 << (char)(x % 10 + '0');
}

template<typename T>
void print(positive n, std::string_view b, std::vector<T> const& arr, positive ans)
{
  std::cout << ">> " << n << "\n";
  std::cout << b << "\n";
  for(const auto item : arr) std::cout << item << " ";
  std::cout << "\n";
  std::cout << "<< " << ans << "\n";
}

void sample()
{
  const positive n = 1041;
  const auto b = to_binary(n);
  auto arr = find_all(b, '1');
  std::string::size_type longest_gap = 0;
  if(arr.size() > 1) {
    std::adjacent_difference(arr.cbegin(), arr.cend(), arr.begin());
    for(auto i = std::next(arr.cbegin()); i != arr.cend(); i++)
      longest_gap = std::max(longest_gap , *i-1);
  }
  print(n, b, arr, longest_gap);
}

int main()
{
  positive max = 0;
  for(uint8_t e = 1; e <= 127; e++){
    const positive p = (positive(1)<<e) + 1;
    const auto b = to_binary(p);
    auto arr = find_all(b, '1');
    if(arr.size() <= 1) continue;
    std::adjacent_difference(arr.cbegin(), arr.cend(), arr.begin());
    std::string::size_type longest_gap = 0;
    for(auto i = std::next(arr.cbegin()); i != arr.cend(); i++)
      longest_gap = std::max(longest_gap , *i-1);
    if(longest_gap > max){
      print(p, b, arr, longest_gap);
      max = longest_gap;
    }
  }
}

