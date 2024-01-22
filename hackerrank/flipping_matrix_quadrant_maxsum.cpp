#include <algorithm>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>

#include <fmt/ranges.h>

using matrix_t = std::vector<std::vector<int>>;

matrix_t matrix;

int f()
{
    static std::unordered_set<matrix_t, boost::hash<matrix_t>> seen;
    static int ans = 0;

    //fmt::print("{}\n", matrix);
    //fmt::print("{}\n", seen.size());
    //std::cout << std::flush;

    if(seen.contains(matrix)) return ans;

    const int n = static_cast<int>(matrix.size())/2;
    int sum = 0;
    for(int i = 0; i<n; i++) for(int j = 0; j<n; j++) sum += matrix[i][j];
    ans = std::max(ans, sum);

    seen.insert(matrix);

    for (int r = 0; r<2*n; r++) {
        std::reverse(matrix[r].begin(), matrix[r].end());
        if(!seen.contains(matrix)) f();
        std::reverse(matrix[r].begin(), matrix[r].end());
    }

    for (int c = 0; c<2*n; c++) {
        for (int r = 0; r<n; r++) std::swap(matrix[r][c], matrix[2*n-1-r][c]);
        if(!seen.contains(matrix)) f();
        for (int r = 0; r<n; r++) std::swap(matrix[r][c], matrix[2*n-1-r][c]);
    }

    return ans;
}

int g()
{
    int ans = 0;
    const int n = static_cast<int>(matrix.size())/2;
    for(int i = 0; i<n; i++) for(int j = 0; j<n; j++) {
      using std::max;
      ans += max(max(max(matrix[2*n-1-i][2*n-1-j], matrix[i][2*n-1-j]), matrix[2*n-1-i][j]), matrix[i][j]);
    }
    return ans;
}

int main(int argc, char* argv[])
{
    int n = 2;
    if(argc > 1) { n = std::stoi(argv[1]); }
    matrix = matrix_t(2*n, std::vector<int>(2*n, 1));
    {
      int next = 0;
      for(int i=0; i<2*n; i++) for(int j=0; j<2*n; j++) matrix[i][j] = next++;
    }
    fmt::print("{} {}\n", f(), g());
}
