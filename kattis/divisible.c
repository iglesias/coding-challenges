#include <stdio.h>
#include <string.h>

#define   FOR(i, a, b)    for ( i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

int S[1000000];
int divisor;

int main()
{
  int T, n, s, i, t, ans, sum;

  scanf("%d\n", &T);
  REP(t, T)
  {
    scanf("%d %d\n", &divisor, &n);

    memset(S, '\0', sizeof(int)*divisor);

    ans = 0;
    sum = 0;
    REP(i, n)
    {
      scanf("%d", &s);
      sum += s + divisor;
      sum %= divisor;

      ans += !(sum);
      ans += S[sum];

      ++S[sum];
    }

    printf("%d\n", ans);
  }

  return 0;
}
