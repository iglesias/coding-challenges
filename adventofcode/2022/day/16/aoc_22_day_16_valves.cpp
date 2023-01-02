#include <bits/stdc++.h>

void read_input();
void print_input();
void preprocess_input();
// part one
// moving in the complete graph with zero valves
// -> it cannot handle the graph in the puzzle input
// the sample input is small enough
void search();
// only moving in a graph with non-zero valves
// -> this has become the key to solve the puzzle input
void search_small_graph();
// part two
void team_search();

namespace {

  using valve_t = int;

  struct state
  {
    static int team_size;

    std::vector<int> minute;
    int ppm;
    std::vector<valve_t> valve;
    int value;
    int mask;

    state(const state &other) : minute(other.minute), ppm(other.ppm),
                         valve(other.valve), value(other.value),
                         mask(other.mask) {}

    state() : minute(std::vector(team_size, 0)), ppm(0), valve(std::vector(team_size, 0)), value(0), mask(0) {}

    state& operator=(const state&other)
    {
      minute = other.minute;
      ppm = other.ppm;
      valve = other.valve;
      value = other.value;
      mask = other.mask;
      return *this;
    }
  };

  int ::state::team_size{1};
}

// valve string <-> integer mappings.
std::unordered_map<std::string, int> vstr2i;
std::vector<std::string> i2vstr;

// valve integer to flow
std::unordered_map<::valve_t, int> v2f;
// adjacency list
std::unordered_map<::valve_t, std::vector<::valve_t>> al;

// lengths of the shortest paths between pairs of valves
std::vector<std::vector<size_t>> spl;
// -> using directly vectors,
// instead of std::unordered_map<::valve_t, ...>
// for simplicity since we will use this table for Floyd-Warshall

// flow to binary-shift (only for nonzero valves)
std::unordered_map<int, int> value2shift;
// -> used for keeping track the opened valves in the state's mask

std::vector<::valve_t> nonzero_valves;

int total_flow{0};

int max_minutes{30};
int main(int argc, char* argv[])
{
  if(argc>1)
    max_minutes = std::stoi(argv[1]);

  read_input();
  print_input();
  preprocess_input();
  //search();
  search_small_graph();
  team_search();
}

void read_input()
{
  // associate valve AA with valve integer i=0.
  vstr2i.insert(std::make_pair("AA", 0));
  i2vstr.push_back("AA");

  int next_shift = 0;
  std::string line;
  while(std::getline(std::cin, line))
  {
    std::istringstream ss(line);
    std::string Valve, has, flow, rate_str, tunnels, lead, to, valves;
    std::string from_valve, to_valve;

    ss >> Valve >> from_valve >> has >> flow >> rate_str;
    int flow_rate{std::stoi(rate_str.substr(rate_str.find('=')+1))};
    assert(Valve=="Valve");
    assert(has=="has");
    assert(flow=="flow");
    //std::cout << ">> " << from_valve << "(" << flow_rate << ") -> ";
    total_flow += flow_rate;
    if(flow_rate>0)
    {
      value2shift.insert(std::make_pair(flow_rate, next_shift)); 
      next_shift++;
    }

    if(vstr2i.find(from_valve) == vstr2i.cend())
    {
      vstr2i.insert(std::make_pair(from_valve, i2vstr.size()));
      i2vstr.push_back(from_valve);
    }

    assert(v2f.find(vstr2i[from_valve]) == v2f.cend());
    v2f.insert(std::make_pair(vstr2i[from_valve], flow_rate));

    ss >> tunnels >> lead >> to >> valves;
    assert(tunnels=="tunnels" or tunnels=="tunnel");
    assert(lead=="lead" or lead=="leads");
    assert(to=="to");
    assert(valves=="valves" or valves=="valve");

    std::vector<valve_t> l;
    while(!ss.eof())
    {
      ss >> to_valve;
      if (to_valve[to_valve.length()-1]==',')
      {
        to_valve.erase(to_valve.length()-1);
        //std::cout << to_valve << ' ';
      }
      else
      {
        //std::cout << to_valve << '\n';
      }

      if(vstr2i.find(to_valve) == vstr2i.cend())
      {
        vstr2i.insert(std::make_pair(to_valve, i2vstr.size()));
        i2vstr.push_back(to_valve);
      }

      l.push_back(vstr2i[to_valve]);
    }

    assert(al.find(vstr2i[from_valve]) == al.cend());
    al.insert(std::make_pair(vstr2i[from_valve], std::move(l)));
  }

  //std::cout << "\n\n";
}

