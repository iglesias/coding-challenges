#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

using namespace std;

double sell(double money, double price)
{
  double x = money - money*0.03;
  x = floor(x*100)/100;
  return x*price;
}

int main()
{
  vector<double> ex(365);
  vector<double> ca(365);
  vector<double> us(365);

  while (true)
  {
    int d;
    cin >> d;
    if (d == 0) break;

    REP (i, d) cin >> ex[i];

    ca[0] = 1000;
    us[0] = sell(1000, 1/ex[0]);
    FOR (i, 1, d)
    {
      ca[i] = max(ca[i-1], sell(us[i-1], ex[i]));
      us[i] = max(us[i-1], sell(ca[i-1], 1/ex[i]));
    }

    printf("%.2lf\n", ca[d-1]);
  }

  return 0;
}
