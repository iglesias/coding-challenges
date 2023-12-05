#include <bits/stdc++.h>

#include <format>

using uint = unsigned int;
using uu   = std::pair<uint, uint>;

namespace
{

struct range
{
  uint dst, src, len;
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
    // Any source number that aren't mapped
    // correspond to the same destination number.
    return n;
  }
};

} // anonymous namespace

::map s2s, s2f, f2w, w2l, l2t, t2h, h2l;
std::vector<uint> seeds;

void read_input();

int main()
{
  read_input();

  std::pair<uint, uint> ans{std::make_pair(INT_MAX, INT_MAX)};

  for(auto seed : seeds)
  {
    ans.first = std::min(ans.first,
        h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))))
      );
  }
  std::cout << ans.first << '\n';

  assert(seeds.size()%2 == 0);
  for(uint i{0}; i < seeds.size(); i += 2)
  {
    for(uint seed{seeds[i]}; seed <= seeds[i]+seeds[i+1]-1; seed++)
    {
      ans.second = std::min(ans.second,
          h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))))
        );
    }
  }
  std::cout << ans.second << '\n';
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