void print_input()
{
  for(const auto &kv : al)
  {
    const auto &k = kv.first;
    std::cout << ">> " << i2vstr[k] << "(" << v2f[k] << ") ->";
    for(const auto &to_valve : kv.second)
    {
      std::cout << ' ' << i2vstr[to_valve];
    }
    std::cout << '\n';
  }

  for(const auto &kv : vstr2i)
  {
    std::cout << kv.first << " -> " << kv.second << '\n';
  }

  for(const auto &kv : al)
  {
    const auto &k = kv.first;
    std::cout << ">> " << k << "(" << v2f[k] << ") ->";
    for(const auto &to_valve : kv.second)
    {
      std::cout << ' ' << to_valve;
    }
    std::cout << '\n';
  }

  std::cout << "flow rate values to binary shift\n";
  for(const auto &kv : value2shift)
  {
    std::cout << kv.first << " -> " << kv.second << " " << (1<<kv.second) << '\n';
  }
  std::cout << value2shift.size() << '\n';
}

int max{0};
void dfs(::state s)
{
  // pruning
  if(max>s.value+total_flow*(max_minutes-s.minute[0]+1))
  {
    return;
  }

  s.value += s.ppm;

  if(s.minute[0]>=max_minutes)
  {
    //assert(s.minute[0]==max_minutes);
    max = std::max(max, s.value);
    return;
  }

  //assert(s.minute[0]<max_minutes);
  // open current valve?
  //assert(v2f.find(s.valve[0])!=v2f.cend());
  if(v2f[s.valve[0]]>0)
  {
    //assert(value2shift.find(v2f[s.valve[0]])!=value2shift.cend());
    //std::cout << " value2shift[" << v2f[s.valve[0]] << "]=" << value2shift[v2f[s.valve[0]]] << '\n';
    if(!(s.mask & (1<<value2shift[v2f[s.valve[0]]])))
    {
      ::state t(s);
      t.ppm += v2f.at(t.valve[0]);
      t.mask |= (1<<value2shift[v2f[t.valve[0]]]);
      //std::cout << ">> ppm=" << s.ppm << " mask=" << std::bitset<8>(s.mask) << '\n';
      t.minute[0]++;

      if(t.ppm == total_flow)
      {
        max = std::max(max, t.value + (max_minutes-s.minute[0])*t.ppm);
        return;
      }

      dfs(t);
    }
  }

  // move to other valves
  for(valve_t next_valve : al[s.valve[0]])
  {
    ::state t(s);
    t.valve[0] = next_valve;
    t.minute[0]++;
    dfs(t);
  }
}

void search()
{
  max = 0;
  ::state s;
  s.minute[0] = 1;
  dfs(s);
  std::cout << "Part one: " << max << " (slow method)\n";
}

void preprocess_input()
{
  for(auto [valve, flow] : v2f)
    if(flow>0) nonzero_valves.push_back(valve);

  const size_t N{i2vstr.size()};
  assert(N==vstr2i.size() && N==v2f.size() && N==al.size());
  assert(N>=value2shift.size());

  /// Floyd-Warshall
  // we will use the lengths of the shortest paths from the
  // starting point (valve AA) to all other *non-zero* valves,
  // and the lengths of the shortest paths between non-zero valves.

  spl = std::vector(N, std::vector(N, 1000000000ul));

  // initialize paths formed by an edge
  for(const auto &kv : al)
  {
    const ::valve_t from{kv.first};
    for(const ::valve_t to : al.at(from))
      spl[from][to] = 1u; // edges have 1 unit of length
  }

  for(size_t i{0}; i<N; i++)
    spl[i][i] = 0u; 

  for(size_t k{0}; k<N; k++)
    for(size_t i{0}; i<N; i++)
      for(size_t j{0}; j<N; j++)
        if(spl[i][j] > spl[i][k] + spl[k][j])
          spl[i][j] = spl[i][k] + spl[k][j];
}

