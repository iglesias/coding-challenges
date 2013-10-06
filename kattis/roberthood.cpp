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
//

/* Point.h */

template <class T>
struct Point
{
  typedef Point P;
  typedef const P& R;
  T x, y;
  explicit Point(T x=T(), T y=T()) : x(x), y (y) { }
  bool operator<(R p) const { return x < p.x || (x==p.x && y < p.y); }
  bool operator==(R p) const { return x==p.x && y==p.y; }
  P operator-(R p) const { return P(x-p.x, y-p.y); }
  T cross(R p) const { return x*p.y - y*p.x; }
  T dist2() const { return x*x + y*y; }
};

/* convex hull */
typedef Point<int> ptype;
ptype origin;
bool comp(const ptype& p, const ptype& q)
{
  return (p-origin).cross(q-p) > 0 ||
    ((p-origin).cross(q-p) == 0 && (p-origin).dist2() < (q-origin).dist2());
}

template <class It>
It convex_hull(It begin, It end)
{
  if (end-begin < 2) return end;

  origin = *min_element(begin, end);
  sort(begin, end, comp);
  It r = begin + 1;
  for (It i = begin+2; i != end; ++i)
  {
    while (r > begin && (*r-*(r-1)).cross(*i-*r) <= 0) --r;
    // swap(*++r, *i);
    It tmp = ++r;
    *r = *i;
    *i = *tmp;
  }

  if (r-begin == 1 && *begin == *r) r--;
  return ++r;
}

int main()
{
  int C;
  cin >> C;
  vector<Point<int> > p;
  p.reserve(C);

  int x, y;
  REP(i, C)
  {
    cin >> x >> y;
    p.push_back(Point<int>(x, y));
  }

  p.resize(convex_hull(ALL(p)) - p.begin());

  int maxdist2 = 0;
  REP(i, p.size()) FOR(j, i+1, p.size())
    maxdist2 = max(maxdist2, (p[i]-p[j]).dist2());

  printf("%.8lf\n", sqrt(maxdist2));

  return 0;
}
