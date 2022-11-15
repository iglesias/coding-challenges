#include <bits/stdc++.h>

int main()
{
  int n, r, k;
  std::cin >> n >> r >> k;

  int ans{ k + std::abs(r-k) + 2*std::max(0, static_cast<int>(std::ceil((n-(k+std::abs(r-k)))/2.))) + r };
  std::cout << ans;
}
