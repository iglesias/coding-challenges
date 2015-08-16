// code.google.com/codejam/contest/351101/dashboard

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

#define   FOR(i, a, b)    for ( auto i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

using namespace std;

int main()
{
  int N;
  cin >> N;

  REP(n,N)
  {
    int C;
    cin >> C;
    int I;
    cin >> I;
    vector<int> v(I), w(I);
    REP(i,I)
    {
      cin >> v[i];
      w[i] = v[i];
    }

    sort(ALL(w));
    int i = 0;
    while (true)
    {
      if (binary_search(ALL(w), C-w[i])) break;
      i++;
    }

    auto it1 = find(ALL(v), w[i]);
    auto it2 = it1;
    if (2*w[i] == C)
      it2 = find(it1++, v.end(), C-w[i]);
    else
      it2 = find(ALL(v), C-w[i]);

    vector<int> idxs(2);
    idxs[0] = it1 - v.begin() + 1;
    idxs[1] = it2 - v.begin() + 1;
    sort(ALL(idxs));

    if (idxs[0] == idxs[1]) idxs[1]++;

    printf("Case #%d: %d %d\n", n+1, idxs[0], idxs[1]);
  }

  return 0;
}
