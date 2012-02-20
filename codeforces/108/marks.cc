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

char gradebook[100][100];

int main()
{
  int n, m, ans = 0;
  char str[1];

  scanf("%d %d\n", &n, &m);

  FOR(i, 0, n)
  {
    FOR(j, 0, m)
      gradebook[i][j] = getchar();
    gets(str);
  }
  
  FOR(i, 0, n)
  {
    FOR(j, 0, m)
    {
      char mygrade = gradebook[i][j];
      int ii = 0;
      for ( ; ii < n ; ++ii )
      {
        if ( ii == i ) continue;
        if ( gradebook[ii][j] > mygrade )
          break;
      }
      if ( ii >= n )
      {
        ++ans;
        break;
      }
    }
  }

  printf("%d\n", ans);

  return 0;
}
