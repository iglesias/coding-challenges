#include <bits/stdc++.h>

#define dbp(...) dblog(#__VA_ARGS__, __VA_ARGS__)

template<typename... Args>
void dblog(std::string vars, Args&&... values)
{
    std::cout << "[" << vars << " = ";
    std::string delim = "";
    (..., (std::cout << delim << values, delim = ", "));
    std::cout << "]\n";
}

// maximum number of rows and columns of buildings
constexpr int HMAX{700};
constexpr int YMAX{700};
// side length of square buildings and diameter of round towers in meters
constexpr int L{10};

namespace
{
  struct p
  {
    int x, y;
    p(int _x, int _y) : x(_x), y(_y) {}
    p(const p &other) : x(other.x), y(other.y) {}
    bool operator==(const p &other) const { return x==other.x && y==other.y; }
    friend std::ostream& operator<<(std::ostream &os, const p &point)
    {
      return os << "(" << point.x << ", " << point.y << ")";
    }
  };
}

namespace std
{
  template<> struct hash<::p>
  {
    size_t operator()(const ::p &p) const
    {
      assert(0 <= p.x && p.x <= HMAX*L);
      assert(0 <= p.y && p.y <= YMAX*L);
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
  float dist;

  edge_dest_t(vertex_t _u, float _d) : u(_u), dist(_d) {}

  bool operator>(const edge_dest_t &other) const
  {
    return this->dist>other.dist;
  }

  friend std::ostream& operator<<(std::ostream &os, const edge_dest_t &e)
  {
    return os << "{u=" << e.u << ", dist=" << e.dist << "}";
  }
};

void build_graph();
float shortest_path();

int h, w;
std::vector<std::string> world;

int main()
{
  dbp(std::numbers::pi);
  const ::p p0(0,0);
  const std::hash<::p> hash;
  dbp(hash(p0));
  dbp(hash(::p(7000, 7000)));
  dbp(hash(::p(4512, 1234)));
  dbp(hash(::p(525, 32)));

  std::cin >> h >> w;
  dbp(h, w);
  std::string str;
  std::getline(std::cin, str);
  while(std::getline(std::cin, str))
    world.push_back(std::move(str));
  
  build_graph();
  std::cout << std::fixed << std::setprecision(10) << shortest_path() << std::endl;
}

std::unordered_map<vertex_t, std::unordered_map<vertex_t, float>> graph;

void add_edge(const ::vertex_t &from, const ::vertex_t &to, float dist)
{
  static bool toggle{false};
  dbp(from, to);

  if(graph.contains(from))
  {
    assert(!graph.at(from).contains(to));
    graph.at(from).insert({to, dist});
  }
  else
  {
    graph.insert({from, std::unordered_map<vertex_t, float>({{to, dist}})});
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
  /* Only squares
  for(int hi{0}; hi<=h; hi++) for(int wi{0}; wi<=w;wi++)
  {
    //        |          0--.--1
    //     ---x--        |     |
    //        |          .     .
    //                   |     |
    //                   2--.--3
    //
    //
    
    const ::p vertex(hi*L, wi*L);
    // vertical edges at the bottom of vertex
    if(vertex.x<h*L)
    {
      add_edge(vertex, ::p(vertex.x+L, vertex.y), L);
      add_edge(::p(vertex.x+L, vertex.y), vertex, L);
    }
    // horizontal edges on the right of vertex
    if(vertex.y<w*L)
    {
      add_edge(vertex, ::p(vertex.x, vertex.y+L), L);
      add_edge(::p(vertex.x, vertex.y+L), vertex, L);
    }
  }
  */

  for(int hi{1}; hi<=h; hi++) for(int wi{1}; wi<=w; wi++)
  {
    assert(hi<=int(world.size()));
    assert(wi<=int(world[hi-1].length()));
    if(world[hi-1][wi-1]=='x')
    {
      // own cell edges.
      /// top
      if(hi==1)
        add_edge(vertex_t((hi-1)*L, (wi-1)*L), vertex_t((hi-1)*L, wi*L), L);
      /// bottom
      add_edge(vertex_t(hi*L, (wi-1)*L), vertex_t(hi*L, wi*L), L);
      /// left
      if(wi==1)
        add_edge(vertex_t((hi-1)*L, (wi-1)*L), vertex_t(hi*L, (wi-1)*L), L);
      /// right
      add_edge(vertex_t((hi-1)*L, wi*L), vertex_t(hi*L, wi*L), L);

      // edges to the cell at the bottom.
      if(hi<h and world[hi][wi-1]=='o')
      {
        // horizontal at the bottom.
        add_edge(vertex_t(hi*L, (wi-1)*L), vertex_t(hi*L, (wi-1)*L+L/2), L/2);
        add_edge(vertex_t(hi*L, wi*L),     vertex_t(hi*L, wi*L-L/2),     L/2);
        // vertical on the sides.
        //TODO
      }

      // edges to the cell at the right.
      if(wi<h and world[hi-1][wi]=='o')
      {
        // horizontal top and bottom, from the right corners of the
        // square in the current cell to the top and bottom midpoints
        // of the round tower to the right:
        add_edge(vertex_t((hi-1)*L, wi*L), vertex_t((hi-1)*L, wi*L+L/2), L/2);
        add_edge(vertex_t(    hi*L, wi*L), vertex_t(    hi*L, wi*L+L/2), L/2);
        //TODO vertical on the right from top to midpoint,
        //and another one from midpoint to bottom
      }
    }
    else
    {
      assert(world[hi-1][wi-1]=='o');
      // own cell edges.
      // Special case for the last cell reaching the goal at the south-east corner:
      ///TODO
      /// first quadrant
      add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, wi*L),     std::numbers::pi*L/2);
      /// second quadrant
      add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, (wi-1)*L), std::numbers::pi*L/2);
      /// third quadrant
      add_edge(vertex_t(hi*L-L/2, (wi-1)*L), vertex_t(hi*L, wi*L-L/2),     std::numbers::pi*L/2);
      /// fourth quadrant
      add_edge(vertex_t(hi*L, wi*L-L/2),     vertex_t(hi*L-L/2, wi*L),     std::numbers::pi*L/2);

      // edges to the cell at the bottom.
      // TODO
      // edges to the cell at the right.
      // TODO
    }
  }

  dbp(graph.size());
}

