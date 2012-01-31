#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define   FOR(k, a, b)    for ( typeof(a) k = (a) ; k < (b) ; ++k )

using namespace std;

typedef pair< int, int> ii;

typedef long long ll;

bool canAlone(int u, int M);
bool canCouple(int d, int u, int M);
bool canTrio(int c, int d, int u, int M);

int main()
{
  
  int N, M, l;
  ll dp[1000];
  string status;

  scanf("%d\n", &N);

  FOR(i, 0, N)
  {
    cin >> M >> status;
    l = status.length();

    dp[l] = 1;
    for ( int j = l-1 ; j >= 0 ; --j )
      dp[j] = ( 
        (canAlone(status[j]-'0', M) ? dp[j+1] : 0) +
        ( ( j < l-1 && canCouple(status[j]-'0', status[j+1]-'0', M)) ? dp[j+2] : 0 ) +
        ( ( j < l-2 &&   canTrio(status[j]-'0', status[j+1]-'0', status[j+2]-'0', M)) ? dp[j+3] : 0 ) 
              ) % 0xfaceb00c;

    printf("Case #%d: %lld\n", i+1, dp[0]);
  }

  return 0;
}

bool canAlone(int u, int M)
{
  if ( u == 0 || u > M )
    return false;
  else
    return true;
}

bool canCouple(int d, int u, int M)
{
  if ( d == 0 || d*10 + u > M )
    return false;
  else
    return true;
}

bool canTrio(int c, int d, int u, int M)
{
  if ( c == 0 || c*100 + d*10 + u > M )
    return false;
  else
    return true;
}
