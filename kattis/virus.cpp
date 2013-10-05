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

int virus(const string& str1, const string& str2)
{
  int start = -1;
  REP(i, min(str1.length(), str2.length()))
    if (str1[i] != str2[i])
    {
      start = i;
      break;
    }

  if (start == -1)
  {
    if (str1.length() >= str2.length())
      return 0;
    else
      return str2.length()-str1.length();
  }

  int end = -1;
  unsigned int i = str1.length()-1;
  unsigned int j = str2.length()-1;
  while (i >= 0 && j >= 0)
  {
    if (str1[i] != str2[j])
    {
      end = j;
      break;
    }

    i--, j--;
  }

  if (end < start)  return 0;
  else              return end-start+1;
}

int main()
{
  string str1, str2;
  cin >> str1 >> str2;
  cout << virus(str1, str2) << endl;
  return 0;
}
