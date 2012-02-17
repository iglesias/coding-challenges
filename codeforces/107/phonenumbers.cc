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

bool isTaxi(const string & number)
{
  int digit = number[0];
  FOR( i, 1, number.length() )
  {
    if ( i == 2 || i == 5 ) continue;
    if ( digit != number[i] ) return false;
  }
  return true;
}

bool isPizza(const string & number)
{
  int digit = number[0];
  FOR( i, 1, number.length() )
  {
    if ( i == 2 || i == 5 ) continue;
    if ( digit <= number[i] ) return false;
    digit = number[i];
  }
  return true;
}

vector< string > names;

string maximize(const vector< int > & v)
{

  int maxValue = *max_element(ALL(v));

  string ans;
  bool found = false;
  FOR(i, 0, v.size())
  {

    if ( v[i]  == maxValue )
    {
      if ( !found )
      {
        found = true; 
        ans = names[i];
      }
      else
      {
        ans += ", ";
        ans += names[i];
      }
    }

  }

  return ans;

}

int main()
{
  int n, s;
  string number, ans;

  scanf("%d\n", &n);

  names.resize(n);
  vector< int >    taxis  (n, 0);
  vector< int >    pizzas (n, 0);
  vector< int >    girls  (n, 0);

  FOR(i, 0, n)
  {
    cin >> s >> names[i];
    FOR(j, 0, s)
    {
      cin >> number;
      if ( isTaxi(number) ) ++taxis[i];
      else if ( isPizza(number) ) ++pizzas[i];
      else ++girls[i];
    }
  }

  ans = maximize(taxis);
  printf("If you want to call a taxi, you should call: %s.\n", ans.c_str());
  ans.clear();
  ans = maximize(pizzas);
  printf("If you want to order a pizza, you should call: %s.\n", ans.c_str());
  ans.clear();
  ans = maximize(girls);
  printf("If you want to go to a cafe with a wonderful girl, you should call: %s.\n", 
          ans.c_str());

  return 0;
}
