#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <string_view>

constexpr int score(std::string_view s)
{
    std::array<int, 1024> v;
    std::adjacent_difference(s.cbegin(), s.cend(), v.begin());
    return std::accumulate(std::next(v.cbegin()),
                           v.cbegin() + s.length(),
                           0,
                           [](int acc, const int val) {
                               return acc += std::abs(val);
                           });
}

static_assert(score("hello") == 13);
static_assert(score("platypus") == 55);
static_assert(score("honorificabilitudinitatibus") == 181);

int main(){}
