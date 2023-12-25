#include <bits/stdc++.h>

std::vector<std::string> split(std::string const& str, char sep)
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

using graph_t = std::unordered_map<std::string, std::unordered_set<std::string>>;

graph_t read_input();
int solve(graph_t& graph);

int main()
{
  auto graph = read_input();
  int ans = solve(graph);
  std::cout << "Multiplying the sizes of the two groups produces " << ans << ".\n";
}


graph_t read_input()
{
  graph_t graph;

  auto insert = [](graph_t& graph, const std::string& u, const std::string& v){
    if(!graph.contains(u))
      graph.insert(std::make_pair(u, std::unordered_set{v}));
    else
      graph.at(u).insert(v);
  };

  std::string line;
  while(std::getline(std::cin, line)){
    const std::string u = line.substr(0, 3);
    const std::string rest = line.substr(5);
    const std::vector<std::string> rest_split = split(rest, ' ');
    for(std::string const& v : rest_split){
      insert(graph, u, v);
      insert(graph, v, u);
    }
  }

  return graph;
}

bool is_reachable(const graph_t& graph, const std::string& u, const std::string& v, std::unordered_set<std::string>& visited)
{
  if(visited.contains(u)) return false;
  visited.insert(u);

  for(const std::string& w : graph.at(u))
    if(w == v or is_reachable(graph, w, v, visited))
      return true;

  return false;
}

bool is_reachable(const graph_t& graph, const std::string& u, const std::string& v)
{
  std::unordered_set<std::string> visited;
  return is_reachable(graph, u, v, visited);
}

int count_reachable(const graph_t& graph, const std::string& u)
{
  int ans = 0;
  for(const auto& kv : graph)
    if(u == kv.first or is_reachable(graph, u, kv.first)) ans++;

  return ans;
}

int solve(graph_t& graph)
{
  graph.at("cfn").erase("jkn");
  graph.at("jkn").erase("cfn");

  graph.at("sfd").erase("ljm");
  graph.at("ljm").erase("sfd");

  graph.at("rph").erase("gst");
  graph.at("gst").erase("rph");


  return count_reachable(graph, "cfn") * count_reachable(graph, "jkn");
}
