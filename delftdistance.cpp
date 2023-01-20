// g++ -std=c++20 -Wall -Wextra -pedantic -o delftdistance delftdistance.cpp

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
constexpr int WMAX{700};
// side length of square buildings and diameter of round towers in meters
constexpr int L{10};

namespace
{
  struct p
  {
    int x, y;
    p() : x(0), y() {}
    p(int _x, int _y) : x(_x), y(_y) {}
    p(const p &other) : x(other.x), y(other.y) {}
    p& operator=(const p &other) { x = other.x; y = other.y; return *this; } // For path reconstruction (debug)
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

  friend std::ostream& operator<<(std::ostream &os, const edge_dest_t &e)
  {
    return os << "{u=" << e.u << ", dist=" << e.dist << "}";
  }
};

void build_graph();
double shortest_path();

int h, w;
std::vector<std::string> world;

int main()
{
  std::cout << std::fixed << std::setprecision(10) << std::numbers::pi << std::endl;
  const ::p p0(0,0);
  const std::hash<::p> hash;
  dbp(hash(p0));
  dbp(hash(::p(7000, 7000)));
  dbp(hash(::p(4512, 1234)));
  dbp(hash(::p(525, 32)));

  std::cin >> h >> w;
  std::string str;
  std::getline(std::cin, str);
  while(std::getline(std::cin, str))
    world.push_back(std::move(str));
  
  build_graph();
  std::cout << std::fixed << std::setprecision(10) << shortest_path() << std::endl;
}

std::unordered_map<vertex_t, std::unordered_map<vertex_t, double>> graph;

void add_edge(const ::vertex_t &from, const ::vertex_t &to, double dist)
{
  //TODO for debugging could help to verify that the Euclidean distance between from and to is equal to dist.
  static bool toggle{false};

  if(graph.contains(from))
  {
    //dbp(from, to);
    //assert(!graph.at(from).contains(to)); // ♥ ♥ ♥
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

  assert(L%2 == 0);

  for(int hi{1}; hi<=h; hi++) for(int wi{1}; wi<=w; wi++)
  {
    //dbp(hi, wi);
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
        // A - horizontal edges of L/2 length at the bottom,
        // from bottom-left corner to midpoint, and
        add_edge(vertex_t(hi*L, (wi-1)*L), vertex_t(hi*L, (wi-1)*L+L/2), L/2);
        // from bottom-right corner to midpoint.
        add_edge(vertex_t(hi*L, wi*L),     vertex_t(hi*L, wi*L-L/2),     L/2);
        // vertical on the sides.
        //TODO
      }

      // edges to the cell at the right.
      if(wi<w and world[hi-1][wi]=='o')
      {
        // OJO! These ones seem to be the same as A; only if the
        // cell on top of the one with 'o' was also an X...
        //
        // B - horizontal top and bottom, from the right corners of the
        // square in the current cell to the top and bottom midpoints
        // of the round tower to the right:
        //TODO add similar if guards for the rest of  this kind of edges in the other cases
        if(hi<2 or (world[hi-2][wi-1]=='o' and world[hi-2][wi]=='o'))
          add_edge(vertex_t((hi-1)*L, wi*L), vertex_t((hi-1)*L, wi*L+L/2), L/2);

        add_edge(vertex_t(hi*L, wi*L), vertex_t(hi*L, wi*L+L/2), L/2);

        // C - vertical on the right from top to midpoint,
        add_edge(vertex_t((hi-1)*L, wi*L), vertex_t((hi-1)*L+L/2, wi*L), L/2);
        // and another one from midpoint to bottom
        add_edge(vertex_t((hi-1)*L+L/2, wi*L), vertex_t(hi*L, wi*L), L/2);
      }
    }
    else
    {
      assert(world[hi-1][wi-1]=='o');
      // own cell edges.
      /// first quadrant
      add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, wi*L),     std::numbers::pi*L/4);
      /// second quadrant
      add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t(hi*L-L/2, (wi-1)*L), std::numbers::pi*L/4);
      /// third quadrant
      add_edge(vertex_t(hi*L-L/2, (wi-1)*L), vertex_t(hi*L, wi*L-L/2),     std::numbers::pi*L/4);
      /// fourth quadrant
      add_edge(vertex_t(hi*L, wi*L-L/2),     vertex_t(hi*L-L/2, wi*L),     std::numbers::pi*L/4);

      // edges to the cell at the bottom with a square
      if(hi<h and world[hi][wi-1]=='x')
      {
        assert(false);
        // F - vertical edges on the left and on the right from midpoints
        // to top-left corner of the square
        // TODO
        // top top-right corners of square
        // TODO

        // G - horizontal edges at the bottom from midpoint
        // along the side of the square
        // from top-left corner to midpoint
        // TODO
        // from top-right corner to midpoint
        // TODO
      }
      // vertical edges to a cell at the bottom with another tower.
      else if(hi<h and world[hi][wi-1]=='o')
      {
        add_edge(vertex_t(hi*L-L/2, (wi-1)*L), vertex_t(hi*L+L/2, (wi-1)*L), L);
        add_edge(vertex_t(hi*L-L/2,     wi*L), vertex_t(hi*L+L/2,     wi*L), L);
      }

      // edges to a cell on the right with a square.
      if(wi<w and world[hi-1][wi]=='x')
      {
        // D - horizontal edges on top and bottom from midpoint
        // to top and bottom left corners of the square, respectively
        if(hi<2 or world[hi-2][wi-1]=='o')
          add_edge(vertex_t((hi-1)*L, (wi-1)*L+L/2), vertex_t((hi-1)*L, wi*L), L/2);

        add_edge(vertex_t(hi*L, (wi-1)*L+L/2), vertex_t(hi*L, wi*L), L/2);

        // E - vertical edges from midpoint along the side of the square,
        // from bottom-right corner to midpoint
        add_edge(vertex_t(hi*L, wi*L), vertex_t(hi*L-L/2, wi*L), L/2);
        // from top-left corner to midpoint
        add_edge(vertex_t((hi-1)*L, wi*L), vertex_t((hi-1)*L+L/2, wi*L), L/2);
      }
      // horizontal edges to a cell on the right with another tower.
      else if(wi<w and world[hi-1][wi]=='o')
      {
        add_edge(vertex_t((hi-1)*L, wi*L-L/2), vertex_t((hi-1)*L, wi*L+L/2), L);
        add_edge(vertex_t(hi*L,     wi*L-L/2), vertex_t(hi*L,     wi*L+L/2), L);
      }
    }
  }

  // Special case to start at the north-west corner:
  if(world[0][0]=='o')
    add_edge(vertex_t(0, 0), vertex_t(0, L/2), L/2); 

  // Special case to reach the goal at the south-east corner:
  if(world[h-1][w-1]=='o')
    add_edge(vertex_t(h*L-L/2, w*L), vertex_t(h*L, w*L), L/2);
}

