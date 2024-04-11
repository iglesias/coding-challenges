#include <bitset>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct digcomm {
  char dir;
  int  len;
  std::string col;
};

std::vector<digcomm> read_input();
int solve_part_one(std::vector<digcomm> const& input);
void convert(std::vector<digcomm> const& input);

int main() {
  auto const input = read_input();
  std::cout << "Part one: " << solve_part_one(input) << '\n';
  convert(input);
}

std::vector<digcomm> read_input() {
  std::vector<digcomm> input;
  std::string line;
  while(std::getline(std::cin, line)) {
    std::stringstream ss(line);
    char dir;
    std::string len, col;
    ss >> dir >> len >> col;
    input.emplace_back(dir, std::stoi(len), std::move(col));
  }
  return input;
}

using ii = std::pair<int, int>;
using TopLeft = ii;
using BotRight = ii;
using grid = std::vector<std::vector<char>>;

ii operator+(ii const& p0, ii const& p1) {
  return {p0.first + p1.first, p0.second + p1.second};
}

ii operator+=(ii& p0, ii const& p1) {
  p0 = p0 + p1;
  return p0;
}

auto fillTrenchesAndGetCorners(grid& G, std::vector<digcomm> const& input) -> std::pair<TopLeft, BotRight>
{
  std::map<char, ii> const dir2delta{
    {'R', {0,+1}}, {'D', {+1,0}}, {'L', {0,-1}}, {'U', {-1,0}},
  };

  int const RMAX = static_cast<int>(G.size());
  int const CMAX = static_cast<int>(G.at(0).size());

  ii p{RMAX/2,CMAX/2};
  ii topLeft{RMAX-1, CMAX-1};
  ii botRight{0,0};

  for(auto const& dc : input){
    auto delta = dir2delta.at(dc.dir);
    for(int i = 0; i < dc.len; i++){
      p += delta;
      G.at(p.first).at(p.second) = '#';

      topLeft.first = std::min(topLeft.first, p.first);
      topLeft.second = std::min(topLeft.second, p.second);
      botRight.first = std::max(botRight.first, p.first);
      botRight.second = std::max(botRight.second, p.second);
    }
  }

  return {topLeft, botRight};
}

//FIXME extract from input. Maximum dimensions to
//initialize trench and level terrain grid.
int const RMAX=501, CMAX=501;

void fill(std::pair<int, int> const& p, grid& G) {
  assert(G[p.first][p.second] != '#');
  static std::bitset<RMAX * CMAX> visited;
  visited.set(p.first * CMAX, + p.second);
  assert(G[p.first][p.second] == '.');
  G[p.first][p.second] = 'x';
  static const std::vector<ii> deltas{{0, +1}, {+1, 0}, {0, -1}, {-1, 0}};
  for(auto const& delta : deltas) {
    std::pair<int, int> const q{p + delta};
    assert(0 <= q.first and q.first < RMAX and 0 <= q.second and q.second < CMAX);
    if(!visited.test(q.first*CMAX + q.second) and G[q.first][q.second] == '.')
      fill(q, G);
  }
}

int solve_part_one(std::vector<digcomm> const& input) {

  auto G = std::vector(RMAX, std::vector(CMAX, '.'));
  auto const [topLeft, botRight] = fillTrenchesAndGetCorners(G, input);

  ii x; int min_column = CMAX-1;
  for(int r = 0; r<RMAX; r++) for(int c = 0; c<CMAX; c++)
    if(G[r][c] == '#')
      if(c < min_column){
        min_column = std::min(min_column, c);
        x = {r,c};
      }

  ii start{x.first+1, x.second+1};
  fill(start, G);

  int ans{0};
  for(int r=topLeft.first; r<=botRight.first; r++)
    for(int c=topLeft.second; c<=botRight.second; c++)
      if(G[r][c] == 'x' or G[r][c] == '#') ans++;
  return ans;
}

char digitCharToDirection(char c)
{
  switch(c){
    case '0': return 'R';
    case '1': return 'D';
    case '2': return 'L';
    case '3': return 'U';
  }
  assert(false);
}

// color -> instruction parameters
void convert(std::vector<digcomm> const& input)
{
  for(auto const& dc : input){
    int decval;
    std::stringstream stringstream;
    std::string hexval = dc.col.substr(2, dc.col.length()-4);
    stringstream << hexval;
    stringstream >> std::hex >> decval;
    std::cout << digitCharToDirection(dc.col.at(dc.col.length()-2)) << ' ' << decval << std::endl;
  }
}

std::ostream& operator<<(std::ostream& os, digcomm const& dc) {
  os << "digcomm(";
  os << dc.dir << ' ' << std::setw(2) << dc.len << ' ';
  os << dc.col;
  os << ")";
  return os;
}
