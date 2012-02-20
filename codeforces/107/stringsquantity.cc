// {{{ Boilerplate Code <--------------------------------------------------

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

using namespace std;

typedef pair< int, int >  ii;
typedef long long         ll;

// }}}

int main()
{
  int n, m, k, exp;
  ll  ans = 1;

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
