// g++ -Wall -Wextra -pedantic -Wconversion -Wfatal-errors -fsanitize=address,shadow-call-stack,undefined -fsanitize-address-use-after-scope
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> world;
int R, C; // number of rows and columns
// S position, column and row.
int r_s{-1}, c_s{-1};
void read_input();
std::vector<std::vector<int>> numbered;
std::pair<int, int> solve();

int main()
{
  read_input();
  std::pair<int, int> const ans = solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  R = 0;
  std::string line;
  while(std::getline(std::cin, line))
  {
    if(C==0) C = (int)line.length();
    assert((int)line.length()==C);
    world.push_back(std::move(line));
    R++;
  }
}

auto connects_north = [](char c)
  { return c=='S' or c=='|' or c=='L' or c=='J'; };
auto connects_south = [](char c)
  { return c=='S' or c=='|' or c=='7' or c=='F'; };
auto connects_east = [](char c)
  { return c=='S' or c=='-' or c=='L' or c=='F'; };
auto connects_west = [](char c)
  { return c=='S' or c=='-' or c=='7' or c=='J'; };

std::string generate_path_rec(int r, int c, std::string path)
{
  if(path.length() and r == r_s and c == c_s)
    return path;
  // south
  if(r<R-1 and connects_south(world[r][c]) and (!path.length() or path.back() != 'N') and world[r+1][c] != '.')
    return generate_path_rec(r+1, c, path+'S');
  // north
  if(r>0 and connects_north(world[r][c]) and (!path.length() or path.back() != 'S') and world[r-1][c] != '.')
    return generate_path_rec(r-1, c, path+'N');
  // east
  if(c<C-1 and connects_east(world[r][c]) and (!path.length() or path.back() != 'W'))
    return generate_path_rec(r, c+1, path+'E');
  // west
  if(c>0 and connects_west(world[r][c]) and (!path.length() or path.back() != 'E'))
    return generate_path_rec(r, c-1, path+'W');

  std::unreachable();
}

std::string generate_path()
{
  std::string path = generate_path_rec(r_s, c_s, "");
  return path;
}

