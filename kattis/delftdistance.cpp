#include <bits/stdc++.h>

// maximum number of rows and columns of buildings
constexpr int HMAX{700};
constexpr int WMAX{700};
// side length of square buildings and diameter of round towers in meters
constexpr int L{10};

namespace
{
  struct p
  {
    int x, y;
    p() : x(0), y(0) {}
    p(int _x, int _y) : x(_x), y(_y) {}
    p(const p &other) : x(other.x), y(other.y) {}
    bool operator==(const p &other) const { return x==other.x && y==other.y; }
  };
}

namespace std
{
  template<> struct hash<::p>
  {
    size_t operator()(const ::p &p) const
    {
      assert(0 <= p.x && p.x <= HMAX*L);
      assert(0 <= p.y && p.y <= WMAX*L);
      // 7000 is the max value, which has 4 digits, FIXME detect #digits from constants
      // hash by shifting one of the coordinates and having both in a decimal number
      // example: x = 525, y=32 -> hash = 5250032
      return p.x*10000 + p.y;
    }
  };
}

using vertex_t = ::p;

struct edge_dest_t
{
  vertex_t u;
  double dist;

  edge_dest_t(vertex_t _u, double _d) : u(_u), dist(_d) {}

  bool operator>(const edge_dest_t &other) const
  {
    return this->dist>other.dist;
  }
};

void build_graph();
double shortest_path();

int h, w;
std::vector<std::string> world;

int main()
{
  const ::p p0(0,0);

  std::cin >> h >> w;
  std::string str;
  std::getline(std::cin, str);
  while(std::getline(std::cin, str))
    world.push_back(str);
  
  build_graph();
  std::printf("%f\n", shortest_path());
}

std::unordered_map<vertex_t, std::unordered_map<vertex_t, double>> graph;

void add_edge(const ::vertex_t &from, const ::vertex_t &to, double dist)
{
  //TODO for debugging could help to verify that the Euclidean distance between from and to is equal to dist.
  static bool toggle{false};

  if(graph.find(from) != graph.cend())
  {
    graph.at(from).insert({to, dist});
  }
  else
  {
    graph.insert({from, std::unordered_map<vertex_t, double>({{to, dist}})});
  }

  if(!toggle)
  {
    toggle = true;
    add_edge(to, from, dist);
  }
  else
  {
    toggle = false;
  }
}

void build_graph()
{
  assert(L%2 == 0);
  /// top
  for(int wi{1}; wi<=w; wi++)
  {
    add_edge(vertex_t(0, (wi-1)*L), vertex_t(0, (wi-1)*L+L/2), L/2);
    add_edge(vertex_t(0, wi*L-L/2), vertex_t(0, wi*L), L/2);
  }
  /// left
  for(int hi{1}; hi<=h; hi++)
  {
    add_edge(vertex_t((hi-1)*L, 0), vertex_t((hi-1)*L+L/2, 0), L/2);
    add_edge(vertex_t(hi*L-L/2, 0), vertex_t(hi*L, 0), L/2);
  }
  for(int hi{1}; hi<=h; hi++)
  {
      for(int wi{1}; wi<=w; wi++)
      {
          assert(hi<=int(world.size()));
          assert(wi<=int(world[hi-1].length()));
          
          /// bottom
          add_edge(vertex_t(hi*L, (wi-1)*L), vertex_t(hi*L, (wi-1)*L+L/2), L/2);
          add_edge(vertex_t(hi*L, wi*L-L/2), vertex_t(hi*L, wi*L), L/2);
          /// right
          add_edge(vertex_t((hi-1)*L, wi*L), vertex_t((hi-1)*L+L/2, wi*L), L/2);
          add_edge(vertex_t(hi*L-L/2, wi*L), vertex_t(hi*L, wi*L), L/2);
      }
  }
  for(int hi{1}; hi<=h; hi++)
  {
    for(int wi{1}; wi<=w; wi++)
    {
        if(world[hi-1][wi-1]=='O')
        {
            /// first quadrant
            add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, wi*L),     3.1415926536*L/4.);
            /// second quadrant
            add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, (wi-1)*L), 3.1415926536*L/4.);
            /// third quadrant
            add_edge(vertex_t(hi*L-L/2, (wi-1)*L), vertex_t(hi*L, wi*L-L/2),     3.1415926536*L/4.);
            /// fourth quadrant
            add_edge(vertex_t(hi*L, wi*L-L/2),     vertex_t(hi*L-L/2, wi*L),     3.1415926536*L/4.);
        }
    }
  }
}

double shortest_path()
{
  std::priority_queue<edge_dest_t, std::vector<edge_dest_t>, std::greater<edge_dest_t>> pq;

  // Create a container for distances
  // and initialize all to infinity.
  std::unordered_map<vertex_t, double> dist;
  for(const auto &[u, ignore] : graph)
    dist.insert(std::make_pair(u, std::numeric_limits<double>::max()));

  // Initialize queue with starting vertex
  // (top-left corner at distance 0.0).
  pq.emplace(vertex_t(0,0), 0.0);
  dist.at(vertex_t(0,0)) = 0.0;

  while(!pq.empty())
  {
    const vertex_t u{pq.top().u};
    pq.pop();

    for(const auto &[v, weight] : graph.at(u))
    {
      if(dist.at(v) > dist.at(u) + weight)
      {
        dist.at(v) = dist.at(u) + weight;
        pq.push(edge_dest_t(v, dist[v]));
      }
    }
  }

  return dist.at(vertex_t(L*h, L*w));
}
