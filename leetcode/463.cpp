#include <vector>

#include <gtest/gtest.h>

int islandPerimeter(std::vector<std::vector<int>> &grid) {
  int const numRows = static_cast<int>(grid.size());
  int const numCols = static_cast<int>(grid[0].size());
  int ans{0};
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (grid[row][col]) {
        // |
        if (col == 0)
          ans++;
        else if (!grid[row][col - 1])
          ans++;
        //
        if (col == numCols - 1)
          ans++;
        else if (!grid[row][col + 1])
          ans++;
        //
        if (row == 0)
          ans++;
        else if (!grid[row - 1][col])
          ans++;
        //
        if (row == numRows - 1)
          ans++;
        else if (!grid[row + 1][col])
          ans++;
      }
    }
  }
  return ans;
}

TEST(islandPerimeter, singleOneCell) {
  std::vector<std::vector<int>> grid = {{1}};
  ASSERT_EQ(islandPerimeter(grid), 4);
}

TEST(islandPerimeter, row) {
  std::vector<std::vector<int>> grid = {{1, 0}};
  ASSERT_EQ(islandPerimeter(grid), 4);
}

TEST(islandPerimeter, column) {
  std::vector<std::vector<int>> grid = {{1}, {0}};
  ASSERT_EQ(islandPerimeter(grid), 4);
}

TEST(islandPerimeter, twoByTwoOnes) {
  std::vector<std::vector<int>> grid = {{1, 1}, {1, 1}};
  ASSERT_EQ(islandPerimeter(grid), 8);
}

TEST(islandPerimeter, twoByTwoLowerRight) {
  std::vector<std::vector<int>> grid = {{1, 0}, {1, 1}};
  ASSERT_EQ(islandPerimeter(grid), 8);
}

TEST(islandPerimeter, fourByFour) {
  std::vector<std::vector<int>> grid = {
      {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}};
  ASSERT_EQ(islandPerimeter(grid), 16);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
