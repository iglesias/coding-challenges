#include <bits/stdc++.h>

using ii = std::pair<int, int>;

int main()
{
  ii ans{0, 0};
  std::string line;
  while(std::getline(std::cin, line))
  {
    int id;
    sscanf(line.c_str(), "Game %d:", &id);
    bool valid{true};
    int max_blue{0}, max_red{0}, max_green{0};

    std::string::size_type n{0}, start{line.find(':')+2};
    while(n != std::string::npos)
    {
      n = line.find(';', n+1);
      auto const s{line.substr(start, n-start)};
      start = n+1;

      int num_commas{0};
      for(char c : s) if(c==',') num_commas++;
      std::istringstream ss(s);
      for(int i{0}; i <= num_commas; i++)
      {
        std::string color;
        int number;
        ss >> number >> color;

        if(*color.rbegin()==',') color.pop_back();

        if(color=="blue"){
          valid = valid && number <= 14;
          max_blue = std::max(max_blue, number);
        }else if(color=="red"){
          valid = valid && number <= 12;
          max_red = std::max(max_red, number);
        }else if(color=="green"){
          valid = valid && number <= 13;
          max_green = std::max(max_green, number);
        }else assert(false);
      }
    }

    if(valid) ans.first += id;
    int const power{max_blue*max_red*max_green};
    ans.second += power;
  }

  std::cout << "Part one: " << ans.first  << '\n';
  std::cout << "Part two: " << ans.second << '\n';
}
