// {{{ Boilerplate Code <--------------------------------------------------------------------------
//
//
// Basic ------------------------------------------------------------------------------------------

#include <bits/stdc++.h>
#include <cstdio>
#include <string>
#include <utility>

#define   ALL(a)          (a).begin(), (a).end()
#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

using ii = std::pair<int, int>;
using ll = long long;

// I/O --------------------------------------------------------------------------------------------

#include <cxxabi.h>

// Specialization to PRINT std::vector<T>.
// This type of function wouldn't be needed using fmt.
template<typename T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  /*
  // Print the vector's template type name demangled.
  // It is somewhat polluting in common cases.
  //
  // FIXME actually, what'd be more useful for printing
  // with the vector instead of the type are the indices
  // to the items.
  int status;
  char* demangled_name =
    abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
  assert(!status);
  os << "vector<" << demangled_name << ">(" << (v.size() ? '\0' : ')'); // taking care of closing () if v=[]
  std::free(demangled_name);
  */
  os << "vector(" << (v.size() ? '\0' : ')');

  // When the vector is of strings or so, they can contain commas and
  // the separation between elements of the vector won't be clear.
  // Solve it by printing strings between quotes (or an opening and a
  // closing character in general).
  char opening{}, closing{};
  if constexpr(std::is_same<T, std::string>() or std::is_same<T, char const*>()) opening = closing = '\'';
  for(size_t i{0}; i < v.size(); i++)
    os << opening << v[i] << closing << ((i == v.size()-1)? ")" : ", ");

  return os;
}

template<typename T> std::ostream& operator<<(std::ostream& os, std::pair<T,T> const& p)
{
  os << "pair(";

  // When the pair is of strings or so, they can contain commas and
  // the separation between elements of the vector won't be clear.
  // Solve it by printing strings between quotes (or an opening and a
  // closing character in general).
  char opening{}, closing{};
  if constexpr(std::is_same<T, std::string>() or std::is_same<T, char const*>()) opening = closing = '\'';
    os << opening << p.first << closing << ", " << opening << p.second << closing << ")";

  return os;
}

#define   PRINT(...)      print(#__VA_ARGS__, __VA_ARGS__)
// TODO   separate print w/ & w/o template type names (PRINTT)

template<typename... Args> void print(std::string vars, Args&&... values)
{
  std::cout << "[" << vars << " = ";
  std::string delim = "";
  (..., (std::cout << delim << values, delim = ", "));
  std::cout << "]\n";
}

// String Utils --------------------------------------------------------------------------------------------

//FIXME improve template with concept or traits.
template<typename string> auto split(std::string const& str, string sep) -> std::vector<std::string>
{
  std::string::size_type pos{0}, n;
  std::vector<std::string> out;

  while((n = str.find(sep, pos)) != std::string::npos)
  {
    out.push_back(str.substr(pos, n-pos));
    pos = n+1;
  }

  if(pos != 0) out.push_back(str.substr(pos));

  return out;
}

// ------------------------------------------------------------------------------> End of Boilerplate }}}

std::vector<std::string> world;
int R, C; // number of rows and columns
// S position, column and row.
int r_s{-1}, c_s{-1};
void read_input();
// I am somewhat confident long long is not required. 
std::vector<std::vector<long long>> numbered;
std::pair<long long, long long> ans;
void solve();

