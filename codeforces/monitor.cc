// https://codeforces.com/problemset/problem/16/C
// Prime factorization and keeping track of a fraction of
// integers in its simplified form.
// $ g++ -std=c++17 -Wall -Wextra -o monitor monitor.cc

#include <bits/stdc++.h>

using namespace std;

vector<int> prime_factors(int n)
{
  vector<int> ans;

  while (n%2 == 0)
  {
    ans.push_back(2);
    n /= 2;
  }

  for (int i = 3; i <= sqrt(n); i += 2)
  {
    while (n%i == 0)
    {
      ans.push_back(i);
      n /= i;
    }
  }

  if (n > 2)
    ans.push_back(n);

  return ans;
}

int main()
{
  uint64_t a, b, x, y;
  cin >> a >> b >> x >> y;

  uint64_t best_a{0}, best_b{0};

  vector<int> x_factors = prime_factors(x);
  vector<int> y_factors = prime_factors(y);

  vector<int> x_simplified_factors;
  set_difference(x_factors.begin(), x_factors.end(),
                 y_factors.begin(), y_factors.end(),
                 back_inserter(x_simplified_factors));

  vector<int> y_simplified_factors;
  set_difference(y_factors.begin(), y_factors.end(),
                 x_factors.begin(), x_factors.end(),
                 back_inserter(y_simplified_factors));

  int x_simplified = 1;
  for (int factor : x_simplified_factors) x_simplified *= factor;

  int y_simplified = 1;
  for (int factor : y_simplified_factors) y_simplified *= factor;

  uint64_t try_multiple = 1;
  for (;;)
  {
    uint64_t try_a = try_multiple * x_simplified;
    if (try_a > a) break;

    uint64_t try_b = try_multiple * y_simplified;
    if (try_b > b) break;

    if (try_a * try_b < best_a * best_b) continue;

    best_a = try_a;
    best_b = try_b;

    try_multiple++;
  }

  cout << best_a << ' ' << best_b << '\n';
}
