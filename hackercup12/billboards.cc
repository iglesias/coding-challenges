#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )

using namespace std;

int main()
{

  string line, word;
  vector< string > words;
  int T, W, H, ans = 0;
  scanf("%d\n", &T);

  FOR(k, 0, T)
  {
    words.clear();
    getline(cin, line);
    stringstream ss(line);
    ss >> W >> H;

    do
    {
      ss >> word;
      words.push_back(word);
    } while ( !ss.eof() );

    // Get the upper bound: C*(size)^2 - H*size <= W*H

    // Number of characters in the sentence
    int C = 0;
    FOR ( i, 0, words.size() )
      C += words[i].length();

    int ub = sqrt(W*H/C);

    for ( int i = ub ; i >= 1 ; --i )
    {

      // All individual words fit in a line
      FOR( w, 0, words.size() )
        if ( (int) words[w].length()*i > W )
          continue;

      int Hleft = H-i;
      int Wleft = W;

      int w = 0;
      while ( w < (int) words.size() )
      {
      
        if ( Hleft < 0 )
          break;

        if ( (int) words[w].length()*i <= Wleft )
        {
          //printf(">>>> %s fits i = %d\n", words[w].c_str(), i);
          Wleft -= (words[w].length() + 1)*i;
          //printf(">>>> Wleft = %d\n", Wleft);
          ++w;
        }
        else
        {
          //printf(">>>> next line\n");
          Wleft  = W;
          Hleft -= i;
        }

      }

      if ( w >= (int) words.size() )
      {
        ans = i;
        break;
      }

    }

    printf("Case #%d: %d\n", k+1, ans);

  }


  return 0;

}
