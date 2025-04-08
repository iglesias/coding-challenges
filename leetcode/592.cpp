#include <cstdlib>
#include <numeric>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::make_pair;
using std::pair;
using std::string_view;

struct fraction {
    constexpr fraction() : den(0), num(0), neg(false) {}
    int den;
    int num;
    bool neg;
};

constexpr auto parse_number(string_view expr, const size_t i) -> pair<int, size_t> {
    if (expr[i] == '1')
        return i < expr.length() - 1 and expr[i + 1] == '0' ? make_pair(10, i + 2) : make_pair( 1, i + 1);
    else
        return {expr[i] - '0', i + 1};
}

constexpr auto parse_fraction(string_view expression, const size_t i = 0) -> pair<fraction, size_t> {
    fraction f;
    size_t j = i;
    if (expression[j] == '+' or expression[j] == '-') {
        f.neg = expression[j] == '-';
        j++;
    } 
    std::tie(f.num, j) = parse_number(expression, j);
    j++;
    std::tie(f.den, j) = parse_number(expression, j);
    return {f, j};
}

constexpr auto reduce(const std::vector<fraction>& fs) -> fraction {
    fraction result;
    int prod = 1;
    for (const fraction& f : fs)
        prod *= f.den;
    result.den = prod;
    for (const fraction& f : fs)
        result.num += (f.neg ? -1 : 1) * f.num * (prod / f.den);
    const int gcd = std::gcd(result.num, result.den);
    result.num /= gcd;
    result.den /= gcd;
    if (result.num < 0) {
        result.num *= -1;
        result.neg = true;
    }
    return result;
}

constexpr auto fraction_addition(std::string expression) -> std::string {
    std::vector<fraction> fractions;
    auto [f, i] = parse_fraction(expression);
    fractions.push_back(f);
    while (i < expression.length()) {
        std::tie(f, i) = parse_fraction(expression, i);
        fractions.push_back(f);
    }
    const fraction result = reduce(fractions);
    return (result.neg ? "-" : "") + std::to_string(result.num) + "/" + std::to_string(result.den);
}

// try with <charconv>'s to_chars instead
//using namespace std::literals;
//static_assert(fraction_addition("-1/2+1/2") == "0/1");


TEST(FractionAddition, a) { EXPECT_EQ(fraction_addition("-1/2+1/2"),     "0/1"); }
TEST(FractionAddition, b) { EXPECT_EQ(fraction_addition("-1/2+1/2+1/3"), "1/3"); }
TEST(FractionAddition, c) { EXPECT_EQ(fraction_addition(" 1/3-1/2"),    "-1/6"); }

TEST(FractionAddition, d__expression_with_a_10) { EXPECT_EQ(fraction_addition("-5/2+10/3+7/9"), "29/18"); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
