#include <cassert>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )
#define   REPEAT(i, n)    FOR(i, 0, n)

using ii = std::pair<int, int>;

void read_input();
std::pair<int, unsigned long long> solve(int num_steps);

using namespace std;

using grid = vector<string>;

grid G;
int R, C;

void print(grid const& g)
{
  cout << "\n\n";
  REPEAT(r, R) cout << g[r] << "\n";
  cout << "\n\n";
}

int main(int argc, char* argv[])
{
  read_input();
  int const num_steps = argc > 1 ? std::stoi(argv[1]) : 64;
  std::pair const ans = solve(num_steps);
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line)){
    G.push_back(line);
    R++;
    C = int(line.size());
  }
}

vector<ii> const deltas{{0,1}, {0,-1}, {1,0} , {-1,0}};

queue<ii> paint_grid(grid const& G, grid& g, queue<ii>& q)
{
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
}

queue<ii> make_new_positions(grid const& G, queue<ii>& q)
{
  queue<ii> nq;
  unordered_set<ii, boost::hash<ii>> seen;
  while(!q.empty()){
    auto p = q.front();
    q.pop();
    if(seen.contains(p)) continue;
    seen.insert(p);
    for(auto const& delta : deltas){
      int r = p.first + delta.first, c = p.second + delta.second;
      if(seen.contains({r, c})) continue;
      // In part two the map is infinite:
      //if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
      int rr = r, cc = c;
      rr = (rr%R+R)%R; //while(rr < 0) rr += R;
      cc = (cc%C+C)%C; //while(cc < 0) cc += C;
      if(G[rr][cc] != '#'){
        nq.emplace(r, c);
        seen.emplace(r, c);
      }
    }
  }
  return nq;
}

ii get_start_position(grid const& G)
{
  REPEAT(r, R) REPEAT(c, C) if(G[r][c] == 'S') return make_pair(r, c);
  assert(false);
  return {};
}

std::pair<int, unsigned long long> solve(int num_steps)
{
  ii const start = get_start_position(G);

  std::pair<int, unsigned long long> ans;
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
    ans.first = static_cast<int>(q.size());
  }

  {
    queue<ii> q;
    q.push(start);
    grid g;
    for(int i = 0; i < num_steps; i++){
      auto nq = make_new_positions(G,q);
      q = nq;
    }

    ans.second = static_cast<int>(q.size());
  }

  return ans;
}
