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

int solve(int n)
{
  n = (n*567/9 + 7492)*235/47 - 498;
  n = abs(n);
//   if (n < 10) return 0;
  n /= 10;
  return n%10;
}

int main()
{
  int t;
  cin >> t;
  REP(i, t)
  {
    int n;
    cin >> n;
    cout << solve(n) << endl;
  }

  return 0;
}
