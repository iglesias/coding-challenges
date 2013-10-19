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
  vector<vector<string> > pages;
  int T;
  cin >> T;
  REP(t, T)
  {
    pages.clear();
    pages.resize(101);
    int max_rank = 0;
    REP(i, 10)
    {
      string s;
      int j;
      cin >> s;
      cin >> j;
      pages[j].push_back(s);
      if (j > max_rank) max_rank = j;
    }

    cout << "Case #" << t+1 << ":" << endl;
    REP(j, pages[max_rank].size())
      cout << pages[max_rank][j] << endl;
  }

  return 0;
}