void search_small_graph()
{
  max = 0;
  std::queue<::state> q;

  // Start with as many states as non-zero
  // valves, each of them corresponding to
  // moving from the start valve to a 
  // non-zero valve and opening it.

  for(::valve_t nonzero_valve : nonzero_valves)
  {
    ::state s{::state()};
    // Getting to the valve plus one minute opening it.
    s.minute[0] = 1 + spl[0][nonzero_valve] + 1;
    s.valve[0] = nonzero_valve;
    assert(v2f.at(nonzero_valve)>0);
    s.value = (max_minutes-s.minute[0]+1)*v2f[nonzero_valve];
    assert(value2shift.find(v2f[nonzero_valve])!=value2shift.cend());
    s.mask = 1<<value2shift[v2f[nonzero_valve]];
    q.push(s);
  }

  while(!q.empty())
  {
    ::state s{q.front()};
    q.pop();

    // Expand only to other non-zero valves, ...
    for(::valve_t nonzero_valve : nonzero_valves)
    {
      assert(v2f.at(nonzero_valve)>0);

      if(nonzero_valve==s.valve[0]) continue;

      // ... that aren't already open
      assert(value2shift.find(v2f[nonzero_valve])!=value2shift.cend());
      if(s.mask & (1<<value2shift[v2f[nonzero_valve]])) continue;

      // Is there time to open the next non-zero valve?
      if(s.minute[0]+spl[s.valve[0]][nonzero_valve]+1>static_cast<size_t>(max_minutes)) continue;

      ::state t(s);
      t.minute[0] += spl[s.valve[0]][nonzero_valve]+1;
      t.valve[0] = nonzero_valve;
      t.value += (max_minutes-t.minute[0]+1)*v2f[nonzero_valve];
      t.mask |= (1<<value2shift[v2f[nonzero_valve]]);
      q.push(t);
    }

    max = std::max(max, s.value);
  }

  std::cout << "Part one: " << max << '\n';
}

void team_search()
{
  max = 0;
  // Same approach that search_small_graph,
  // but with a extended state for you and elephant.
  ::state::team_size = 2;
  std::queue<::state> q;

  for(size_t i{0}; i<nonzero_valves.size(); i++)
    for(size_t j{i+1}; j<nonzero_valves.size(); j++)
    {
      ::state s{::state()};
      s.minute[0] = 1 + spl[0][nonzero_valves[i]] + 1;
      s.minute[1] = 1 + spl[0][nonzero_valves[j]] + 1;
      s.valve[0] = nonzero_valves[i];
      s.valve[1] = nonzero_valves[j];
      assert(v2f.at(s.valve[0])>0);
      assert(v2f.at(s.valve[1])>0);
      s.value  = (max_minutes-s.minute[0]+1)*v2f[s.valve[0]];
      s.value += (max_minutes-s.minute[1]+1)*v2f[s.valve[1]];
      assert(value2shift.find(v2f[s.valve[0]])!=value2shift.cend());
      assert(value2shift.find(v2f[s.valve[1]])!=value2shift.cend());
      s.mask  = 1<<value2shift[v2f[s.valve[0]]];
      s.mask |= 1<<value2shift[v2f[s.valve[1]]];
      q.push(s);
    }

  while(!q.empty())
  {
    ::state s{q.front()};
    q.pop();

    // pruning
    if(max>s.value+total_flow*(max_minutes-std::min(s.minute[0],s.minute[1])+1))
      continue;

    for(size_t i{0}; i<nonzero_valves.size(); i++)
      for(size_t j{0}; j<nonzero_valves.size(); j++)
      {
        if(i==j) continue;

        //assert(v2f.at(nonzero_valves[i])>0);
        //assert(v2f.at(nonzero_valves[j])>0);

        if(nonzero_valves[i]==s.valve[0]) continue;
        if(nonzero_valves[j]==s.valve[1]) continue;

        // Valves that aren't already open
        //assert(value2shift.find(v2f[nonzero_valves[i]])!=value2shift.cend());
        //assert(value2shift.find(v2f[nonzero_valves[j]])!=value2shift.cend());
        if(s.mask & (1<<value2shift[v2f[nonzero_valves[i]]])) continue;
        if(s.mask & (1<<value2shift[v2f[nonzero_valves[j]]])) continue;

        // Is there time to open the next non-zero valves?
        if(s.minute[0]+spl[s.valve[0]][nonzero_valves[i]]+1>static_cast<size_t>(max_minutes)) continue;
        if(s.minute[1]+spl[s.valve[1]][nonzero_valves[j]]+1>static_cast<size_t>(max_minutes)) continue;

        ::state t(s);
        t.minute[0] += spl[s.valve[0]][nonzero_valves[i]]+1;
        t.minute[1] += spl[s.valve[1]][nonzero_valves[j]]+1;
        t.valve[0] = nonzero_valves[i];
        t.valve[1] = nonzero_valves[j];
        t.value += (max_minutes-t.minute[0]+1)*v2f[nonzero_valves[i]];
        t.value += (max_minutes-t.minute[1]+1)*v2f[nonzero_valves[j]];
        t.mask |= (1<<value2shift[v2f[nonzero_valves[i]]]);
        t.mask |= (1<<value2shift[v2f[nonzero_valves[j]]]);
        q.push(t);
      }

    max = std::max(max, s.value);
  }

  std::cout << "Part two: " << max << '\n';
}
