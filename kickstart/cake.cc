#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

using ull = unsigned long long;

vector<vector<ull>> dp;

void presolve(const int N)
{
  for (int n = 1; n <= N; n++) dp[1][n] = n;

  for (int l = 2; l*l <= N; l++) {
    for (int n = 1; n <= N; n++) {
      if (n >= l*l) dp[l][n] = min(dp[l-1][n], dp[l][n-l*l]+1);
      else          dp[l][n] = dp[l-1][n];
    }
  }
}


int main()
{
  dp = vector<vector<ull>>(100 + 1, vector<ull>(10000 + 1, 0));
  presolve(10000);

  int T, N;
  cin >> T;
  for (int t = 0; t < T; t++) {
    cin >> N;
    cout << "Case #" << t+1 << ": " << dp[100][N] << endl;
  }
}
