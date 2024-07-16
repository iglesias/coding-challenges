#include <iostream>
#include <set>
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

std::string part_two(std::multiset<std::string> set, bool opt);

int main() {
    std::string line;
    std::vector<size_t> ones_count;
    std::multiset<std::string> set;
    while (std::getline(std::cin, line)) {
        if (not ones_count.size())
            ones_count = std::vector<size_t>(line.length(), 0);
        size_t i{0};
        for (const char c : line) ones_count[i++] += c == '1';
        set.insert(line);
    }
    std::string gamma_rate, epsilon_rate;
    for (const size_t count : ones_count) {
        gamma_rate.append(  count > set.size() / 2 ? "1" : "0");
        epsilon_rate.append(count > set.size() / 2 ? "0" : "1");
    }
    std::cout << "Part one: " << bin2dec(gamma_rate) * bin2dec(epsilon_rate) << '\n';

    const int o2_generator_rating = bin2dec(part_two(set, false));
    const int co2_scrubber_rating = bin2dec(part_two(set, true));
    std::cout << "Part two: " << o2_generator_rating * co2_scrubber_rating << '\n';
}

std::string part_two(std::multiset<std::string> set, bool opt) {
    for (size_t i{0}; set.size() > 1; ++i) {
        size_t ones_count{0};
        for (const std::string& item : set) ones_count += item.at(i) == '1';
        const size_t n = set.size();
        if (not n % 2 and ones_count == n / 2) ones_count = n;
        auto it = set.begin();
        while (it != set.end())
            if (ones_count > n / 2) {
                if (it->at(i) == "01"[opt]) it = set.erase(it);
                else                        it++;
            } else {
                if (it->at(i) == "10"[opt]) it = set.erase(it);
                else                        it++;
            }
    }
    return *set.begin();
}
