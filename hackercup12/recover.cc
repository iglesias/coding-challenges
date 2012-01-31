#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#define   FOR(k, a, b)    for ( typeof(a) k = (a) ; k < (b) ; ++k )

using namespace std;

typedef pair< int, int> ii;

typedef long long ll;

// GLOBAL VARIABLES

int     N;
string  STR;
int     IDX;

///////////////////

void merge(vector< int > & arr, int i1, int j1, int i2, int j2);

ll checksum(vector< int > const & arr)
{
  ll result = 1;
  FOR(i, 0, N)
    result = (31*result + arr[i]) % 1000003;

  return result;
}


void mergeRecover(vector< int > & arr, int i, int j)
{
  int n = j - i + 1;
  
  if ( n <= 1 )
    return;

  int mid = floor(n/2) + i;

  mergeRecover(arr, i  , mid-1);
  mergeRecover(arr, mid,     j);

  merge(arr, i, mid-1, mid, j);
}

void merge(vector< int > & arr, int i1, int j1, int i2, int j2)
{
  int k = 0, n = j2 - i1 + 1, start = i1, end = j2;
  vector< int > sortArr(n);

  while ( i1 <= j1 && i2 <= j2 )
  {
    if ( STR[IDX] == '1' )
    {
      sortArr[k] = arr[i1];
      ++i1;
    }
    else if ( STR[IDX] == '2' )
    {
      sortArr[k] = arr[i2];
      ++i2;
    }
    else
      printf("Something screwed! Debug is not '1' or '2'\n");

    ++IDX, ++k;
  }

  while ( i1 <= j1 )  sortArr[k++] = arr[i1++];
  while ( i2 <= j2 )  sortArr[k++] = arr[i2++];

  for ( int i = start, j = 0 ; i <= end ; ++i, ++j )
    arr[i] = sortArr[j];
}

int main()
{
  int T;

  scanf("%d\n", &T);

  FOR(i, 0, T)
  {
    scanf("%d\n", &N);

    vector< int > arr(N);
    FOR(j, 0, N)  arr[j] = j;

    getline(cin, STR);
    IDX = 0;

    mergeRecover(arr, 0, N-1);
    
    vector< int > org(N);
    FOR(j, 0, N)
      org[ arr[j] ] = j+1;

    printf("Case #%d: %lld\n", i+1, checksum(org));
  }

  return 0;
}
