class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int const numRows = static_cast<int>(grid.size());
        int const numCols = static_cast<int>(grid[0].size());
        int ans{0};
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                if (grid[row][col]) {
                    // |
                    if (col == 0) ans++;
                    else if (!grid[row][col - 1]) ans++;
                    // 
                    if (col == numCols - 1) ans++;
                    else if (!grid[row][col + 1]) ans++;
                    // 
                    if (row == 0) ans++;
                    else if (!grid[row - 1][col]) ans++;
                    // 
                    if (row == numRows - 1) ans++;
                    else if (!grid[row + 1][col]) ans++;
                }
            }
        }
        return ans;
    }
};

int main() {}
