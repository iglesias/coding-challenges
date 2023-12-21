// {{{ Boilerplate Code <--------------------------------------------------------------------------
//
//
// Basic ------------------------------------------------------------------------------------------

#include <bits/stdc++.h>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

using ii = std::pair<int, int>;

void read_input();
ii ans;
void solve();

using namespace std;

using grid = vector<string>;

grid G;
int R, C;

void print(grid const& g)
{
  cout << "\n\n";
  REP(r, R) cout << g[r] << "\n";
  cout << "\n\n";
}

int main()
{
  read_input();
  solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    G.push_back(line);
    R++;
    C = int(line.size());
  }
}

void solve()
{
  ii start;
  REP(r, R) REP(c, C) if(G[r][c] == 'S') start = make_pair(r, c);

  vector<ii> const deltas{{0,1}, {0,-1}, {1,0} , {-1,0}};

  auto f = [&deltas](grid const& G, grid& G1, queue<ii>& q) -> pair<queue<ii>, int> {
    queue<ii> nq;
    set<ii> seen;
    int num_pos = 0;
    while(!q.empty()){
      auto p = q.front();
      q.pop();
      if(seen.contains(p)) continue;
      seen.insert(p);
      for(auto const& delta : deltas){
        int r = p.first + delta.first, c = p.second + delta.second;
        if(0 <= r and r < R and 0 <= c and c < C and G[r][c] != '#'){
          G1[r][c] = 'O';
          num_pos++;
          nq.emplace(r, c);
        }
      }
    }
    return make_pair(nq, num_pos);
  };

  /*
  G1 = G;
  auto [q2, n2] = f(G,G1,q1);
  print(G1);

  G1 = G;
  auto [q3, n3] = f(G,G1,q2);
  print(G1);
  */

  queue<ii> q;
  q.push(start);
  grid g;
  for(int i = 0; i < 64; i++){
    g = G;
    auto [nq, nn] = f(G,g,q);
    q = nq;
  }
  print(g);
  REP(r, R) REP(c, C) if(g[r][c] == 'O') ans.first++;
}