void print_path(const std::unordered_map<vertex_t, vertex_t> pred);
double shortest_path()
{
  std::cout << ">> shortest_path()\n";
  std::priority_queue<edge_dest_t, std::vector<edge_dest_t>, std::greater<edge_dest_t>> pq;

  // Create a container for distances
  // and initialize all to infinity.
  std::unordered_map<vertex_t, double> dist;
  for(const auto &[u, ignore] : graph)
    dist.insert(std::make_pair(u, std::numeric_limits<double>::max()));

  // Container for path reconstruction.
  std::unordered_map<vertex_t, vertex_t> pred;

  // Initialize queue with starting vertex
  // (top-left corner at distance 0.0).
  pq.emplace(vertex_t(0,0), 0.0);
  dist.at(vertex_t(0,0)) = 0.0;

  while(!pq.empty())
  {
    const vertex_t u{pq.top().u};
    pq.pop();

    //Return if destination reached (bottom-right).
    //TODO probably helps with time, not 100% sure if it is correct solution.
    /*
    if(u==vertex_t(L*h, L*w))
      return dist.at(u);
    */

    for(const auto &[v, weight] : graph.at(u))
    {
      if(dist.at(v) > dist.at(u) + weight)
      {
        dist.at(v) = dist.at(u) + weight;
        pred[v] = u;
        pq.push(edge_dest_t(v, dist[v]));
      }
    }
  }

  assert(dist.at(vertex_t(L*h, L*w))<std::numeric_limits<double>::max());

  //print_path(pred);
  return dist.at(vertex_t(L*h, L*w));
}

// Path reconstruction:
void print_path(const std::unordered_map<vertex_t, vertex_t> pred)
{
  std::cout << "Path: ";
  vertex_t u_p{vertex_t(L*h, L*w)};
  do
  {
    std::cout << u_p << " <- ";
    u_p = pred.at(u_p);
  } while(u_p!=vertex_t(0,0));
  std::cout << vertex_t(0,0) << std::endl;
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
