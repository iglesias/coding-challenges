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
    if (scanf("%d %d\n", &divisor, &n) != 2) {
      fprintf(stderr, "Error reading input for divisor and/or n\n");
      return 1;
    }

    memset(S, '\0', sizeof(int)*divisor);

    ans = 0;
    sum = 0;
    REP(i, n)
    {
      if (scanf("%d", &s) != 1) {
        fprintf(stderr, "Error reading input for s\n");
        return 1;
      }
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