std::pair<int, int> solve()
{
  // Find S.
  for(int r=0; r<R; r++) for(int c=0; c<C; c++)
    if(world[r][c]=='S') { r_s = r; c_s = c; break; }
  assert(r_s != -1 and c_s != -1);

  // Find the loop starting at S
  //   part one doesn't seem to need it explicitly, just the count
  numbered = std::vector<std::vector<int>>(R, std::vector<int>(C, -1));
  numbered[r_s][c_s] = 0;
  std::queue<std::pair<int, int>> q;
  q.emplace(r_s, c_s);

  while(!q.empty())
  {
    auto const [r,c] = q.front();
    q.pop();
    // south
    if(r<R-1 and connects_south(world[r][c]) and connects_north(world[r+1][c]) and numbered[r+1][c]==-1)
    {
      q.emplace(r+1, c);
      numbered[r+1][c] = numbered[r][c]+1;
    }
    // north
    if(r>0 and connects_north(world[r][c]) and connects_south(world[r-1][c]) and numbered[r-1][c]==-1)
    {
      q.emplace(r-1, c);
      numbered[r-1][c] = numbered[r][c]+1;
    }
    // east
    if(c<C-1 and connects_east(world[r][c]) and connects_west(world[r][c+1]) and numbered[r][c+1]==-1)
    {
      q.emplace(r, c+1);
      numbered[r][c+1] = numbered[r][c]+1;
    }
    // west
    if(c>0 and connects_west(world[r][c]) and connects_east(world[r][c-1]) and numbered[r][c-1]==-1)
    {
      q.emplace(r, c-1);
      numbered[r][c-1] = numbered[r][c]+1;
    }
  }

  std::pair<int, int> ans{0,0};
  for(int r=0; r<R; r++)
  {
    for(int c=0; c<C; c++)
    {
      if(r==r_s && c==c_s)
        std::cout << 'S';
      else
      {
        if(numbered[r][c]!=-1) std::cout << std::to_string(numbered[r][c]%10);
        else                   std::cout << world[r][c];
      }
      ans.first = std::max(ans.first, numbered[r][c]);
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  //generate a path along the loop (I think there're at least two paths)
  auto path = generate_path();

  auto world_experiment = world; // it will contain the loop cells denoting its direction l(left), r(right), u(up), d(down)
  auto isnt_loop = [&world_experiment](int r, int c) -> bool
  {
                      char const cell = world_experiment[r][c];
                      return !(cell=='d' or cell=='u' or cell=='r' or cell=='l');
  };

  {
    int r = r_s, c = c_s;
    for(char cell : path)
    {
      switch(cell)
      {
      case 'S':
        world_experiment[r][c] = 'd';
        r += 1;
        break;
      case 'N':
        world_experiment[r][c] = 'u';
        r -= 1;
        break;
      case 'E':
        world_experiment[r][c] = 'r';
        c += 1;
        break;
      case 'W':
        world_experiment[r][c] = 'l';
        c -= 1;
        break;
      }
    }

    for(int r=0; r<R; r++)
    {
      for(int c=0; c<C; c++)
        std::cout << world_experiment[r][c];
      std::cout << '\n';
    }
    std::cout << '\n';

    r = r_s, c = c_s;
    for(char cell : path)
    {
      // paint the non-loop cells on one side of the loop in one color(o) and cells on the other side on another (x)
      switch(cell)
      {
      case 'S':
        if(c>0 and isnt_loop(r, c-1))   { assert(world_experiment[r][c-1] != 'x');  world_experiment[r][c-1] = 'o'; }
        if(c<C-1 and isnt_loop(r, c+1)) { assert(world_experiment[r][c+1] != 'o'); world_experiment[r][c+1] = 'x'; }
        r += 1;
        break;
      case 'N':
        if(c>0 and isnt_loop(r, c-1))   world_experiment[r][c-1] = 'x';
        if(c<C-1 and isnt_loop(r, c+1))   world_experiment[r][c+1] = 'o';
        r -= 1;
        break;
      case 'E':
        if(r>0 and isnt_loop(r-1, c))   world_experiment[r-1][c] = 'x';
        if(r<R-1 and isnt_loop(r+1, c)) world_experiment[r+1][c] = 'o';
        c += 1;
        break;
      case 'W':
        if(r>0 and isnt_loop(r-1, c))   world_experiment[r-1][c] = 'o';
        if(r<R-1 and isnt_loop(r+1, c)) world_experiment[r+1][c] = 'x';
        c -= 1;
        break;
      }
    }

  }

  {
    //fill
    //color(o) another (x)
    using pairs_set = std::set<std::pair<int, int>>;
    std::function<void(int, int, char, pairs_set&)> fill;
    fill = [&world_experiment, isnt_loop, &fill](int r, int c, char color, pairs_set& visited) -> void {
      if(visited.contains(std::make_pair(r,c))) return;
      world_experiment[r][c] = color;
      visited.emplace(r, c);
      if(r>0 and isnt_loop(r-1,c))   fill(r-1, c, color, visited);
      if(r<R-1 and isnt_loop(r+1,c)) fill(r+1, c, color, visited);
      if(c>0 and isnt_loop(r,c-1))   fill(r, c-1, color, visited);
      if(c<C-1 and isnt_loop(r,c+1)) fill(r, c+1, color, visited);
    };
    pairs_set visited;
    for(int r=0; r<R; r++) for(int c=0; c<C; c++)
      if(world_experiment[r][c]=='x') fill(r, c, 'x', visited);
    visited.clear();
    for(int r=0; r<R; r++) for(int c=0; c<C; c++)
      if(world_experiment[r][c]=='o') fill(r, c, 'o', visited);

    for(int r=0; r<R; r++)
    {
      for(int c=0; c<C; c++) std::cout << world_experiment[r][c];
      std::cout << '\n';
    }
    std::cout << '\n';

    std::pair<int, int> count{0,0};
    for(int r=0; r<R; r++) for(int c=0; c<C; c++)
      if(world_experiment[r][c]=='x')      count.first++;
      else if(world_experiment[r][c]=='o') count.second++;
    std::cout << "x: " << count.first << " o: " << count.second << "\n";
  }
  return ans;
}
