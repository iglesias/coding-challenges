#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <cmath>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#define   FOR(i, a, b)    for ( decltype(a) i = (a) ; i < (b) ; ++i )
#define   REPEAT(i, n)    FOR(i, 0, n)

using ii = std::pair<int, int>;

void read_input();
std::pair<int32_t, int64_t> solve();

using grid_t = std::vector<std::string>;

// input
grid_t G;
int R, C;

void print(grid_t const& g)
{
  static bool first_call = true;
  if (first_call) first_call = false;
  else            std::cout << "\033[" << R << "A\033[" << C << "D";
  REPEAT(r, R) std::cout << g[r] << "\n";
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(200ms);
}

int main()
{
  read_input();
  std::pair const ans = solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  while(getline(std::cin, line)){
    G.push_back(line);
    R++;
    C = int(line.size());
  }
}

std::vector<ii> const deltas{{0,1}, {0,-1}, {1,0} , {-1,0}};

std::queue<ii> paint_grid(grid_t const& G, grid_t& g, std::queue<ii>& q)
{
  std::queue<ii> nq;
  std::set<ii> nqed;
  while(!q.empty()){
    auto p = q.front();
    q.pop();
    for(auto const& delta : deltas){
      int r = p.first + delta.first, c = p.second + delta.second;
      if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
      if(nqed.contains({r, c})) continue;
      if(G[r][c] != '#'){
        g[r][c] = 'O';
        nq.emplace(r, c);
        nqed.emplace(r, c);
      }
    }
  }
  return nq;
}

std::queue<ii> make_new_positions(grid_t const& G, std::queue<ii>& q)
{
  std::queue<ii> nq;
  std::unordered_set<ii, boost::hash<ii>> nqed;
  while(!q.empty()){
    ii const p = q.front();
    q.pop();
    for(ii const& delta : deltas){
      int const r = p.first + delta.first, c = p.second + delta.second;
      // In part two the map is infinite:
      //if(not(0 <= r and r < R and 0 <= c and c < C)) continue;
      int const rr = ((r % R) + R) % R; //while(rr < 0) rr += R;
      int const cc = ((c % C) + C) % C; //while(cc < 0) cc += C;
      if(G[rr][cc] != '#' and !nqed.contains({r, c})){
        nq.emplace(r, c);
        nqed.emplace(r, c);
      }
    }
  }
  return nq;
}

ii get_start_position(grid_t const& G)
{
  REPEAT(r, R) REPEAT(c, C) if(G[r][c] == 'S') return std::make_pair(r, c);
  std::unreachable();
  return {};
}

int64_t fit_polynomial_and_extrapolate(std::vector<int64_t> const& x, std::vector<int64_t> const& y)
{
  namespace py = pybind11;
  py::scoped_interpreter const guard{};
  py::object const scipy_interpolate = py::module::import("scipy.interpolate");
  py::object const poly =
      scipy_interpolate.attr("lagrange")(py::array_t<int64_t>(x.size(), x.data()),
                                         py::array_t<int64_t>(y.size(), y.data()));
  //TODO does the call chain leak?
  py::buffer_info const coef_buf = poly.attr("coef").cast<py::array_t<double>>().request();
  std::vector<double> coef_vec(static_cast<double*>(coef_buf.ptr),
                               static_cast<double*>(coef_buf.ptr) + coef_buf.size);
  std::ranges::reverse(coef_vec);
  py::object const numpy_polynomial = py::module::import("numpy.polynomial");
  return std::llround(numpy_polynomial.attr("Polynomial")
                      (py::array_t<double>(coef_vec.size(), coef_vec.data()))
                      (26501365).cast<double>());
}

std::pair<int, int64_t> solve()
{
  ii const start = get_start_position(G);
  std::pair<int, int64_t> ans;

  {
    std::queue<ii> q;
    q.push(start);
    grid_t g;
    for(int i = 0; i < 64; i++){
      g = G;
      q = paint_grid(G,g,q);
      //print(g);
    }
    ans.first = static_cast<int>(q.size());
  }

  {
    std::vector<int64_t> const poly_xs{65, 65 + R, 65 + R*2};
    std::vector<int64_t> poly_ys;
    {
      std::queue<ii> q;
      q.push(start);
      grid_t g;
      for (int i = 0; i < *std::rbegin(poly_xs); i++) {
        if (i == poly_xs.at(0) or i == poly_xs.at(1))
          poly_ys.push_back(q.size());
        q = make_new_positions(G,q);
      }
      poly_ys.push_back(q.size());
    }

    ans.second = fit_polynomial_and_extrapolate(poly_xs, poly_ys);
  }

  return ans;
}