int main()
{
  read_input();
  //PRINT(world);
  for(int r=0; r<R; r++)
  {
    for(int c=0; c<C; c++)
      std::cout << world[r][c];
    std::cout << '\n';
  }
  solve();
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
  { return c=='S' or c=='|' or c=='7' or c=='F'; }; //TODO
auto connects_east = [](char c)
  { return c=='S' or c=='-' or c=='L' or c=='F'; }; //TODO
auto connects_west = [](char c)
  { return c=='S' or c=='-' or c=='7' or c=='J'; }; //TODO

std::string generate_path_rec(int r, int c, std::string path)
{
  //PRINT(__func__, r, c, path);
  if(path.length() and r == r_s and c == c_s)
  {
    return path;
  }
  // south
  if(r<R-1 and connects_south(world[r][c]) and (!path.length() or path.back() != 'N') and world[r+1][c] != '.')
  {
    //r += 1;
    return generate_path_rec(r+1, c, path+'S');
  }
  // north
  if(r>0 and connects_north(world[r][c]) and (!path.length() or path.back() != 'S') and world[r-1][c] != '.')
  {
    //r -= 1;
    return generate_path_rec(r-1, c, path+'N');
  }
  // east
  if(c<C-1 and connects_east(world[r][c]) and (!path.length() or path.back() != 'W')) //FIXME the condition in world != '.' could be relevant here too
  {
    //c += 1;
    return generate_path_rec(r, c+1, path+'E');
  }
  // west
  if(c>0 and connects_west(world[r][c]) and (!path.length() or path.back() != 'E')) //FIXME the condition in world != '.' could be relevant here too

  {
    //c -= 1;
    return generate_path_rec(r, c-1, path+'W');
  }
  assert(false);
  return path;
}

std::string generate_path()
{
  std::string path = generate_path_rec(r_s, c_s, "");
  return path;
}

void solve()
{
  // Find S.
  for(int r=0; r<R; r++) for(int c=0; c<C; c++)
    if(world[r][c]=='S') { r_s = r; c_s = c; break; }
  assert(r_s != -1 and c_s != -1);
  PRINT(r_s, c_s);

  // Find the loop starting at S
  //   part one doesn't seem to need it explicitly, just the count
  numbered = std::vector<std::vector<long long>>(R);
  for(int r=0; r<R; r++) numbered[r] = std::vector<long long>(C, -1);
  numbered[r_s][c_s] = 0;
  std::queue<ii> q;
  q.emplace(r_s, c_s);

  while(!q.empty())
  {
    ii p = q.front();
    q.pop();
    int r = p.first, c = p.second; //FIXME sugar
    // south
    //if(r<R-1 and connects_north(world[r+1][c]) and numbered[r+1][c]==-1)
    if(r<R-1 and connects_south(world[r][c]) and connects_north(world[r+1][c]) and numbered[r+1][c]==-1)
    {
      q.emplace(r+1, c);
      numbered[r+1][c] = numbered[r][c]+1;
    }
    // north
    //if(r>0 and connects_south(world[r-1][c]) and numbered[r-1][c]==-1)
    if(r>0 and connects_north(world[r][c]) and connects_south(world[r-1][c]) and numbered[r-1][c]==-1)
    {
      q.emplace(r-1, c);
      numbered[r-1][c] = numbered[r][c]+1;
    }
    // east
    //if(c<C-1 and connects_west(world[r][c+1]) and numbered[r][c+1]==-1)
    if(c<C-1 and connects_east(world[r][c]) and connects_west(world[r][c+1]) and numbered[r][c+1]==-1)
    {
      q.emplace(r, c+1);
      numbered[r][c+1] = numbered[r][c]+1;
    }
    // west
    //if(c>0 and connects_east(world[r][c-1]) and numbered[r][c-1]==-1)
    if(c>0 and connects_west(world[r][c]) and connects_east(world[r][c-1]) and numbered[r][c-1]==-1)
    {
      q.emplace(r, c-1);
      numbered[r][c-1] = numbered[r][c]+1;
    }
  }

  //PRINT(numbered);
  for(int r=0; r<R; r++)
  {
    for(int c=0; c<C; c++)
    {
      if(r==r_s && c==c_s)
        std::cout << 'S';
      else
      {
      //std::cout << (numbered[r][c]!=-1 ? std::to_string(numbered[r][c]) : ".");
      //std::cout << (numbered[r][c]!=-1 ? std::to_string(numbered[r][c]%10) : std::to_string(world[r][c]));
      //std::cout << (numbered[r][c]!=-1 ? (numbered[r][c]>9 ? "X" : std::to_string(numbered[r][c])) : ".");
      //std::cout << (numbered[r][c]!=-1 ? world[r][c] : '.');
      //std::cout << (numbered[r][c]!=-1 ? 'X' : '.');
      if(numbered[r][c]!=-1)
        std::cout << std::to_string(numbered[r][c]%10);
      else
        std::cout << world[r][c];
      }
      ans.first = std::max(ans.first, numbered[r][c]);
    }
    std::cout << '\n';
  }

  //generate a path along the loop (I think there're at least two paths)
  auto path = generate_path();
  PRINT(path);

  //WIP now, let's see how we can use the path and the loop to continue solving the problem looking into the enclosing area
  auto world_experiment = world; // it will contain the loop cells denoting its direction l(left), r(right), u(up), d(down)
  auto isnt_loop =
    [&world_experiment](int r, int c) -> bool {
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
    std::cout << '\n';
    for(int r=0; r<R; r++)
    {
      for(int c=0; c<C; c++)
        std::cout << world_experiment[r][c];
      std::cout << '\n';
    }
    std::cout << '\n';
  {
    //fill
    //color(o) another (x)
    std::function<void(int, int, char, std::set<ii>&)> fill;
    fill = [&world_experiment, isnt_loop, &fill](int r, int c, char color, std::set<ii>& visited) -> void {
      if(visited.contains(std::make_pair(r,c))) return;
      world_experiment[r][c] = color;
      visited.emplace(r, c);
      if(r>0 and isnt_loop(r-1,c)) fill(r-1, c, color, visited);
      if(r<R-1 and isnt_loop(r+1,c)) fill(r+1, c, color, visited);
      if(c>0 and isnt_loop(r,c-1)) fill(r, c-1, color, visited);
      if(c<C-1 and isnt_loop(r,c+1)) fill(r, c+1, color, visited);
    };
    std::set<ii> visited;
    for(int r=0; r<R; r++) for(int c=0; c<C; c++)
      if(world_experiment[r][c]=='x') fill(r, c, 'x', visited);
    visited.clear();
    for(int r=0; r<R; r++) for(int c=0; c<C; c++)
      if(world_experiment[r][c]=='o') fill(r, c, 'o', visited);

    std::cout << '\n';
    for(int r=0; r<R; r++)
    {
      for(int c=0; c<C; c++)
        std::cout << world_experiment[r][c];
      std::cout << '\n';
    }
    std::cout << '\n';

    ii count{0,0};
    for(int r=0; r<R; r++) for(int c=0; c<C; c++) if(world_experiment[r][c]=='x') count.first++; else if(world_experiment[r][c]=='o') count.second++;
    PRINT(count);
  }
}

//HELP
//for(int r=0; r<R; r++) for(int c=0; c<C; c++)
//
/*
 *  | is a vertical pipe connecting north and south.
    - is a horizontal pipe connecting east and west.
    L is a 90-degree bend connecting north and east.
    J is a 90-degree bend connecting north and west.
    7 is a 90-degree bend connecting south and west.
    F is a 90-degree bend connecting south and east.
 *
 */

//163 is too low
//220 is too low
//500 is too high
//350 is not the right answer
//300 is not the right answer
//271 (or was it 273?) is the answer for someone else
//269 is the answer for someone else
//
//316 is not the right answer
//317?
