#include <bits/stdc++.h>

void read_input();
std::pair<int, int> ans;
void solve();

std::pair<long long, long long> const limits{200000000000000, 400000000000000};

int main()
{
  read_input();
  solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

struct Point {
  double x, y;
  explicit Point(double x=0, double y=0) : x(x), y(y) {}
  Point operator+(Point p) const { return Point(x+p.x, y+p.y); }
  Point operator-(Point p) const { return Point(x-p.x, y-p.y); }
  Point operator*(double d) const { return Point(x*d, y*d); }
  Point operator/(double d) const { return Point(x/d, y/d); }
  double cross(Point p) const { return x*p.y - y*p.x; }
  double cross(Point a, Point b) const { return (a-*this).cross(b-*this); }
};

std::vector<Point> positions;
std::vector<Point> velocities;

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    auto const at_pos = line.find('@');
    auto const s1 = line.substr(0, at_pos);
    long long px, py, pz;
    char comma;
    std::stringstream ss1(s1);
    ss1 >> px >> comma >> py >> comma >> pz;
    positions.emplace_back(px, py);
    auto const s2 = line.substr(at_pos+1);
    int vx, vy, vz;
    std::stringstream ss2(s2);
    ss2 >> vx >> comma >> vy >> comma >> vz;
    velocities.emplace_back(vx, vy);
  }
}

std::pair<int, Point> find_intersection(std::pair<Point, Point> lhs, std::pair<Point, Point> rhs)
{
  auto const d = (lhs.second - lhs.first).cross(rhs.second - rhs.first);
  if(d == 0) return {-(lhs.first.cross(lhs.second, rhs.second) == 0), Point(0,0)};
  auto const p = rhs.first.cross(lhs.second, rhs.second), q = rhs.first.cross(rhs.second, lhs.first);
  return {1, (lhs.first * p + lhs.second * q) / d};
}

void solve()
{
  int const n = static_cast<int>(positions.size());
  for(int i = 0; i < n; i++)
    for(int j = i+1; j < n; j++){
      auto const r = find_intersection({positions.at(i), positions.at(i)+velocities.at(i)*1e9},
                                       {positions.at(j), positions.at(j)+velocities.at(j)*1e9});

      if(r.first == 0) continue;

      if(not((r.second.x-positions.at(i).x)/velocities.at(i).x < 0 or 
             (r.second.y-positions.at(j).y)/velocities.at(j).y < 0))
        if(not(r.second.x < limits.first or r.second.x > limits.second or
               r.second.y < limits.first or r.second.y > limits.second))
          ans.first++;
    }
}
