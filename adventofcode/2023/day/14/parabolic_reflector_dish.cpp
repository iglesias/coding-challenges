#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
//#include <map>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

void read_input(std::istream& input);
void solve();

namespace {

static void main(benchmark::State& state)
{
  // Setup
  for (auto _ : state) {
    std::ifstream input("./parabolic_reflector_dish_input.txt");
    read_input(input);
    // SUB
    solve();
  }
}

}  // anonymous

BENCHMARK(::main);
BENCHMARK_MAIN();

std::vector<std::string> m;
int R, C;

void read_input(std::istream& input)
{
  R = C = 0;
  m.clear();
  std::string line;
  while(std::getline(input, line))
  {
    m.push_back(line);
    R++;
    C = (int)line.length();
  }
}

long long cycle_number;

void cycle();

int try_wrap(std::unordered_map<size_t, long long>& cache)
{
  size_t h = std::hash<std::string>{}(m[0]);
  for(int r=1; r<R; r++) h ^= std::hash<std::string>{}(m[r]);
  if(!cache.contains(h))
  {
    cache.insert(std::make_pair(h, cycle_number));
    return -1;
  }
  for(int i = 0; i < (1000000000-cycle_number)%cache.at(h); i++) cycle();
  int ans = 0;
  for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans += (R-r);
  return ans;
}

void tilt_north()
{
  for(int r=0;r<R;r++) for(int c=0;c<C;c++)
    if(m[r][c]=='O' and r>0 and m[r-1][c]=='.')
    {
      int dr=r-1;
      while(dr > 0 and m[dr-1][c] == '.') dr -= 1;
      m[r][c] = '.';
      m[dr][c] = 'O';
    }
}

void tilt_west()
{
  for(int r=0;r<R;r++) for(int c=0;c<C;c++)
    if(m[r][c]=='O' and c>0 and m[r][c-1]=='.')
    {
      int dc=c-1;
      while(dc > 0 and m[r][dc-1] == '.') dc -= 1;
      m[r][c] = '.';
      m[r][dc] = 'O';
    }
}

void tilt_south()
{
  for(int r=R-1;r>=0;r--) for(int c=0;c<C;c++)
    if(m[r][c]=='O' and r<R-1 and m[r+1][c]=='.')
    {
      int dr=r+1;
      while(dr < R-1 and m[dr+1][c] == '.') dr += 1;
      m[r][c] = '.';
      m[dr][c] = 'O';
    }
}

void tilt_east()
{
  for(int r=0;r<R;r++) for(int c=C-1;c>=0;c--)
    if(m[r][c]=='O' and c<C-1 and m[r][c+1]=='.')
    {
      int dc=c+1;
      while(dc < C-1 and m[r][dc+1] == '.') dc += 1;
      m[r][c] = '.';
      m[r][dc] = 'O';
    }
}

void cycle()
{
  tilt_north();
  tilt_west();
  tilt_south();
  tilt_east();
}

void solve()
{
  std::pair<int, int> ans{0, 0};
  tilt_north();
  for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans.first += (R-r);
  tilt_west();
  tilt_south();
  tilt_east();
  cycle_number = 1;

  std::unordered_map<size_t, long long> cache;
  for(long long i=0; i<1000000000; i++)
  {
    cycle();
    cycle_number++;
    if ((ans.second = try_wrap(cache)) > 0) break;
  }
  //std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
  if (ans.first != 106997 or ans.second != 99641) throw std::exception();
}
