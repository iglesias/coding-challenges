#include <algorithm>
#include <bitset>
#include <queue>
#include <cassert>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using std::vector;

int snakes_and_ladders(vector<vector<int>>& board) {
    std::bitset<20*20 + 1> Qed;
    std::queue<std::pair<int, int>> Q;
    Q.emplace(1, 0);
    int const n = static_cast<int>(board.size());
    while (!Q.empty()) {
        auto const [square, num_rolls] = Q.front();
        Q.pop();
        if (square == n*n) return num_rolls;
        for (int i = square + 1; i <= std::min(n*n, square + 6); i++) {
            int const row = n - 1 - (i - 1) / n;
            // Reverse the column order based n's and row's parities.
            bool const reverse = (n % 2 == 0) ? (row % 2 == 0) : (row % 2 == 1);
            int const col = reverse ? n - 1 - (i - 1) % n : (i - 1) % n;
            assert(row >= 0 and row < n);
            assert(col >= 0 and col < n);
            int const next = board[row][col] == -1 ? i : board[row][col];
            if (Qed.test(next)) continue;
            Qed.set(next);
            Q.emplace(next, num_rolls + 1);
        }
    }
    return -1;
}

TEST(SnakesAndLadders, Example1) {
    vector<vector<int>> board = {
        {-1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1},
        {-1, 35, -1, -1, 13, -1},
        {-1, -1, -1, -1, -1, -1},
        {-1, 15, -1, -1, -1, -1}
    };
    EXPECT_EQ(snakes_and_ladders(board), 4);
}

TEST(SnakesAndLadders, Example2) {
    vector<vector<int>> board = {{-1, -1}, {-1, 3}};
    EXPECT_EQ(snakes_and_ladders(board), 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
