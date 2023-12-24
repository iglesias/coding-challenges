#include <bits/stdc++.h>

void read_input();
int solve_part_one();

int main()
{
  read_input();
  std::cout << "Part one: " << solve_part_one() << "\n";
}

std::vector<std::string> G;
int R, C;

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line)) G.push_back(std::move(line));
  R = static_cast<int>(G.size());
  C = static_cast<int>(G[0].size());
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

int solve_part_one()
{
  std::queue<state> q;
  state s(0,1);
  q.push(s);
  int max_num_steps = 0;

  while(!q.empty()){
    s = q.front();
    q.pop();

    if(s.p.first == R-1 and s.p.second ==  C-2){
      max_num_steps = std::max(max_num_steps, s.num_steps);
      continue;
    }

    switch(G[s.p.first][s.p.second]){
      case '.':
        for(auto const& delta : deltas){
          auto p = s.p;
          if((p.first == 0 and delta.first == -1) or (p.second == 0 and delta.second == -1)) continue;
          p.first += delta.first; p.second += delta.second;
          if(p.first > R-1 or p.second > C-1) continue;
          if(G[p.first][p.second] == '#') continue;
          if(s.visited.test(p.first*C + p.second)) continue;
          q.emplace(p.first, p.second, s.num_steps+1, s.visited);
        }
        break;
      case '>':
        s.visited.set(s.p.first*C + s.p.second);
        if(s.visited.test(s.p.first*C + s.p.second+1)) continue;
        q.emplace(s.p.first, s.p.second+1, s.num_steps+1, s.visited);
        break;
      case 'v':
        s.visited.set(s.p.first*C + s.p.second);
        if(s.visited.test((s.p.first+1)*C + s.p.second)) continue;
        q.emplace(s.p.first+1, s.p.second, s.num_steps+1, s.visited);
    }
  }

  return max_num_steps;
}
