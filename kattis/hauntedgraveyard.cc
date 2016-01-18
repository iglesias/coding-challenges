#include <cmath>
#include <iostream>
#include <vector>
#include <limits>

#define FOR(i, a, b) for ( auto i = (a) ; i < (b) ; ++i )
#define REP(i, n) FOR(i, 0, n)

using namespace std;

// forward declarations
string BellmanFord();
struct edge;

typedef vector<edge> graph;

// globals
int W, H, G, E;
graph g;

struct vertex
{
  int x;
  int y;

  vertex() : x(0), y(0) {}
  vertex(int _x, int _y) : x(_x), y(_y) {}

  // go from grid to linear coordinates
  int idx() const { return x + W*y; }
};

struct edge
{
  vertex src;
  vertex dst;
  int w;

  edge() : w(0) {}
  edge(vertex _src, vertex _dst, int _w) : src(_src), dst(_dst), w(_w) {}
};

int main()
{
  while (true)
  {

    cin >> W >> H; 
    if (W == 0 && H == 0) break;

    // read graves
    vector<bool> graves(W*H, false);
    cin >> G;
    REP(i, G)
    {
      int X, Y;
      cin >> X >> Y;
      graves[vertex(X, Y).idx()] = true;
    }

    // read haunted holes
    vector<bool> holes(W*H, false);
    g.clear();
    cin >> E;
    REP(i, E)
    {
      int X1, Y1, X2, Y2, T;
      cin >> X1 >> Y1 >> X2 >> Y2 >> T;
      if (X1 == W-1 && Y1 == H-1) continue; // if we arrive at the exit, it is finished
      vertex src(X1, Y1);
      vertex dst(X2, Y2);
      g.push_back(edge(src, dst, T));
      holes[src.idx()] = true;
    }

    // insert edges
    REP(x, W) REP(y, H)
    {
      vertex v(x,y);

      // no edges for graves, neither for haunted holes
      if (graves[v.idx()] || holes[v.idx()]) continue;
      // no edges from the exit
      if (x == W-1 && y == H-1) continue;

      // try west
      if (x > 0)
      {
        vertex dst(x-1, y);
        if (!graves[dst.idx()]) g.push_back(edge(v, dst, 1));
      }
      // try east
      if (x < W-1)
      {
        vertex dst(x+1, y);
        if (!graves[dst.idx()]) g.push_back(edge(v, dst, 1));
      }
      // try south
      if (y > 0)
      {
        vertex dst(x, y-1);
        if (!graves[dst.idx()]) g.push_back(edge(v, dst, 1));
      }
      // try north
      if (y < H-1)
      {
        vertex dst(x, y+1);
        if (!graves[dst.idx()]) g.push_back(edge(v, dst, 1));
      }
    }

    cout << BellmanFord() << endl;
  }

  return 0;
}

string BellmanFord()
{
  // initialize distances
  vector<double> dists(W*H, numeric_limits<double>::infinity());
  dists[0] = 0;
  
  // relax edges repeatedly
  REP(i, W*H) REP(j, g.size())
  {
    vertex u = g[j].src;
    vertex v = g[j].dst;
    int w = g[j].w;
    if (dists[u.idx()] + w < dists[v.idx()]) dists[v.idx()] = dists[u.idx()] + w;
  }

  // check for negative-weight cycles
  REP(j, g.size())
  {
    vertex u = g[j].src;
    vertex v = g[j].dst;
    int w = g[j].w;
    if (dists[u.idx()] + w < dists[v.idx()]) return "Never";
  }

  vertex exit(W-1, H-1);
  if (dists[exit.idx()] == numeric_limits<double>::infinity()) return "Impossible";
  else return to_string(static_cast<int>(dists[exit.idx()]));
}
