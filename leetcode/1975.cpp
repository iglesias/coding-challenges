#include <algorithm>
#include <climits>
#include <cstdlib>
#include <vector>

using std::vector;

long long maxMatrixSum(vector<vector<int>> const& matrix) {
    bool negative_parity = false;
    long long one_norm = 0;
    int infinity_norm = INT_MAX;
    for (size_t i = 0; i < matrix.size(); i++) for (size_t j = 0; j < matrix.size(); j++) {
        negative_parity = matrix[i][j] < 0 ? !negative_parity : negative_parity;
        one_norm += abs(matrix[i][j]);
        infinity_norm = std::min(infinity_norm, abs(matrix[i][j]));
    }
    return negative_parity ? one_norm - 2*infinity_norm : one_norm;
}

int main () {}
