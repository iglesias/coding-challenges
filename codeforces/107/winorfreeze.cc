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

ll primeDiv(const ll & n)
{

  FOR(i, 2, sqrt(n)+1)
    if ( !(n % i) )
      return i;

  return n;
}

int main()
{

  ll q, d1, d2;

  cin >> q;

  d1 = primeDiv(q);
  //cout << ">>>> d1 = " << d1 << endl;
  if ( d1 == q )
  {
    //cout << ">>>> initially prime!" << endl;
    printf("1\n0\n");
  }
  else
  {
    d2 = primeDiv(q/d1);
    //cout << ">>>> d2 = " << d2 << endl;
    if ( d1*d2 == q )
      printf("2\n");
    else
      cout << 1 << endl << d1*d2 << endl;
  }

  return 0;
}
