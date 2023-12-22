#include <bits/stdc++.h>

void read_input();
int shortest_path(int min_straight_blocks, int max_straight_blocks);

int main()
{
  read_input();
  int ans = shortest_path(1, 3);
  std::cout << "Part one: " << ans << "\n";
  ans = shortest_path(4, 10);
  std::cout << "Part two: " << ans << '\n';
}

std::vector<std::string> G;
int R, C;


void read_input()
{
  std::string line;
  while(std::getline(std::cin, line)) G.push_back(std::move(line));
  R = (int)G.size(); C = (int)G[0].size();
}

struct vertex_t
{
  int r, c;
  vertex_t(int _r, int _c) : r(_r), c(_c) {}
};

int dirChar2Index(char dir)
{
  switch(dir){
    case '^':
      return 0;
    case '>':
      return 1;
    case 'v':
      return 2;
    case '<':
      return 3;
  }
  return -1;
}

struct state
{
  vertex_t u;
  int dist;

  char last_dir;
  int  last_dir_count;

  bool operator>(state const& other) const { return dist > other.dist; }

  state(vertex_t _u, int _dist, char _last_dir, int _last_dir_count) :
    u(_u), dist(_dist), last_dir(_last_dir), last_dir_count(_last_dir_count){}
};

std::array<char, 3> get_dirs_from(char c)
{
  switch(c){
    case '^':
      return {'^', '<', '>'};
    case '>':
      return {'>', '^', 'v'};
    case 'v':
      return {'v', '>', '<'};
    case '<':
      return {'<', 'v', '^'};
  }
  return {'x','x','x'};
}

using ii = std::pair<int, int>;

//FIXME scoping, init directly, and const
int MIN_STRAIGHT_BLOCKS;
int MAX_STRAIGHT_BLOCKS;

ii get_coords(vertex_t u, char dir)
{
  switch(dir){
    case '^':
      return {u.r-MIN_STRAIGHT_BLOCKS, u.c};
    case '>':
      return {u.r, u.c+MIN_STRAIGHT_BLOCKS};
    case 'v':
      return {u.r+MIN_STRAIGHT_BLOCKS, u.c};
    case '<':
      return {u.r, u.c-MIN_STRAIGHT_BLOCKS};
  }
  return {-1,-1};
}

int constexpr num_dirChars = 4;

ii dirChar2Delta(char dir)
{
  switch(dir){
    case '^':
      return {-1,0};
    case '>':
      return {0,+1};
    case 'v':
      return {+1,0};
    case '<':
      return {0,-1};
  }
  return {0,0};
}

using Cost = int;
using NumBlocks = int;
auto get_moves_from(state const& e) -> std::vector<std::tuple<vertex_t, char, Cost, NumBlocks>>
{
  std::vector<std::tuple<vertex_t, char, Cost, NumBlocks>> out;
  for(char const dir : get_dirs_from(e.last_dir)){
    ii const delta = dirChar2Delta(dir);
    if(dir == e.last_dir and e.last_dir_count < MAX_STRAIGHT_BLOCKS){
      int const r = e.u.r+delta.first, c = e.u.c+delta.second;
      if(r < 0 or c < 0 or r >= R or c >= C) continue;
      out.emplace_back(vertex_t(r,c), dir, G[r][c]-'0', 1);
    }else if(dir != e.last_dir){
      auto const& [r, c] = get_coords(e.u, dir);
      if(r < 0 or c < 0 or r >= R or c >= C) continue;
      Cost cost = 0;
      int rr = e.u.r, cc = e.u.c;
      for(int i = 0; i < MIN_STRAIGHT_BLOCKS; i++){
        rr += delta.first;
        cc += delta.second;
        cost += G[rr][cc]-'0';
      }
      out.emplace_back(vertex_t(r,c), dir, cost, MIN_STRAIGHT_BLOCKS);
    }
  }
  return out;
}

int shortest_path(int min_straight_blocks, int max_straight_blocks)
{
  MIN_STRAIGHT_BLOCKS = min_straight_blocks;
  MAX_STRAIGHT_BLOCKS = max_straight_blocks;
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  //FIXME reduce last dimension using MAX_STRAIGHT_BLOCKS-MIN_STRAIGHT_BLOCKS; it could also use 0-based indexing
  auto dist = std::vector(R, std::vector(C, std::vector(num_dirChars, std::vector(MAX_STRAIGHT_BLOCKS+1, INT_MAX/2))));
  int cost_1 = 0; for(int i = 1; i <= MIN_STRAIGHT_BLOCKS; i++) cost_1 += G[i][0]-'0';
  int cost_2 = 0; for(int i = 1; i <= MIN_STRAIGHT_BLOCKS; i++) cost_2 += G[0][i]-'0';
  dist[MIN_STRAIGHT_BLOCKS][0][dirChar2Index('v')][MIN_STRAIGHT_BLOCKS] = cost_1;
  dist[0][MIN_STRAIGHT_BLOCKS][dirChar2Index('>')][MIN_STRAIGHT_BLOCKS] = cost_2;

  pq.emplace(vertex_t(MIN_STRAIGHT_BLOCKS,0), cost_1, 'v', MIN_STRAIGHT_BLOCKS);
  pq.emplace(vertex_t(0,MIN_STRAIGHT_BLOCKS), cost_2, '>', MIN_STRAIGHT_BLOCKS);

  while(!pq.empty()){
    auto const e{pq.top()};
    if(e.u.r == R-1 and e.u.c == C-1) return e.dist;
    pq.pop();

    for(auto const& [v, dir, cost, nblocks] : get_moves_from(e)){
      auto tentativeScore = e.dist + cost;
      int const last_dir_count_index = (e.last_dir == dir ? e.last_dir_count+nblocks : nblocks);
      if(tentativeScore < dist[v.r][v.c][dirChar2Index(dir)].at(last_dir_count_index)){
        dist[v.r][v.c][dirChar2Index(dir)].at(last_dir_count_index) = tentativeScore;
        if(dir == e.last_dir)
          pq.emplace(v, tentativeScore, dir, e.last_dir_count+nblocks);
        else
          pq.emplace(v, tentativeScore, dir, MIN_STRAIGHT_BLOCKS);
      }
    }
  }

  return -1;
}
