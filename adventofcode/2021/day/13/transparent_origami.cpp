#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

struct dot { int x, y; };

void print(const vector<vector<char>>& grid) {
    for (const vector<char>& row : grid) {
        for (char c : row) std::cout << c;
        std::cout << '\n';
    }
}

void fold_up(vector<vector<char>>& grid, int y);
void fold_left(vector<vector<char>>& grid, int x);

int main() {
    vector<dot> dots;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            int max_x{0}, max_y{0};
            for (const dot& d : dots) {
                if (d.x > max_x) max_x = d.x;
                if (d.y > max_y) max_y = d.y;
            }
            vector<vector<char>> grid(max_y + 1, vector<char>(max_x + 1, '.'));
            for (const dot& d : dots) grid[d.y][d.x] = '#';
            while (std::getline(std::cin, line)) {
                const auto pos = line.find("=");
                const char along_x_or_y = line.at(pos - 1);
                const int x_or_y = std::stoi(line.substr(pos + 1));
                if (along_x_or_y == 'x') fold_left(grid, x_or_y);
                else fold_up(grid, x_or_y);
            }
            print(grid);
            return 0;
        }
        const auto pos = line.find(",");
        const int x = std::stoi(line.substr(0, pos));
        const int y = std::stoi(line.substr(pos + 1));
        dots.emplace_back(x, y);
    }
}

void fold_up(vector<vector<char>>& grid, int y) {
    for (int i = 1; y + i < static_cast<int>(grid.size()); i++)
        for (size_t j = 0; j < grid[y + i].size(); j++)
            if (grid[y + i][j] == '#') grid[y - i][j] = '#';
    grid.erase(grid.begin() + y, grid.end());
}

void fold_left(vector<vector<char>>& grid, int x) {
    for (size_t i = 0; i < grid.size(); i++)
        for (int j = 1; x + j < static_cast<int>(grid[j].size()); j++)
            if (grid[i][x + j] == '#') grid[i][x - j] = '#';
    for (vector<char>& row : grid)
        row.erase(row.begin() + x, row.end());
}

