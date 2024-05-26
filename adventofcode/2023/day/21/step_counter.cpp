#include <cassert>
#include <chrono>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
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
  static bool first_call = true;
  if (first_call) first_call = false;
  else            cout << "\033[" << R << "A\033[" << C << "D";
  REPEAT(r, R) cout << g[r] << "\n";
  using namespace chrono_literals;
  this_thread::sleep_for(200ms);
}

int main(int argc, char* argv[])
{
  read_input();
  int const num_steps = argc > 1 ? stoi(argv[1]) : 64;
  pair const ans = solve(num_steps);
  cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  string line;
  while(getline(cin, line)){
    G.push_back(line);
    R++;
    C = int(line.size());
  }
}

vector<ii> const deltas{{0,1}, {0,-1}, {1,0} , {-1,0}};

queue<ii> paint_grid(grid const& G, grid& g, queue<ii>& q)
{
  queue<ii> nq;
  set<ii> nqed;
  while(!q.empty()){
    auto p = q.front();
    q.pop();
    for(auto const& delta : deltas){
      int r = p.first + delta.first, c = p.second + delta.second;
      if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
      if(nqed.contains({r, c})) continue;
      if(G[r][c] != '#'){
        g[r][c] = 'O';
        nq.emplace(r, c);
        nqed.emplace(r, c);
      }
    }
  }
  return nq;
}

queue<ii> make_new_positions(grid const& G, queue<ii>& q)
{
  queue<ii> nq;
  unordered_set<ii, boost::hash<ii>> nqed;
  while(!q.empty()){
    ii const p = q.front();
    q.pop();
    for(ii const& delta : deltas){
      int const r = p.first + delta.first, c = p.second + delta.second;
      // In part two the map is infinite:
      //if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
      int const rr = ((r % R) + R) % R; //while(rr < 0) rr += R;
      int const cc = ((c % C) + C) % C; //while(cc < 0) cc += C;
      if(G[rr][cc] != '#' and !nqed.contains({r, c})){
        nq.emplace(r, c);
        nqed.emplace(r, c);
      }
    }
  }
  return nq;
}

using num_steps_t = int;
unsigned long long solve_part_two(grid const& G, ii const& start, num_steps_t const num_steps)
{
  queue<pair<ii, num_steps_t>> q;
  q.emplace(start, 0);
  unordered_set<pair<ii, num_steps_t>, boost::hash<pair<ii, num_steps_t>>> qed;
  qed.emplace(start, 0);
  unsigned long long ans = 0;
  while(!q.empty()){
    pair<ii, num_steps_t> const p = q.front();
    q.pop();
    if(p.second == num_steps) {
      ans++;
      continue;
    }
    for(auto const& delta : deltas){
      ii const pos = make_pair(p.first.first + delta.first, p.first.second + delta.second);
      if(qed.contains({pos, p.second + 1})) continue;
      int const rr = ((pos.first % R) + R) % R;
      int const cc = ((pos.second % C) + C) % C;
      if(G[rr][cc] != '#'){
        q.emplace(pos, p.second + 1);
        qed.emplace(pos, p.second + 1);
      }
    }
  }
  return ans;
}

ii get_start_position(grid const& G)
{
  REPEAT(r, R) REPEAT(c, C) if(G[r][c] == 'S') return make_pair(r, c);
  assert(false);
  return {};
}

pair<int, unsigned long long> solve(int num_steps)
{
  ii const start = get_start_position(G);

  pair<int, unsigned long long> ans;
  {
    queue<ii> q;
    q.push(start);
    grid g;
    for(int i = 0; i < 64; i++){
      g = G;
      q = paint_grid(G,g,q);
      //print(g);
    }
    ans.first = static_cast<int>(q.size());
  }

  {
    queue<ii> q;
    q.push(start);
    grid g;
    for(int i = 0; i < num_steps; i++){
      q = make_new_positions(G,q);
      cout << q.size() << endl;
    }

    ans.second = static_cast<int>(q.size());
  }
  //cout << "solve_part_two=" << solve_part_two(G, start, num_steps) << endl;

  return ans;
}
