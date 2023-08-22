#include <bits/stdc++.h>

using namespace std;

constexpr int NMAX = 50;
using matrix = array<array<int, NMAX>, NMAX>;

int N, K;

void print(const matrix& m)
{
    for (int i=0; i<N; i++)
    {
        for (int j = 0; j<N; j++)
            cout << m[i][j] << ((j == N-1)? '\0': ' ');
        cout << '\n';
    }
}

// ls stands for Latin square
matrix gen_golden_ls()
{
    matrix m;

    // Set first row to 1, 2, 3, ..., N
    for (int j=0; j<N; j++)
        m[0][j] = j+1;
        
    // Set columns
    for (int j=0; j<N; j++)
    {
        for (int i=1; i<N; i++)
        {
            if (m[i-1][j] == 1)
                m[i][j] = N;
            else
                m[i][j] = m[i-1][j]-1;
        }
    }
    
    return m;
}

set<matrix> tried;

set<int> traces;
int trace(const matrix& m)
{
  tried.insert(m);
  int ans = 0;
  for (int i=0; i<N; i++)
    ans += m[i][i];
  traces.insert(ans);
  return ans;
}

vector<vector<int>> perms;
matrix permute_rows(const matrix& m, int i)
{
  matrix ans = m;
  for (int ii=0; ii<N; ii++)
    for (int j=0; j<N; j++)
      ans[ii][j] = m[perms[i][ii]-1][j];
  /*
  cout << "permute_rows i=" << i << endl;
  cout << "perms[i]=";
  for (int v : perms[i]) cout << v;
  cout << endl;
  cout << "permuted" << endl;
  print(ans);
  cout << endl;
  */
  return ans;
}

matrix permute_cols(const matrix& m, int j)
{
  matrix ans = m;
  for (int i=0; i<N; i++)
    for (int jj=0; jj<N; jj++)
      ans[i][jj] = m[i][perms[j][jj]-1];
  return ans;
}

int tried_count = 0;

bool search(matrix& ans, const matrix& golden)
{
  vector<int> iota_v(N);
  iota(iota_v.begin(), iota_v.end(), 1);

  perms.clear();
  perms.push_back(iota_v);
  while (next_permutation(iota_v.begin(), iota_v.end()))
    perms.push_back(iota_v);

  /*
  cout << "perms\n";
  for (const auto& perm : perms)
  {
    for (int v : perm)
      cout << v;
    cout << ' ';
  }
  cout << endl;
  */

  for (int i=0; i<(int)perms.size(); i++)
  {
    for (int j=0; j<(int)perms.size(); j++)
    {
      ans = permute_rows(golden, i);
      ans = permute_cols(ans, j);
      if (trace(ans) == K)
        return true;
      tried_count++;
    }
  }

  return false;
}

#define rep(i) for (int i=0; i<N; i++)

bool operator==(const matrix& lhs, const matrix& rhs)
{
  rep(i) rep(j)
  {
    if (lhs[i][j] < 1 || lhs[i][j] > N) assert(false);
    if (rhs[i][j] < 1 || rhs[i][j] > N) assert(false);
    if (lhs[i][j] != rhs[i][j]) return false;
  }

  return true;
}

namespace std
{
  template<> struct hash<matrix>
  {
    std::size_t operator()(matrix const& m) const noexcept
    {
      std::size_t h = 0;
      rep(i) rep(j)
      {
        h ^= i ^ j ^ m[i][j];
      }

      return h;
    } 
  };
}

unordered_set<matrix> reduced_ls;

void fill(matrix& m, int i, int j)
{
  if (j == N)
  {
    reduced_ls.insert(m);
    return;
  }

  set<int> possible;
  for (int i=1; i<=N; i++)
    possible.insert(i);
  for (int ii=0; ii<i; ii++)
    possible.erase(m[ii][j]);
  for (int jj=0; jj<j; jj++)
    possible.erase(m[i][jj]);

  for (int v : possible)
  {
    m[i][j] = v;
    if (i == N-1)
      fill(m, 1, j+1);
    else
      fill(m, i+1, j);
  }
}

int main()
{
    int T;
    cin >> T;
    for (int t=0; t<T; t++)
    {
        cin >> N >> K;

        matrix m = vector<vector<int>>(N, vector<int>(N, 0));
        rep(i)
        {
          m[i][0] = i+1;
        }

        rep(j)
        {
          m[0][j] = j+1;
        }
        
        reduced_ls.clear();
        fill(m, 1, 1);
//        cout << "#reduced_ls=" << reduced_ls.size() << endl;
        
        tried_count = 0;
        bool found = false;
        tried.clear();
        traces.clear();
        // Make all columns and rows permutations and check trace
        for (const matrix& golden : reduced_ls)
        {
          matrix ans;
          if (search(ans, golden))
          {
            cout << "Case #" << t+1 << ": POSSIBLE\n";
            print(ans);
            found = true;
            break;
          }
        }

        if (!found)
        {
//          cout << "tried_count=" << tried_count << endl;
//          cout << "#tried=" << tried.size() << endl;
//          cout << "#traces=" << traces.size() << endl;
          cout << "Case #" << t+1 << ": IMPOSSIBLE\n";
        }
    }
}

/*
2
3 6
2 3
44
5 5
5 1
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5 5
5
1 1
1 2
1 10
1 -1
1 0
6
2 0
2 -1
2 5
2 3
2 2
2 4
5
1 -1
2 -1
3 -1
4 -1
5 -1
*/
