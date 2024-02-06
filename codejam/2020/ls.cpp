#include <bits/stdc++.h>

#define rep(i, n) for (int (i) = 0; (i) < (n); (i)++)

namespace ls {

using namespace std;

constexpr int NMAX = 50;
using matrix = array<array<int, NMAX>, NMAX>;

// FIXME globals.
int N, K;

}

namespace std {
  template<> struct hash<ls::matrix>
  {
    std::size_t operator()(ls::matrix const& m) const noexcept
    {
      std::size_t h = 0;
      rep(i, ls::N) rep(j, ls::N)
      {
        h ^= i ^ j ^ m[i][j];
      }

      return h;
    } 
  };
}

namespace ls {

matrix permute_cols(const matrix& m, int j);
matrix permute_rows(const matrix& m, int i);
int trace(const matrix& m);

// FIXME globals.
vector<vector<int>> perms;
int tried_count = 0;

bool search(matrix& ans, const matrix& golden)
{
  vector<int> iota_v(N);
  iota(iota_v.begin(), iota_v.end(), 1);

  perms.clear();
  perms.push_back(iota_v);
  while(next_permutation(iota_v.begin(), iota_v.end())) perms.push_back(iota_v);

  rep(i, static_cast<int>(perms.size())) rep(j, static_cast<int>(perms.size()))
  {
    ans = permute_rows(golden, i);
    ans = permute_cols(ans, j);
    if (trace(ans) == K) return true;
    tried_count++;
  }

  return false;
}

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

matrix permute_rows(const matrix& m, int i)
{
  matrix ans = m;
  for (int ii=0; ii<N; ii++)
    for (int j=0; j<N; j++)
      ans[ii][j] = m[perms[i][ii]-1][j];
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

bool operator==(const matrix& lhs, const matrix& rhs)
{
  rep(i, N) rep(j, N)
  {
    if (lhs[i][j] < 1 || lhs[i][j] > N) assert(false);
    if (rhs[i][j] < 1 || rhs[i][j] > N) assert(false);
    if (lhs[i][j] != rhs[i][j]) return false;
  }

  return true;
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
  for (int i=1; i<=N; i++)    possible.insert(i);
  for (int ii=0; ii<i; ii++)  possible.erase(m[ii][j]);
  for (int jj=0; jj<j; jj++)  possible.erase(m[i][jj]);

  for (int v : possible)
  {
    m[i][j] = v;
    if (i == N-1) fill(m, 1, j+1);
    else          fill(m, i+1, j);
  }
}

} // namespace ls

int main()
{
    int T;
    std::cin >> T;
    for (int t=0; t<T; t++)
    {
      std::cin >> ls::N >> ls::K;

        //matrix m = vector<vector<int>>(N, vector<int>(N, 0));
        ls::matrix m;
        rep(i, ls::N) m[i][0] = i+1;

        rep(j, ls::N) m[0][j] = j+1;
        
        ls::reduced_ls.clear();
        ls::fill(m, 1, 1);
        
        ls::tried_count = 0;
        bool found = false;
        ls::tried.clear();
        ls::traces.clear();
        // Make all columns and rows permutations and check trace
        for (const ls::matrix& golden : ls::reduced_ls)
        {
          ls::matrix ans;
          if (ls::search(ans, golden))
          {
            std::cout << "Case #" << t+1 << ": POSSIBLE\n";
            ls::print(ans);
            found = true;
            break;
          }
        }

        if (!found)
        {
//          cout << "tried_count=" << tried_count << endl;
//          cout << "#tried=" << tried.size() << endl;
//          cout << "#traces=" << traces.size() << endl;
          std::cout << "Case #" << t+1 << ": IMPOSSIBLE\n";
        }
    }
}
