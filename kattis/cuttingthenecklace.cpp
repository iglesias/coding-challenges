#include <cassert>
#include <iostream>
#include <vector>

using pos = unsigned long long;

pos k, n;

bool solve(const std::vector<pos>& A, pos i, pos r)
{
  pos b{0};
  if (i > n-1) b = i - (n-1);

  while (i >= b)
  {
    pos cur{A[i--]};
    while (cur < r && i >= b)
    {
      cur += A[i--];
    }
    
    if (cur == r)
    {
      cur = 0;
    }
    else
    {
      assert(cur > r);
      return false;
    }
  }

  return true;
}

int main()
{
    std::cin >> k >> n;
    
    std::vector<pos> A(n);
    pos max{0}, total{0};
    for (pos i{0}; i<n; i++)
    {
        pos weight;
        std::cin >> weight;
        max = std::max(max, weight);
        total += weight;
        A[i] = weight;
    }
    
    pos r{total/k};
    // Each split segment must add up to r, if possible,
    // so that each of the k friends gets the same amount.
    if (total%k != 0 || max > r)
    {
        std::cout << "NO\n";
        return 0;
    }

    for (pos i{0}; i<r-1; i++)
    {
      A.push_back(A[i]);
    }

    for (pos i{n+r-2}; i>=n-1; i--)
    {
      if (solve(A, i, r))
      {
        std::cout << "YES\n";
        return 0;
      }
    }

    std::cout << "NO\n";
}
