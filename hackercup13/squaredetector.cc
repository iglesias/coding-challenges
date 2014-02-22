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
#include <unordered_set>

#define   FOR(i, a, b)    for ( auto i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

using namespace std;

typedef pair< int, int >  ii;
typedef long long         ll;

// }}}

int main()
{
  unordered_set<int> squares;
  FOR(i, 1, 21) squares.insert(i*i);
  int T, N;
  cin >> T;
  REP(t, T)
  {
    cin >> N;
    vector<string> board(N);
    REP(i, N) cin >> board[i];
    int num_black = 0;
    REP(i, N) REP(j, N) if (board[i][j] == '#') num_black++;
//     printf("num_black=%d\n", num_black);
    if (num_black == 1)
    {
      cout << "Case #" << t+1 << ": YES\n";
    }
    else if (squares.find(num_black) == squares.end())
    {
//       printf(">>>> not in squares\n");
      cout << "Case #" << t+1 << ": NO\n";
    }
    else
    {
      queue<ii> q;
      REP(i, N)
      {
        REP(j, N)
        {
          if (board[i][j] == '#')
          {
            q.push(ii(i,j));
            break;
          }
        }
        if (!q.empty()) break;
      }

      int num_found = 0;
      while (!q.empty())
      {
        ii cur = q.front();
        q.pop();

        if (board[cur.first][cur.second] == 'X') continue;

        num_found++;
        board[cur.first][cur.second] = 'X';
        if (cur.first+1 < N && board[cur.first+1][cur.second] == '#')
          q.push(ii(cur.first+1, cur.second));
        if (cur.second+1 < N && board[cur.first][cur.second+1] == '#')
          q.push(ii(cur.first, cur.second+1));
      }

//       printf("num_found=%d\n", num_found);
      if (num_found != num_black)
      {
        cout << "Case #" << t+1 << ": NO\n";
      }
      else
      {
        int min_i = 100;
        int min_j = 100;
        int max_i = -1;
        int max_j = -1;
        REP(i, N) REP(j, N)
        {
          if (board[i][j] == 'X' || board[i][j] == '#')
          {
            min_i = min(min_i, i);
            max_i = max(max_i, i);

            min_j = min(min_j, j);
            max_j = max(max_j, j);
          }
        }

//         printf("max_i=%d min_i=%d max_j=%d min_j=%d\n", max_i, min_i, max_j, min_j);
        if (max_i-min_i == max_j-min_j)
          cout << "Case #" << t+1 << ": YES\n";
        else
          cout << "Case #" << t+1 << ": NO\n";
      }
    }
  }
  return 0;
}
