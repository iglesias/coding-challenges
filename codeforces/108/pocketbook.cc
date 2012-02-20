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

bool USED[26];

int main()
{
  int n, m;

  scanf("%d %d\n", &n, &m);         // n = number of names, m = length of each name

  vector< int >     ms(m, 0);       // Different letters on each position
  vector< string >  names(n, "");

  FOR(i, 0, n)
    cin >> names[i];

  FOR(i, 0, m)
  {
    FOR(j, 0, 26)  USED[j] = false;

    FOR(j, 0, n)
      if ( !USED[ names[j][i] - 'A' ] )
      {
        ++ms[i];
        USED[ names[j][i] - 'A' ] = true;
      }
  }

  ll ans = 1;
  FOR(i, 0, m)
    ans = ( ans * ms[i] ) % 1000000007;

  cout << ans << endl;

  return 0;
}
