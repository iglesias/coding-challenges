#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define   FOR(k, a, b)    for ( typeof(a) k = (a) ; k < (b) ; ++k )

using namespace std;

typedef pair< int, int> ii;

typedef long long ll;

#define   MAX   20000

bool rows[MAX+1], cols[MAX+1], diags1[2*MAX], diags2[2*MAX];  // 1-indices

int ans = 0, X, Y, N, xk, yk;

int diag1(int x, int y)
{
  return x + y - 1;
}

int diag2(int x, int y)
{
  return x + (Y - y);
}

int main()
{

  for ( ; ; )
  {
  
    ans = 0;

    scanf("%d %d %d\n", &X, &Y, &N);
    if ( X == 0 && Y == 0 && N == 0 )
      break;

    FOR(i, 1, X+1)  rows[i]   = false;
    FOR(i, 1, Y+1)  cols[i]   = false;
    FOR(i, 1, X+Y)  diags1[i] = false;
    FOR(i, 1, X+Y)  diags2[i] = false;

    FOR(i, 0, N)
    {
      scanf("%d %d\n", &xk, &yk);
      rows[xk] = true;
      cols[yk] = true;
      diags1[ diag1(xk, yk) ] = true;
      diags2[ diag2(xk, yk) ] = true;
    }

    FOR(i, 1, X+1)
    {
      if ( rows[i] ) continue;
      FOR(j, 1, Y+1)
      {
        if ( cols[j] ) continue;
        if ( diags1[ diag1(i, j) ] ) continue;
        if ( diags2[ diag2(i, j) ] ) continue;

        ++ans;
      }
    }

    printf("%d\n", ans);

  }

  return 0;
}
