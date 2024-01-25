#include <algorithm> // C++23's std::ranges::fold_left
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define   FOR(i, a, b)    for (decltype(b) i = (a) ; i < (b) ; ++i)
#define   REP(i, n)       FOR(i, 0, n)
#define   RFOR(i, b, a)   for (decltype(a) i = (b) ; i > (a) ; --i)
#define   RREP(i, n)      RFOR(i, n, 0)

struct point { int x, y, z; point(int x, int y, int z) : x(x), y(y), z(z) {} };

bool operator==(point const& p, point const& q)
{
  return p.x == q.x and p.y == q.y and p.z == q.z;
}

using std::pair;
using std::vector;

using std::ranges::count_if;
using std::ranges::fold_left;
using std::ranges::sort;

using brick = pair<point, point>;

vector<brick> getinput();
pair<int, int> solve(vector<brick>&);

int main()
{
  auto bricks = getinput();
  const auto ans = solve(bricks);
  std::cout << "Part one: " << ans.first << "\n";
  std::cout << "Part two: " << ans.second << "\n";
}

//getinput helper
namespace { auto get_coords(std::string s) -> point; }

auto getinput() -> vector<brick>
{
  vector<brick> bricks;
  std::string line;
  while(std::getline(std::cin, line))
  {
    const auto tilde_pos = line.find('~');
    bricks.emplace_back(::get_coords(line.substr(0, tilde_pos)), ::get_coords(line.substr(tilde_pos+1)));
  }
  return bricks;
}

namespace {

auto get_coords(std::string s) -> point
{
  auto const comma_poss = std::make_pair(s.find_first_of(','), s.find_last_of(','));
  vector<pair<int, int>> vii{{0,comma_poss.first},
                             {comma_poss.first+1, comma_poss.second-comma_poss.first-1},
                             {comma_poss.second+1, (s.length()-1)-(comma_poss.second+1)+1}};
  vector<int> vi;
  std::for_each(vii.cbegin(), vii.cend(),
                [&vi, &s = std::as_const(s)](pair<int, int> p)
                {
                  vi.push_back(std::stoi(s.substr(p.first, p.second)));
                });
  return point(vi[0], vi[1], vi[2]);
}

} // anonymous

bool is_supported(brick b, vector<brick> const& bricks){
  // Transform brick to its points.
  vector<point> to_check;
  if(b.first.x != b.second.x){
    FOR(x, b.first.x, b.second.x+1) to_check.emplace_back(x, b.first.y, b.first.z);
  }else if(b.first.y != b.second.y){
    FOR(y, b.first.y, b.second.y+1) to_check.emplace_back(b.first.x, y, b.first.z);
  }else{
    // Since support is checked along Z-axis, for bricks in vertical only the
    // bottom point is interesting (the rest "are supported by the brick itself").
    to_check.emplace_back(b.first.x, b.first.y, std::min(b.first.z, b.second.z));
  }

  // Check if any of the points is supported.
  for(point const& p : to_check){
    if(p.z == 1) return true;
    for(brick const& bi : bricks) {
      if(bi.first.x != bi.second.x){
        if(bi.first.y != p.y or bi.first.z != p.z-1) continue;
        FOR(x, bi.first.x, bi.second.x+1) if(p.x == x) return true;
      }else if(bi.first.y != bi.second.y){
        if(bi.first.x != p.x or bi.first.z != p.z-1) continue;
        FOR(y, bi.first.y, bi.second.y+1) if(p.y == y) return true;
      }else{
        if(bi.first.x != p.x or bi.first.y != p.y) continue;
        FOR(z, bi.first.z, bi.second.z+1) if(z == p.z-1) return true;
      }
    }
  }
  return false;
}

void gravity(vector<brick>& bricks){
  std::ranges::sort(bricks, [](brick const& lhs, brick const& rhs){ return lhs.first.z < rhs.first.z; });
  vector<brick> lower_bricks;
  for(brick& b : bricks){
    while(!is_supported(b, lower_bricks)) b.first.z--, b.second.z--;
    lower_bricks.push_back(b);
  }
}

bool disintegrable(brick b, vector<brick> const& bricks)
{
  vector<brick> lower_bricks;
  for(brick const& to_check : bricks){
    if(to_check == b) continue;
    if(!is_supported(to_check, lower_bricks)) return false;
    lower_bricks.push_back(to_check);
  }
  return true;
}

int chainsize(brick b, vector<brick> const& bricks)
{
  vector<brick> lower_bricks;
  int ans = 0;
  for(brick const& to_check : bricks){
    if(to_check == b) continue;
    if(!is_supported(to_check, lower_bricks)) ans += 1;
    else lower_bricks.push_back(to_check);
  }
  return ans;
}

pair<int, int> solve(vector<brick>& bricks)
{
  gravity(bricks);
  return std::make_pair(count_if(bricks,
                                 [&bricks](brick b){ return disintegrable(b, bricks);}),
                        fold_left(bricks, 0,
                                  [&bricks](int n, brick b){ return n+chainsize(b, bricks);}));
}
