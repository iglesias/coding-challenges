#include <chrono>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using std::string;
using std::vector;

int step(vector<string>& levels);

void print(const vector<string>& levels) {
    for (const string& line : levels) {
        for (const char c : line)
            if (c == '0') std::cout << "\033[1m" << c << "\033[0m";
            else          std::cout << c;
        std::cout << '\n';
    }
}

int main() {
    vector<string> levels;
    string line;
    while (std::getline(std::cin, line)) levels.push_back(std::move(line));
    const int num_steps = 1000000;
    int num_flashes{0};
    for (int s = 0; /* part two true or */ s < num_steps; s++) {
        //const int old_num_flashes = num_flashes;
        num_flashes += step(levels);
        /* part two
        if (num_flashes - old_num_flashes == 100) {
            std::cout << s << std::endl;
            break;
        }
        */
        /*
        if (!((s + 1) % 10)) {
            std::cout << "After step " << s + 1 << ":\n";
            print(levels);
        }
        */
        if (s > 0) {
            std::cout << "\033[" << levels.size() << "A\033[" << levels.at(0).length() << "D";
            print(levels);
        } else {
            print(levels);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << num_flashes << '\n';
}

int step(vector<string>& levels) {
    const int R = static_cast<int>(levels.size());
    const int C = static_cast<int>(levels.at(0).length());
    std::queue<std::pair<int, int>> Q;
    for (int r = 0; r < R; r++) for (int c = 0; c < C; c++)
        Q.emplace(r, c);
    int num_flashes = 0;
    while (!Q.empty()) {
        const auto [r, c] = Q.front();
        Q.pop();
        if (levels[r][c] == '9') {
            num_flashes++;
            for (int dr : std::array{-1, 0, 1})
                for (int dc : std::array{-1, 0, 1}) {
                    if (!dr and !dc) continue;
                    if (0 <= r + dr and r + dr < R and
                        0 <= c + dc and c + dc < C)
                        Q.emplace(r + dr, c + dc);
                }
        }
        levels[r][c]++;
    }
    for (int r = 0; r < R; r++) for (int c = 0; c < C; c++)
        if (levels[r][c] > '9') levels[r][c] = '0';
    return num_flashes;
}
