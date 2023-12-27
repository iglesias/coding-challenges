#include <bits/stdc++.h>

using ii = std::pair<int, int>;

using input_t = std::vector<std::vector<int>>;
auto read_input() -> input_t;
auto solve(input_t const&) -> ii;

int main()
{
  auto const input{read_input()};
  auto const ans{solve(input)};
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

template<typename string> auto split(std::string const& str, string sep) -> std::vector<int>
{
  std::string::size_type pos{0}, n;
  std::vector<int> out;

  while((n = str.find(sep, pos)) != std::string::npos)
  {
    out.push_back(std::stoi(str.substr(pos, n-pos)));
    pos = n+1;
  }
  if(pos != 0) out.push_back(std::stoi(str.substr(pos)));

  return out;
}

auto read_input() ->std::vector<std::vector<int>>
{
  std::vector<std::vector<int>> input;
  std::string line;
  while(std::getline(std::cin, line)) input.push_back(split(line, ' '));
  return input;
}

auto solve(input_t const& input) -> ii
{
  ii ans{0,0};
  for(uint n{0}; n<input.size(); n++)
  {
    bool all_zeros{true};
    std::vector<int> v{input[n]}, w, last_ones, first_ones;
    last_ones.push_back(v[v.size()-1]);
    do
    {
      all_zeros = true;
      first_ones.push_back(v[0]);
      for(uint i{0}; i<v.size()-1; i++)
      {
        int const d{v[i+1]-v[i]};
        all_zeros = all_zeros and (d==0);
        w.push_back(d);
      }
      last_ones.push_back(w[w.size()-1]);
      v = w; w.clear();
    } while(!all_zeros);

    for(size_t i{last_ones.size()-1};  i>0; i--) last_ones[i-1]  += last_ones[i];
    for(size_t i{first_ones.size()-1}; i>0; i--) first_ones[i-1] -= first_ones[i];

    ans.first  += last_ones[0];
    ans.second += first_ones[0];
  }

  return ans;
}
