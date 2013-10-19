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

#define FOR(i, a, b) for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define REP(i, n) FOR(i, 0, n)
#define ALL(a) (a).begin(), (a).end()

using namespace std;

typedef pair< int, int > ii;
typedef long long ll;

void removebugs(string& line, const string& bugstr)
{
  string::size_type n = line.find(bugstr);
  string::size_type m = line.find(bugstr[0]);
  while (n != string::npos)
  {
    line.replace(n, bugstr.size(), "");
    if (m < n)
      n = line.find(bugstr, m);
    else
    {
      m = line.find(bugstr[0], n);
      n = line.find(bugstr, n);
    }
  }
}

int main()
{
  for (;;)
  {
    string first_line;
    getline(cin, first_line);
    if (first_line == "") break;

    stringstream ss(first_line);
    int T;
    string bugstr;
    ss >> T >> bugstr;

    string line;
    REP(i, T)
    {
      getline(cin, line);
      removebugs(line, bugstr);
      cout << line << endl;
    }
  }

  return 0;
}
