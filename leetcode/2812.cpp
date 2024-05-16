#include <bitset>
#include <queue>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::make_pair;
using std::pair;
using std::vector;

namespace solution {

int n;

const vector<pair<int, int>> deltas = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void transform(vector<vector<int>>& grid) {
    std::queue<pair<int, int>> Q;
    for (int r = 0; r < n; r++) for(int c = 0; c < n; c++)
        if (grid[r][c]) {
            grid[r][c] = 0;
            Q.push(make_pair(r, c));
        } else grid[r][c] = INT_MAX;
    while (!Q.empty()) {
        pair<int, int> s = Q.front();
        Q.pop();
        const int r = s.first; // FIXME structed binding returning from front?
        const int c = s.second;
        for (const pair<int, int>& delta : deltas) {
            const int rr = r + delta.first;
            const int cc = c + delta.second;
            if (rr < 0 or rr == n or cc < 0 or cc == n) continue;
            if (grid[rr][cc] > grid[r][c] + 1) {
                grid[rr][cc] = grid[r][c] + 1;
                Q.push(make_pair(rr, cc));
            }
        }
    }
}

using mask_t = std::bitset<400*400>;
bool f(const vector<vector<int>>& grid, int y) {
    std::queue<pair<int, int>> Q;
    mask_t Qed{};
    Q.push(make_pair(0, 0));
    Qed.set(0);
    while (!Q.empty()) {
        pair<int, int> s = Q.front();
        Q.pop();
        const int r = get<0>(s);
        const int c = get<1>(s);
        if (r == n - 1 and c == n - 1) return true;
        for (const pair<int, int>& delta : deltas) {
            const int rr = r + delta.first;
            const int cc = c + delta.second;
            if (rr < 0 or rr == n or cc < 0 or cc == n) continue;
            if (grid[rr][cc] < y or Qed.test(rr*n + cc)) continue;
            Q.push(make_pair(rr, cc));
            Qed.set(rr*n + cc);
        }
    }
    return false;
}

int solve(vector<vector<int>>& grid) {
    n = static_cast<int>(grid.size());

    // transform input grid to the minimum distance to any thief
    transform(grid);

    // minimum distance from init or goal to any thief:
    const int x = std::min(grid[0][0], grid[n - 1][n - 1]);

    // binary search, but starting the first iteration with the upper bound:
    pair<int, int> interval{0, x};
    int y = x;
    while (y > 0) {
        if (!f(grid, y))  interval.second = y - 1;
        else              interval.first = y;
        y = (interval.second + interval.first + 1)/2;
        if (interval.second <= interval.first) return interval.first;
    }
    return y;
}

}  // namespace solution


TEST(FindTheSafestPathInAGrid, i) {
    vector<vector<int>> grid = {{1, 0, 0}, {0, 0, 0}, {0, 0, 1}};
    EXPECT_EQ(solution::solve(grid), 0);
}

TEST(FindTheSafestPathInAGrid, ii) {
    vector<vector<int>> grid = {{0, 0, 1}, {0, 0, 0}, {0, 0, 0}};
    EXPECT_EQ(solution::solve(grid), 2);
}

TEST(FindTheSafestPathInAGrid, iii) {
    vector<vector<int>> grid = {{0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}};
    EXPECT_EQ(solution::solve(grid), 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
