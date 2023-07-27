#include <bits/stdc++.h>

// sample input at the bottom

using namespace std;

string to_binary(int n, int N)
{
    string ans;

    if (n == 0) ans = "0";
    else{
      while (n > 0)
      {
          if (n%2)   ans.push_back('1');
          else        ans.push_back('0');
          n  /= 2;
      }
    }

    while((int)ans.length() < N)
      ans.push_back('0');

    reverse(ans.begin(), ans.end());
    return ans;
}

vector<pair<int, int>> intervals;
bool overlap(int i, int j)
{
  pair<int, int> a = intervals[i];
  pair<int, int> b = intervals[j];

  set<pair<int, int>> sorted;
  sorted.insert(a);
  sorted.insert(b);

  //cout << i << ' ' << j << " overlap " << (sorted.begin()->second > sorted.rbegin()->first) << endl;

  return sorted.begin()->second > sorted.rbegin()->first;
}

int main()
{
    int T;
    cin >> T;
    
    for (int t=0; t<T; t++)
    {
        int N;
        cin >> N;

        intervals.clear();
        for (int n=0; n<N; n++)
        {
          int s, e;
          cin >> s >> e;
          intervals.emplace_back(s, e);
        }

        int n = (1<<N) - 1;
        bool found = false;
        while (n >= 0 && !found)
        {
            string code = to_binary(n--, N);
            //cout << code << endl;

            bool stop = false;
            for (int i=0; i<(int)code.length() && !stop; i++)
            {
              for (int j=i+1; j<(int)code.length() && !stop; j++)
              {
                if (code[i] != code[j]) continue;
                if (overlap(i, j)) stop = true;
              }
            }

            if (!stop)
            {
              found = true;
              for (char& c : code)
              {
                if (c == '0') c = 'C';
                else          c = 'J';
              }
              cout << "Case #" << t+1 << ": " << code << '\n';
            }
        }

        if (!found)
          cout << "Case #" << t+1 << ": " << "IMPOSSIBLE\n";
    }
}

/*
4
3
360 480
420 540
600 660
3
0 1440
1 3
2 4
5
99 150
1 100
100 301
2 5
150 250
2
0 720
720 1440
*/
