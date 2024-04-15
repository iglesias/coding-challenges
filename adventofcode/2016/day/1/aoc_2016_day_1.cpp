#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

using std::operator""sv;

auto getinput() -> std::vector<std::pair<char, int>> {
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::pair<char, int>> tokens;
    for (auto const token : std::views::split(input, ", "sv)) {
        char const letter{token[0]};
#if GCC_VERSION > 120000
        int const digits{std::stoi(std::string(std::string_view(token | std::views::drop(1))))};
#endif
        int const digits{0};
        tokens.emplace_back(letter, digits);
    }
    return tokens;
}

int solve(auto const& tokens) {
    int x{0}, y{0};
    int dx{0}, dy{1};
    for (auto const& [letter, digits] : tokens) {
        std::swap(dx, dy);
        if (letter == 'R') {
            dx = -dx;
        } else {
            dy = -dy;
        }
        x += dx * digits;
        y += dy * digits;
    }
    return std::abs(x) + std::abs(y);
}

int main() {
    std::cout << solve(getinput()) << '\n';
}
