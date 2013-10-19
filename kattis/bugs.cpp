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

struct charnode
{
  char c;
  int n;
  charnode(char _c, int _n) : c(_c), n(_n) { }
};

void removebugs(vector<charnode>& program, const string& bugstr)
{
  int idx = 0;
  while (idx != -1)
  {
    int i = 0;
    while (program[idx].c == bugstr[i++])
  }
}

int main()
{
  vector<charnode> program(10000000);
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
    int j = 0;
    REP(i, T)
    {
      getline(cin, line);
      REP(k, line.size())
      {
        program[j] = charnode(line[k], j+1);
        j++;
      }

      program[j] = charnode('\n', j+1);
      j++;
    }
    program[j-1].n = -1;

    int idx = removebugs(program, bugstr);
    while (idx != -1)
    {
      printf("%c", program[idx].c);
      idx = program[idx].n;
    }
  }

  return 0;
}
