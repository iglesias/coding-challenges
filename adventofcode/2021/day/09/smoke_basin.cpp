#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

int main() {
    int R{0}, C;
    std::vector<std::string> v;
    std::string line;
    while (std::getline(std::cin, line)) {
        v.push_back(line);
        C = static_cast<int>(line.length());
        R++;
    }
    int part_one{0};
    constexpr std::array<std::array<int, 2>, 4> deltas = {-1, 0, 1, 0, 0, -1, 0, 1};
    std::vector<size_t> basins_sizes;
    for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) {
        bool found = false;
        for (const auto& delta : deltas)
            if (i + delta[0] >= 0 and i + delta[0] < R and  j + delta[1] >= 0 and j + delta[1] < C)
                if (v[i + delta[0]][j + delta[1]] - '0' <= v[i][j] - '0') found = true;
        if (found) continue;
        part_one += v[i][j] - '0' + 1;
        std::unordered_set<std::pair<int, int>, pair_hash> Qed;
        std::queue<std::pair<int, int>> Q;
        Q.emplace(i, j);
        Qed.emplace(i, j);
        while (not Q.empty()) {
            const auto [r, c] = Q.front();
            Q.pop();
            for (const auto& delta : deltas)
                if (r + delta[0] >= 0 and r + delta[0] < R and  c + delta[1] >= 0 and c + delta[1] < C)
                    if (v[r + delta[0]][c + delta[1]] != '9' and
                        not Qed.contains(std::make_pair(r + delta[0], c + delta[1]))) {
                        Q.emplace(r + delta[0], c + delta[1]);
                        Qed.emplace(r + delta[0], c + delta[1]);
                    }
        }
        basins_sizes.push_back(Qed.size());
    }
    std::cout << part_one << '\n';
    std::ranges::sort(basins_sizes);
    std::cout << basins_sizes.at(basins_sizes.size() - 1) *
                 basins_sizes.at(basins_sizes.size() - 2) *
                 basins_sizes.at(basins_sizes.size() - 3) << '\n'; 
}
