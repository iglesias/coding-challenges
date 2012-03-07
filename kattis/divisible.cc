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

#define   FOR(i, a, b)    for ( i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

using namespace std;

typedef pair< int, int >  ii;
typedef long long         ll;

int S[1000000];
int d;

int main()
{
  int T, n, s, i, t, ans, sum;

  scanf("%d\n", &T);
  REP(t, T)
  {
    scanf("%d %d\n", &d, &n);

    REP(i, d) S[i] = 0;

    ans = 0;
    sum = 0;
    REP(i, n)
    {
      scanf("%d", &s);
      sum += s % d;
      sum %= d;

      ans += !(sum);
      ans += S[sum];

      ++S[sum];
    }

    printf("%d\n", ans);
  }

  return 0;
}
