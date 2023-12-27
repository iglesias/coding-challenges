#include <bits/stdc++.h>

//FIXME try using std::uint_fast32_t brute-forcing the second part,
//with the interval algorithm unsigned might be not possible
//using uu = std::pair<uint, uint>;
using ll = std::pair<long long, long long>;
//FIXME for readability rename ll to interval or so denoting an interval type

namespace
{

struct range
{
  long long dst, src, len;
  friend std::ostream& operator<<(std::ostream& os, range const& r);
};

[[maybe_unused]] std::ostream& operator<<(std::ostream& os, range const& r)
{
  os << r.dst << ' ' << r.src << ' ' << r.len;
  return os;
}

struct map
{
  std::vector<range> ranges;

  long long apply(long long n)
  {
    for(auto const& r : ranges)
      if(r.src <= n and n <= r.src+r.len-1)
        return n-r.src+r.dst;
    return n;
  }

  auto apply(std::queue<ll> q) -> std::queue<ll>
  {
    //std::cout << ">> apply with "; dbp(ranges);
    std::queue<ll> out;
    while(!q.empty())
    {
      //std::cout << ">>>> q has " << q.size() << " elements.\n";
      ll interval{q.front()};
      q.pop();

      //std::cout << ">>>> Pop from q"; dbp(interval);
      assert(interval.second >= interval.first);
      auto const [a, b] = interval;
      bool mapped{false};

      for(auto const& r : ranges)
      {
        //dbp(a, r);
        if(r.src <= a and a <= r.src+r.len-1)
        {
          // Break interval as needed.
          if(b <= r.src+r.len-1)
          {
            //std::cout << ">>>> Push into out"; dbp(a-r.src+r.dst, b-r.src+r.dst);
            out.push(std::make_pair(a-r.src+r.dst, b-r.src+r.dst));
          }
          else
          {
            //std::cout << ">>>> Push into out"; dbp(a-r.src+r.dst, r.len-1+r.dst);
            out.push(std::make_pair(a-r.src+r.dst, r.len-1+r.dst));
            //std::cout << ">>>> and push into q"; dbp(r.src+r.len, b);
            q.push(std::make_pair(r.src+r.len, b));
          }
          mapped = true;
          break;
        }
        else if(r.src <= b and b <= r.src+r.len-1)
        {
          // Only the case breaking the interval is possible at this point.
          assert(a < r.src);
          out.push(std::make_pair(r.dst, b-r.src+r.dst)); // transform from r.src to b
          q.push(std::make_pair(a, r.src-1)); // store from a to r.src-1
          mapped = true;
          break;
        }
      }

      // Those that aren't mapped are left unchanged.
      if(!mapped)
      {
        for(auto const& r : ranges)
          assert(b < r.src or r.src+r.len-1 < a);

        //std::cout << ">>>> Push into out w/o conversion "; dbp(interval);
        out.push(interval);
      }
    }

    return out;
  }
};

} // anonymous namespace

namespace std {
std::ostream& operator<<(std::ostream& in, ll const& p)
{
  in << "(" << p.first << ", " << p.second << ")";
  return in;
}
}

::map s2s, s2f, f2w, w2l, l2t, t2h, h2l;
std::vector<long long> seeds;

long long to_location(long long seed)
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

  ll ans{std::make_pair(LLONG_MAX, LLONG_MAX)};

  assert(seeds.size()%2 == 0);
  for(uint i{0}; i < seeds.size(); i += 2)
  {
    ans.first = std::min(ans.first, std::min(to_location(seeds[i]), to_location(seeds[i+1])));

    ll const seed_interval{std::make_pair(seeds[i], seeds[i]+seeds[i+1]-1)};
    std::queue<ll> location_intervals{to_location(seed_interval)};
    while(!location_intervals.empty())
    {
      auto& l{location_intervals.front()};
      ans.second = std::min(ans.second, std::min(l.first, l.second));
      location_intervals.pop();
    }
  }

  std::cout << "Part one: " << ans.first << '\n';
  std::cout << "Part two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  std::getline(std::cin, line);

  {
    std::stringstream ss(line);
    std::string str;
    ss >> str;
    assert(str=="seeds:");
    std::copy(std::istream_iterator<long long>(ss),
              std::istream_iterator<long long>(),
              std::back_inserter(seeds));
  }

  std::getline(std::cin, line);
  assert(line.empty());

  auto read_map = [&line](std::string_view map_name, map& m)
  {
    std::getline(std::cin, line);
    assert(line==std::format("{} map:", map_name));

    for(;;)
    {
      std::getline(std::cin, line);
      if(line.empty()) break;
      std::stringstream ss(line);
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
