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
  ll n, m, xc, yc, k, dx, dy, nsteps = 0, diffx, diffy, ans = 0, ddx, ddy;

  cin >> n  >> m;
  cin >> xc >> yc;
  cin >> k;

  FOR(i, 0, k)
  {
    cin >> dx >> dy;

    if ( dx > 0 )
      diffx = abs(xc - n);
    else
      diffx = abs(xc - 1);

    if ( dy > 0 )
      diffy = abs(yc - m);
    else
      diffy = abs(yc - 1);

    ddx = abs(dx);
    ddy = abs(dy);

    if ( dx != 0 && dy != 0 )
      nsteps = min(diffx / ddx, diffy / ddy);
    else if ( dx == 0 )
      nsteps = diffy / ddy;
    else if ( dy == 0 )
      nsteps = diffx / ddx;

    //cout << ">>>> nsteps = " << nsteps << endl;

    xc += nsteps*dx;
    yc += nsteps*dy;

    ans += nsteps;
  }

  cout << ans << endl;

  return 0;
}
