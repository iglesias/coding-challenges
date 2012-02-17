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
  int n, k, l, c, d, p, nl, np;

  scanf("%d %d %d %d %d %d %d %d\n", &n, &k, &l, &c, &d, &p, &nl, &np);

  int vec[3];
  vec[0] = k*l/nl/n;
  vec[1] = c*d/n;
  vec[2] = p/np/n;

  printf("%d\n", *min_element(vec, vec+3));

  return 0;
}
