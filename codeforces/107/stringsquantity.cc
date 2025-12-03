#include <cstdio>
#include <iostream>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )

using namespace std;

int main()
{
  int n, m, k, exp;
  long long  ans = 1;

  scanf("%d %d %d\n", &n, &m, &k);

  if ( k == 1 || k > n )
  {
    FOR(i, 0, n)
      ans = (ans * m) % 1000000007;
  }
  else if ( k == n )
  {
    exp = (n + 1) / 2;
    FOR(i, 0, exp)
      ans = (ans * m) % 1000000007;
  }
  else if ( k % 2 )
    ans = m*m;
  else
    ans = m;

  cout << ans << endl;

  return 0;
}
