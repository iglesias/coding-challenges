#include <bits/stdc++.h>

using integer = char;

enum material { ORE=0, CLA, OBS, GEO, M }; // M is the number of materials

std::vector<std::array<std::array<integer, M>, M>> COSTS;
// index cost with blueprint, robot, material

void print_costs();
void read_input();
integer maximize_blueprint(int b, int num_minutes);

int main()
{
  read_input();
  //print_costs();
  const int num_minutes{32};
  //const int num_minutes{24}; // part one
  int ans{0};
  for(size_t b{0}; b<COSTS.size(); b++)
  {
    integer num_geodes = maximize_blueprint(b, num_minutes);
    std::cout << "Blueprint " << (b+1) << " num_geodes=" << int(num_geodes) << '\n';
    ans += (b+1)*num_geodes;
  }
  std::cout << "ans=" << ans << '\n';
}

std::array<integer, M> max_costs;
std::array<std::array<integer, M>, M> costs;

struct state
{
  std::array<integer, M> robots;
  std::array<integer, M> materials;
  int minute;
  state()
  {
    robots = {1, 0, 0, 0};
    materials  = {0, 0, 0, 0};
    minute = 0;
  }
  state(const state &other)
  {
    robots = other.robots;
    materials  = other.materials;
    minute = other.minute;;
  }
  bool operator==(const state &other) const
  {
    bool diff_found{false};
    for(int m{0}; m<M && !diff_found; m++)
    {
      diff_found = diff_found or (robots[m] != other.robots[m])
                      or (materials[m] != other.materials[m]);
    }
    return !diff_found;
  }
  std::vector<state> expand() const
  {
    std::vector<state> e;

    for(int r{M-1}; r>=0; r--)
    {
      // heuristic. FIXME
      if(r<M-1 && materials[r]>=max_costs[r]+2) continue;

      bool have_material{true};
      for(int m{0}; m<M && have_material; m++)
        have_material = have_material && costs[r][m] <= materials[m];

      if(have_material)
      {
        state n(*this);
        n.robots[r]++;
        for(int m{0}; m<M; m++)
          n.materials[m] -= costs[r][m];
        e.push_back(n);
      }
    }

    // expand includes a no buy state
    e.push_back(*this);
    return e;
  }
};

namespace std
{
  template<> struct hash<state>
  {
    size_t operator()(const state &s) const
    {
      std::string str;
      for(int m{0}; m<M; m++)
      {
        str += s.robots[m];
        str += s.materials[m];
      }

      return std::hash<std::string>{}(str);
    }
  };
}

void compute_max_costs(int b)
{
  for(int m{0}; m<M; m++)
  {
    max_costs[m] = 0;
    for(int r{0}; r<M; r++)
    {
      max_costs[m] = std::max(max_costs[m], COSTS[b][r][m]);
    }
  }
}

integer maximize_blueprint(int b, int num_minutes)
{
  compute_max_costs(b);
  costs = COSTS[b];
  state start;
  std::queue<state> q;
  q.push(start);
  integer ans{0};
  std::unordered_set<state> visited;
  while(!q.empty())
  {
    state s = q.front();
    q.pop();

    if(s.minute==num_minutes)
    {
//      std::cout << ">> Reached minute " << num_minutes << "!\n";
//      std::cout << s.robots[ORE] << " ore-collecting robots\n";
//      std::cout << s.materials[ORE] << " ore\n";
      ans = std::max(ans, s.materials[GEO]);
      continue;
    }

    if((num_minutes-s.minute)*std::max(static_cast<integer>(1), s.robots[GEO])+s.materials[GEO]<=ans)
      continue;

    if(visited.find(s)!=visited.cend()) continue;
    visited.insert(s);

    /* Timeline per minute */
    // Start building new robots, available robots collect materials,
    // new robots finish building.

    //I've switched the timeline so that
    //we don't need to keep track of robots started
    //to build and finish those at the end.

    // Start building new robots.
    // New robots are finished building.
    std::vector<state> e = s.expand();

    for(state ei: e)
    {
      // Available robots (s.robots rather than ei.robots) collect materials.
      for(integer m{0}; m<M; m++)
        ei.materials[m] += s.robots[m];
      ei.minute++;
      q.push(ei);
    }
  }

  return ans;
}

void read_input()
{
  int blueprint_1idx{0};
  std::string line;
  while(std::getline(std::cin, line))
  {
    if(blueprint_1idx==3) return; // part two
    blueprint_1idx++;
    COSTS.emplace_back();
    std::istringstream ss(line);
    int d;

    std::string Blueprint;
    char colon;
    ss >> Blueprint >> d >> colon;
    assert(Blueprint=="Blueprint");
    assert(colon==':');
    assert(d==blueprint_1idx);

    std::string Each, X, robot, costs, ore;
    ss >> Each >> X >> robot >> costs >> d >> ore;
    assert(Each=="Each");
    assert(X=="ore");
    assert(robot=="robot");
    assert(costs=="costs");
    assert(ore=="ore.");

    COSTS[blueprint_1idx-1][ORE][ORE] = d;

    ss >> Each >> X >> robot >> costs >> d >> ore;
    assert(Each=="Each");
    assert(X=="clay");
    assert(robot=="robot");
    assert(costs=="costs");
    assert(ore=="ore.");

    COSTS[blueprint_1idx-1][CLA][ORE] = d;

    int e;
    std::string And, clay;
    ss >> Each >> X >> robot >> costs >> d >> ore >> And >> e >> clay;
    assert(Each=="Each");
    assert(X=="obsidian");
    assert(robot=="robot");
    assert(costs=="costs");
    assert(ore=="ore");
    assert(And=="and");
    assert(clay=="clay.");

    COSTS[blueprint_1idx-1][OBS][ORE] = d;
    COSTS[blueprint_1idx-1][OBS][CLA] = e;

    std::string obsidian;
    ss >> Each >> X >> robot >> costs >> d >> ore >> And >> e >> obsidian;
    assert(Each=="Each");
    assert(X=="geode");
    assert(robot=="robot");
    assert(costs=="costs");
    assert(ore=="ore");
    assert(And=="and");
    assert(obsidian=="obsidian.");

    COSTS[blueprint_1idx-1][GEO][ORE] = d;
    COSTS[blueprint_1idx-1][GEO][OBS] = e;
  }
}

void print_costs()
{
  for(int b{0}; b < int(COSTS.size()); b++)
  {
    std::cout << "Blueprint " << b+1 << ":\n";
    for(int r{0}; r<M; r++)
    {
      std::cout << "  robot " << r << " costs ";
      for(int m{0}; m<M; m++)
      {
        std::cout << COSTS[b][r][m] << ((m == M-1) ? '\n' : ' ');
      }
    }
    std::cout << std::endl;
  }
}
