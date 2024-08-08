#include <map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::map;
using std::pair;
using std::vector;

const map<char, char> next{{'E', 'S'}, {'S', 'W'}, {'W', 'N'}, {'N', 'E'}};

const map<char, pair<int, int>> deltas{{'E', {0, +1}}, {'S', {+1, 0}}, {'W', {0, -1}}, {'N', {-1, 0}}};

vector<vector<int>> spiral_matrix_iii(int rows, int cols, int rStart, int cStart) {
    vector<vector<int>> ans{{rStart, cStart}};
    int r = rStart, c = cStart;
    int step = 1, i = 0;
    char dir = 'E';
    while (ans.size() < static_cast<uint64_t>(rows) * cols) {
        const auto& [dr, dc]  = deltas.at(dir);
        for (int i = 0; i < step; i++) {
            r += dr;
            c += dc;
            if (0 <= r and r < rows and 0 <= c and c < cols) ans.push_back({r, c}); }
        i++;
        if (!(i % 2)) step++;
        dir = next.at(dir); }
    return ans; }

TEST(spiral_matrix_iii, oversimple) {
    EXPECT_EQ((vector<vector<int>>{{0, 0}, {0, 1}, {0, 2}, {0, 3}}), spiral_matrix_iii(1, 4, 0, 0)); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); }
