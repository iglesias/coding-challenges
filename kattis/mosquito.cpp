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

int M, P, L, E, R, S, N;

void step()
{
  int old_m = M;
  M = P/S;
  P = L/R;
  L = old_m*E;
}

int main()
{

  string str;
  for(;;)
  {
    getline(cin, str); 
    if (str == "") break;
    stringstream ss(str);
    ss >> M >> P >> L >> E >> R >> S >> N;
    REP(i, N) step();
    cout << M << endl;
  }

  return 0;
}
