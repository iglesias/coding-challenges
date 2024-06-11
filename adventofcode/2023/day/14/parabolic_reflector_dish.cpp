#include <exception>
#include <fstream>
#include <functional>
#include <istream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

#include "splay_tree_map.hpp"

std::tuple<std::vector<std::string>, int, int> read_input(std::istream& input);
template<template<typename ...> class map_t> void solve(const std::vector<std::string>&, int, int);

namespace {

template<template<typename ...> class map_t> static void main(benchmark::State& state)
{
  // Setup
  std::ifstream input("./parabolic_reflector_dish_input.txt");
  const auto& [m, R, C] = read_input(input);
  for (auto _ : state) {
    // SUB
    solve<map_t>(m, R, C);
  }
}

}  // anonymous

BENCHMARK(::main<std::map>);
BENCHMARK(::main<std::unordered_map>);
BENCHMARK(::main<dts::splay_tree::map>);
BENCHMARK_MAIN();

std::tuple<std::vector<std::string>, int, int> read_input(std::istream& input)
{
  std::vector<std::string> m;
  int R = 0, C = 0;
  std::string line;
  while(std::getline(input, line))
  {
    m.push_back(line);
    R++;
    C = (int)line.length();
  }

  return {m, R, C};
}

void cycle(std::vector<std::string>& m, int R, int C);

template<template<typename ...> class map_t>
int try_wrap(map_t<size_t, long long>& cache,
             std::vector<std::string>& m, int R, int C,
             const long long cycle_number)
{
  size_t h = std::hash<std::string>{}(m[0]);
  for(int r=1; r<R; r++) h ^= std::hash<std::string>{}(m[r]);
  if(!cache.contains(h))
  {
    cache.emplace(h, cycle_number);
    return -1;
  }
  for(int i = 0; i < (1000000000-cycle_number)%cache.at(h); i++) cycle(m, R, C);
  int ans = 0;
  for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans += (R-r);
  return ans;
}

void tilt_north(std::vector<std::string>& m, int R, int C)
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

void tilt_west(std::vector<std::string>& m, int R, int C)
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

void tilt_south(std::vector<std::string>& m, int R, int C)
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

void tilt_east(std::vector<std::string>& m, int R, int C)
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

void cycle(std::vector<std::string>& m, int R, int C)
{
  tilt_north(m, R, C);
  tilt_west(m, R, C);
  tilt_south(m, R, C);
  tilt_east(m, R, C);
}

template<template<typename ...> class map_t>
void solve(const std::vector<std::string>& input_m, int R, int C)
{
  std::pair<int, int> ans{0, 0};
  std::vector<std::string> m = input_m; // copy
  tilt_north(m, R, C);
  for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans.first += (R-r);
  tilt_west(m, R, C);
  tilt_south(m, R, C);
  tilt_east(m, R, C);
  long long cycle_number = 1;

  map_t<size_t, long long> cache;
  for(long long i=0; i<1000000000; i++)
  {
    cycle(m, R, C);
    cycle_number++;
    if ((ans.second = try_wrap(cache, m, R, C, cycle_number)) > 0) break;
  }
  //std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
  if (ans.first != 106997 or ans.second != 99641) throw std::exception();
}
