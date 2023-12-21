// {{{ Boilerplate Code <--------------------------------------------------------------------------

#include <bits/stdc++.h>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

using ii = std::pair<int, int>;

// ------------------------------------------------------------------------------> End of Boilerplate }}}

void read_input();
std::pair<int, unsigned long long> ans;
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

  auto paint_grid = [&deltas](grid const& G, grid& g, queue<ii>& q) -> queue<ii> {
    queue<ii> nq;
    set<ii> seen;
    while(!q.empty()){
      auto p = q.front();
      q.pop();
      if(seen.contains(p)) continue;
      seen.insert(p);
      for(auto const& delta : deltas){
        int r = p.first + delta.first, c = p.second + delta.second;
        if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
        if(seen.contains({r, c})) continue;
        if(G[r][c] != '#'){
          g[r][c] = 'O';
          nq.emplace(r, c);
          seen.emplace(r, c);
        }
      }
    }
    return nq;
  };

  auto make_new_positions = [&deltas](grid const& G, queue<ii>& q) -> queue<ii> {
    queue<ii> nq;
    set<ii> seen;
    while(!q.empty()){
      auto p = q.front();
      q.pop();
      if(seen.contains(p)) continue;
      seen.insert(p);
      for(auto const& delta : deltas){
        int r = p.first + delta.first, c = p.second + delta.second;
        if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
        if(seen.contains({r, c})) continue;
        if(G[r][c] != '#'){
          nq.emplace(r, c);
          seen.emplace(r, c);
        }
      }
    }
    return nq;
  };

  {
    queue<ii> q;
    q.push(start);
    grid g;
    for(int i = 0; i < 64; i++){
      g = G;
      auto nq = paint_grid(G,g,q);
      q = nq;
    }
    print(g);
    int positions = 0;
    REP(r, R) REP(c, C) if(g[r][c] == 'O') positions++;
    std::cout << positions << " " << q.size() << "\n";
  }

  {
    queue<ii> q;
    q.push(start);
    grid g;
    for(int i = 0; i < 64; i++){
      auto nq = make_new_positions(G,q);
      q = nq;
    }
    ans.first = static_cast<int>(q.size());
  }
}
