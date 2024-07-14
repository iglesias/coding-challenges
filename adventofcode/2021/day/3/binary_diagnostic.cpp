#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int bin2dec(std::string_view bin) {
    int base{1};
    int ans{0};
    for (ssize_t i = bin.size() - 1; i >= 0; i--) {
        ans += (bin[i] - '0')*base;
        base *= 2;
    }
    return ans;
}

int main() {
    size_t num_numbers{0};
    std::string line;
    std::vector<size_t> ones_count;
    while (std::getline(std::cin, line)) {
        num_numbers++;
        if (!ones_count.size())
            ones_count = std::vector<size_t>(line.length(), 0);
        size_t i{0};
        for (const char c : line) ones_count[i++] += c == '1';
    }
    std::string gamma_rate, epsilon_rate;
    for (const size_t count : ones_count) {
        gamma_rate.append(  count > num_numbers / 2 ? "1" : "0");
        epsilon_rate.append(count > num_numbers / 2 ? "0" : "1");
    }
    std::cout << "Part one: " << bin2dec(gamma_rate) * bin2dec(epsilon_rate) << '\n';
}