float shortest_path()
{
  std::cout << ">> shortest_path()\n";
  std::priority_queue<edge_dest_t, std::vector<edge_dest_t>, std::greater<edge_dest_t>> pq;

  // Create a container for distances
  // and initialize all to infinity.
  std::unordered_map<vertex_t, float> dist;
  for(const auto &[u, ignore] : graph)
    dist.insert(std::make_pair(u, std::numeric_limits<float>::max()));

  // Initialize queue with starting vertex
  // (top-left corner at distance 0.0).
  pq.emplace(vertex_t(0,0), 0.0);
  dist.at(vertex_t(0,0)) = 0.0;

  while(!pq.empty())
  {
    const vertex_t u{pq.top().u};
    dbp(u);
    pq.pop();

    //Return if destination reached (bottom-right).
    if(u==vertex_t(L*h, L*w))
      return dist.at(u);

    for(const auto &[v, weight] : graph.at(u))
    {
      if(dist.at(v) > dist.at(u) + weight)
      {
        dist.at(v) = dist.at(u) + weight;
        pq.push(edge_dest_t(v, dist[v]));
      }
    }
  }

  return -42.;
}

//Given a vertex, list of neighbors vertices and distances to them (edges?)
//Priority queue stuff. std::set because we can erase, not in constant or logarithmic time. std::priority_queue with duplicates. Ah, I suppose following that idea with a std::multiset should be also possible. Ah, but the sets could be problematic to modify while iterating through them.


/*
 *
 *
      XX
      XX
      XX

      X--.--X--.--X
      |     |     |
      .     .     .
      |     |     |
      X--.--X--.--X
      |     |     |
      .     .     .
      |     |     |
      X--.--X--.--X
      |     |     |
      .     .     .
      |     |     |
      X--.--X--.--X
 *
 *
 *
 *
 *
 */
