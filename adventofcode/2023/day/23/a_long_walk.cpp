#include <bitset>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

void read_input();
int solve(bool part_two=false);

int main()
{
  read_input();
  int ans = solve();
  std::cout << "Part one: " << ans << '\n';
  bool part_two = true;
  ans = solve(part_two);
  std::cout << "Part two: " << ans << '\n';
}

std::vector<std::string> G;
int R, C;

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line)) G.push_back(std::move(line));
  R = (int)G.size();
  C = (int)G[0].size();
}

int constexpr AXIS_MAX = 141;

struct state
{
  std::pair<int, int> p;
  int num_steps;
  using bitmask = std::bitset<AXIS_MAX*AXIS_MAX>;
  bitmask visited;

  state(int r, int c) : p(r, c), num_steps(0), visited(0) { visited.set(r*C + c); }
  state(int r, int c, int n, bitmask const& v) : p(r, c), num_steps(n), visited(v)
  {
    visited.set(r*C + c);
  }
};

std::vector<std::pair<int, int>> const deltas = {{0,1}, {1,0}, {-1,0}, {0,-1}};

template<typename T1, typename T2> std::pair<T1, T2> operator+(std::pair<T1, T2> const& p1, std::pair<T1, T2> const& p2)
{
  return std::make_pair(p1.first+p2.first, p1.second+p2.second);
}

template<typename Container> void handle_dot_cell(Container& c, state const& s)
{
  for(auto const& delta : deltas){
    auto const p = s.p + delta;
    if(0 > p.first or 0 > p.second or p.first > R-1 or p.second > C-1) continue;
    if(G[p.first][p.second] == '#') continue;
    if(s.visited.test(p.first*C + p.second)) continue;
    c.emplace(p.first, p.second, s.num_steps+1, s.visited);
  }
}

int solve(bool part_two)
{
  //std::queue<state> q;
  std::stack<state> q;
  q.push(state(0,1));
  int max_num_steps = 0;
  while(!q.empty()){
    //state s = q.front();
    state s = q.top();
    q.pop();
    if(s.p.first == R-1 and s.p.second ==  C-2){
      if(s.num_steps > max_num_steps) std::cout << "s.num.steps=" << s.num_steps << std::endl;
      max_num_steps = std::max(max_num_steps, s.num_steps);
      continue;
    }
    if(part_two and ((G[s.p.first][s.p.second] == '.') or (G[s.p.first][s.p.second] == '>') or (G[s.p.first][s.p.second] == 'v'))){
      handle_dot_cell(q, s);
    }else if(!part_two){
      switch(G[s.p.first][s.p.second]){
        case '.':
          handle_dot_cell(q, s);
          break;
        case '>':
          if(s.visited.test(s.p.first*C + s.p.second+1)) continue;
          q.emplace(s.p.first, s.p.second+1, s.num_steps+1, s.visited);
          break;
        case 'v':
          if(s.visited.test((s.p.first+1)*C + s.p.second)) continue;
          q.emplace(s.p.first+1, s.p.second, s.num_steps+1, s.visited);
      }
    }
  }
  return max_num_steps;
}
