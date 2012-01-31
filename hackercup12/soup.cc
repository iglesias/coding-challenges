#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )

using namespace std;

int main()
{

  string HACKERCUP  = "HACKERUP";
  int N             = HACKERCUP.length();
  int count[N];

  string line;
  int ncases, ans;
  scanf("%d\n", &ncases);

  FOR(k, 0, ncases)
  {
    FOR(i, 0, N) count[i] = 0;
    getline(cin, line);

    FOR( i, 0, line.length() )
      FOR(j, 0, N)
        if ( line[i] == HACKERCUP[j] )
        {
          ++count[j];
          break;
        }

    count[2] = count[2] / 2;

    ans = *min_element(count, count + N);

    printf("Case #%d: %d\n", k+1, ans);

  }


  return 0;

}
