// {{{ Boilerplate Code <--------------------------------------------------

#include <bits/stdc++.h>
#include <cstdint>
#include <cxxabi.h>
#include <queue>
#include <sstream>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

#define dbp(...) dblog(#__VA_ARGS__, __VA_ARGS__)

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  int status;
  char* demangled_name =
    abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
  assert(!status);
  os << "vector<" << demangled_name << ">(";
  std::free(demangled_name);
  for(size_t i{0}; i < v.size(); i++)
    os << v[i] << ((i == v.size()-1)? ")" : ", ");
  return os;
}

template<typename... Args>
void dblog(std::string vars, Args&&... values)
{
  std::cout << "[" << vars << " = ";
  std::string delim = "";
  (..., (std::cout << delim << values, delim = ", "));
  std::cout << "]\n";
}

using ii = std::pair<int, int>;

// }}}

//FIXME try using std::uint_fast32_t
using uu = std::pair<uint, uint>;
using ll = std::pair<long long, long long>;

namespace
{

struct range
{
  long long dst, src, len;
  friend std::ostream& operator<<(std::ostream& in, range const& r);
};

std::ostream& operator<<(std::ostream& in, range const& r)
{
  in << r.dst << ' ' << r.src << ' ' << r.len;
  return in;
}

struct map
{
  std::vector<range> ranges;

  uint apply(uint n)
  {
    for(auto const& r : ranges)
      if(r.src <= n and n <= r.src+r.len-1)
        return n-r.src+r.dst;
    return n;
  }

  auto apply(std::queue<ll> q) -> std::queue<ll>
  {
    std::cout << ">> apply with "; dbp(ranges);
    std::queue<ll> out;
    while(!q.empty())
    {
      std::cout << ">>>> q has " << q.size() << " elements.\n";
      ll interval{q.front()};
      q.pop();

      std::cout << ">>>> Pop from q"; dbp(interval);
      assert(interval.second >= interval.first);
      long long a{interval.first}, b{interval.second};
      bool mapped{false};

      for(auto const& r : ranges)
      {
        //dbp(a, r);
        if(r.src <= a and a <= r.src+r.len-1)
        {
          // Break interval as needed.
          if(b <= r.src+r.len-1)
          {
            std::cout << ">>>> Push into out"; dbp(a-r.src+r.dst, b-r.src+r.dst);
            out.push(std::make_pair(a-r.src+r.dst, b-r.src+r.dst));
          }
          else
          {
            std::cout << ">>>> Push into out"; dbp(a-r.src+r.dst, r.len-1+r.dst);
            out.push(std::make_pair(a-r.src+r.dst, r.len-1+r.dst));
            std::cout << ">>>> and push into q"; dbp(r.src+r.len, b);
            q.push(std::make_pair(r.src+r.len, b));
          }

          mapped = true;
        }
      }

      // Any source number that aren't mapped
      // correspond to the same destination number.
      if(!mapped)
      {
        for(auto const& r : ranges)
          assert(a < r.src or r.src+r.len-1 < a);

        std::cout << ">>>> Push into out w/o conversion "; dbp(interval);
        out.push(interval);
      }
    }

    return out;
  }
};

} // anonymous namespace

namespace std {
std::ostream& operator<<(std::ostream& in, std::pair<uint, uint> const& uu)
{
  in << "(" << uu.first << ", " << uu.second << ")";
  return in;
}
}

::map s2s, s2f, f2w, w2l, l2t, t2h, h2l;
std::vector<uint> seeds;

uint to_location(uint seed)
{
  return h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))));
}

std::queue<ll> to_location(ll seed_interval)
{
  std::queue<ll> q;
  q.push(seed_interval);

  return h2l.apply(t2h.apply(l2t.apply(w2l.apply(
            f2w.apply(s2f.apply(s2s.apply(q)))))));
}

void read_input();

int main()
{
  read_input();

  std::pair<uint, long long> ans{std::make_pair(UINT_MAX, LLONG_MAX)};

  for(auto seed : seeds)
  {
    ans.first = std::min(ans.first,
        h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))))
      );
  }
  std::cout << "Part one: " << ans.first << '\n';

  assert(seeds.size()%2 == 0);
  for(uint i{0}; i < seeds.size(); i += 2)
  {
    ll seed_interval{std::make_pair(seeds[i], seeds[i]+seeds[i+1]-1)};
    std::cout << ">> Range of seeds " << seed_interval.first << ' ' << seed_interval.second << std::endl;

    std::queue<ll> q;
    q.push(seed_interval);

    /*
    q = s2s.apply(q);
    assert(q.size()==1); // for the sample input
    std::cout << ">> Generated range of soil " << q.front().first << ' ' << q.front().second << std::endl;

    q = s2f.apply(q);
    std::cout << ">> Generated...\n";
    */
    auto x{to_location(seed_interval)};
    while(!x.empty())
    {
      auto& y{x.front()};
      ans.second = std::min(ans.second, std::min(y.first, y.second));
      x.pop();
    }
  }
  std::cout << "Part two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;

  std::getline(std::cin, line);
  std::stringstream ss(line);
  std::string str;
  ss >> str;
  assert(str=="seeds:");
  uint seed;
  while(!ss.eof()) { ss >> seed; seeds.push_back(seed); }

  std::getline(std::cin, line);
  assert(line.empty());

  auto read_map = [&line, &ss](std::string_view map_name, map& m)
  {
    std::getline(std::cin, line);
    assert(line==std::format("{} map:", map_name));

    for(;;)
    {
      std::getline(std::cin, line);
      if(line.empty()) break;
      ss = std::stringstream(line);
      ::range r;
      ss >> r.dst >> r.src >> r.len;
      m.ranges.push_back(r);
    }
  };

  read_map("seed-to-soil", s2s);
  read_map("soil-to-fertilizer", s2f);
  read_map("fertilizer-to-water", f2w);
  read_map("water-to-light", w2l);
  read_map("light-to-temperature", l2t);
  read_map("temperature-to-humidity", t2h);
  read_map("humidity-to-location", h2l);
}
