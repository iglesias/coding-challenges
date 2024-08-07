#include <array>
#include <iostream>
#include <string>

constexpr std::array digits_english_words{"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
constexpr std::array teens_english_words{"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
constexpr std::array tens_english_words{"Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

int count_digits(int n) {
    int num_digits = 0;
    do {
        num_digits++;
        n /= 10;
    } while (n > 0);
    return num_digits;
}

int pop_most_significant_digit(int& n) {
    int m = n;
    int base = 1;
    while (m >= 10) {
        m /= 10;
        base *= 10;
    }
    n -= m * base;
    return m;
}

using namespace std::string_literals;

std::string convert_two_digits(int n) {
    if (n < 20) return teens_english_words.at(n - 10);
    const int d = pop_most_significant_digit(n);
    return tens_english_words.at(d - 2) + ((n % 10) ? " "s + digits_english_words.at(n) : ""s);
}

std::string f(int n) {
    const auto g = [&n](const std::string& s) {
        const int n0 = count_digits(n);
        const int d = pop_most_significant_digit(n);
        const int n1 = count_digits(n);
        const std::string maybe_more = ((s == "Hundred"s and n0 - n1 > 2) ? (n0 < 9 ? " Thousand"s : " Million"s): ""s);
        return digits_english_words.at(d) + " "s + s + maybe_more + (n ? " "s + f(n) : ""s);
    };
    const auto h = [&n](const std::string& s) {
        const int n0 = count_digits(n);
        const int d = pop_most_significant_digit(n);
        const int n1 = count_digits(n);
        if (n0 - n1 >= 2)
            return convert_two_digits(d * 10) + " "s + s + (n ? " "s + f(n) : ""s);
        const int e = pop_most_significant_digit(n);
        return convert_two_digits(d * 10 + e) + " "s + s + (n ? " "s + f(n) : ""s);
    };
    switch (count_digits(n)) {
        case 1: return digits_english_words.at(n);
        case 2: return convert_two_digits(n);
        case 3: return g("Hundred"s);
        case 4: return g("Thousand"s);
        case 5: return h("Thousand"s);
        case 6: return g("Hundred"s);
        case 7: return g("Million"s);
        case 8: return h("Million"s);
        case 9: return g("Hundred"s);
        default: return g("Billion"s);
    }
}

std::string number_to_english_words(int num) {
    return f(num);
}

int main() {
    std::cout << number_to_english_words(10001) << std::endl;
}
