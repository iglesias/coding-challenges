#include <cmath>
#include <cstdio>
#include <iostream>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )

using namespace std;

typedef long long         ll;

ll primeDiv(const ll & n)
{

  FOR(i, 2, sqrt(n)+1)
    if ( !(n % i) )
      return i;

  return n;
}

int main()
{

  ll q, d1, d2;

  cin >> q;

  d1 = primeDiv(q);
  //cout << ">>>> d1 = " << d1 << endl;
  if ( d1 == q )
  {
    //cout << ">>>> initially prime!" << endl;
    printf("1\n0\n");
  }
  else
  {
    d2 = primeDiv(q/d1);
    //cout << ">>>> d2 = " << d2 << endl;
    if ( d1*d2 == q )
      printf("2\n");
    else
      cout << 1 << endl << d1*d2 << endl;
  }

  return 0;
}
