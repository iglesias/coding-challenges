#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int N{24};

using world = std::array<std::array<std::array<bool, N>, N>, N>;

namespace
{
  struct p {
    p(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    p() : x(0), y(0), z(0) {}
    int x, y, z;
  };
}

world droplet_world;
world fluid_reachable;
std::vector<::p> cubes;

int part_two();

int main()
{
  std::string line;
  ::p max(0,0,0), min(N-1,N-1,N-1);
  while(std::getline(std::cin, line)) {
    int x, y, z;
    char c1, c2;
    std::istringstream ss(line);
    ss >> x >> c1 >> y >> c2 >> z;
    // shift +1 to deal with the input cube with z=0,
    // in this way there will be space in the world to
    // simulate flood from outside the droplet of cubes
    assert(x+1>0 && x<N-1);
    assert(y+1>0 && y<N-1);
    assert(z+1>0 && z<N-1);

    max.x = std::max(max.x, x+1);
    max.y = std::max(max.y, y+1);
    max.z = std::max(max.z, z+1);

    min.x = std::min(min.x, x+1);
    min.y = std::min(min.y, y+1);
    min.z = std::min(min.z, z+1);

    droplet_world[x+1][y+1][z+1] = true;
    cubes.emplace_back(x+1,y+1,z+1);
  }

  // maximum, minimum cubes coordinates
  // difference to get an idea of the N needed, this is affecting whether the program
  // can finish the recursion in my laptop
  std::cout << ">> max=(" << max.x << ", " << max.y << ", " << max.z << ")\n";
  std::cout << ">> min=(" << min.x << ", " << min.y << ", " << min.z << ")\n";
  std::cout << ">> diff=(" << max.x-min.x << ", " << max.y-min.y << ", " << max.z-min.z << ")\n";

  int part_one_ans{0};
  for(const auto &a : cubes) {
    if(!droplet_world[a.x-1][a.y][a.z]) part_one_ans++;
    if(!droplet_world[a.x+1][a.y][a.z]) part_one_ans++;
    if(!droplet_world[a.x][a.y-1][a.z]) part_one_ans++;
    if(!droplet_world[a.x][a.y+1][a.z]) part_one_ans++;
    if(!droplet_world[a.x][a.y][a.z-1]) part_one_ans++;
    if(!droplet_world[a.x][a.y][a.z+1]) part_one_ans++;
  }

  int part_two_ans{part_two()};
  //int part_two_ans{};

  std::cout << "Part One: " << part_one_ans << 
             "\nPart Two: " << part_two_ans << '\n';
}

int part_two()
{
  auto within_bounds = [](const ::p &p) -> bool {
    return p.x>=0 && p.x<N && p.y>=0 && p.y<N && p.z>=0 && p.z<N;
  };

  std::function<void(int, int, int)> flood = [&](int x, int y, int z) {
  //  std::cout << ">> flood(" << x << ", " << y << ", " << z << ")\n";
    assert(within_bounds(::p(x,y,z)));
    if(droplet_world[x][y][z]) return;
    if(fluid_reachable[x][y][z]) return;

    fluid_reachable[x][y][z] = true;

    std::vector<::p> deltas = {{+1,0,0}, {-1,0,0},{0,+1,0} ,{0,-1,0},{0,0,+1},{0,0,-1}};

    for(const auto &delta : deltas) {
      ::p trypos{x+delta.x, y+delta.y, z+delta.z};
      if(within_bounds(trypos))
        flood(trypos.x, trypos.y, trypos.z);
    }
  };

  for(int x{0}; x<N; x++) for(int y{0}; y<N; y++) {
      flood(x,y,0);
//      flood(x,y,N-1); // not necessary? and didn't change, but seems intuitive
  }

  for(int y{0}; y<N; y++) for(int z{0}; z<N; z++) {
      flood(0,y,z);
//      flood(N-1,y,z); // not necessary?
  }

  for(int x{0}; x<N; x++) for(int z{0}; z<N; z++) {
      flood(x,0,z);
//      flood(x,N-1,z); // not necessary?
  }

  int part_two_ans{0};
  for(const auto &a : cubes) {
    if(!droplet_world[a.x-1][a.y][a.z] && fluid_reachable[a.x-1][a.y][a.z]) part_two_ans++;
    if(!droplet_world[a.x+1][a.y][a.z] && fluid_reachable[a.x+1][a.y][a.z]) part_two_ans++;
    if(!droplet_world[a.x][a.y-1][a.z] && fluid_reachable[a.x][a.y-1][a.z]) part_two_ans++;
    if(!droplet_world[a.x][a.y+1][a.z] && fluid_reachable[a.x][a.y+1][a.z]) part_two_ans++;
    if(!droplet_world[a.x][a.y][a.z-1] && fluid_reachable[a.x][a.y][a.z-1]) part_two_ans++;
    if(!droplet_world[a.x][a.y][a.z+1] && fluid_reachable[a.x][a.y][a.z+1]) part_two_ans++;
  }

  return part_two_ans;
}
