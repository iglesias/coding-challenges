#include <bits/stdc++.h>

namespace 
{
  // a point in the 2D grid or matrix.
  struct p
  {
    // row and column,
    // zero-based indexing and
    // there'll even be points created whose r and/or c are out of bounds.
    int r, c;
    p(int _r, int _c) : r(_r), c(_c) {}
    p() : r(0), c(0) {}
  };

  struct number
  {
    // numbers are contiguous parts rows;
    // this abstraction is slightly too generic since start.r and end.r
    // aren't necessarily the same (whereas numbers shall occupy only 1 row), 
    // it might've become handy if e.g. in part two there could be
    // numbers placed vertically as well.
    p start, end;
    // the next two will be filled during part one and used for part two.
    int is_part; // -1 -> uninitialized, 0 -> no, 1 -> yes. FIXME std::optional for uninitialized.
    int value;   // the same applies regarding uninitialized.
    number(p _start, p _end) : start(_start), end(_end), is_part(-1), value(-1) {}
  };
};

using input_t = std::vector<std::string>; 

auto read() -> input_t;
auto extract_numbers(input_t const&) -> std::vector<::number>;
int part_numbers_sum(input_t const& input, std::vector<::number>& numbers);
int part_two(input_t const& input, std::vector<::number> const& numbers);

int main()
{
  auto const input{read()};
  auto numbers{extract_numbers(input)};
  std::cout << "Part one: " << part_numbers_sum(input, numbers) << '\n';
  std::cout << "Part two: " << part_two(input, numbers) << '\n';
}

input_t read()
{
  std::vector<std::string> out;
  std::string line;
  while(std::getline(std::cin, line)) out.push_back(line);
  return out;
}

// à la parsing
std::vector<::number> extract_numbers(input_t const& input)
{
  std::vector<::number> out;
  unsigned int i{0}, j{0};
  bool busy{false}; // parsing state, is a number being processed? FIXME rename.
  ::p start; // FIXME I think this can be scoped better.
  while(i < input.at(0).length()) // FIXME style one-liner loops
  {
    while(j < input.size())
    {
      if('0' <= input[i][j] and input[i][j] <= '9')
      {
        if(!busy)
        {
          start.r = i;
          start.c = j;
          busy = true;
        }
      }
      else
      {
        if(busy)
        {
          out.emplace_back(start, ::p(i, j-1));
          busy = false;
        }
      }

      j++;
    }

    if(busy)
    {
      out.emplace_back(start, ::p(i, j-1));
      busy = false;
    }

    j = 0;
    i++;
  }

  return out;
}

int part_numbers_sum(input_t const& input, std::vector<::number>& numbers)
{
  int ans{0};

  for(auto& number : numbers)
  {
    int const start_r{number.start.r-1}, start_c{number.start.c-1};
    int const end_r{number.start.r+1},   end_c{number.end.c+1};
    number.is_part = 0;

    for(int i{start_r}; i <= end_r && !number.is_part; i++)
    {
      if(i < 0) continue;
      if(i > (int)input.size()-1) break;

      for(int j{start_c}; j <= end_c && !number.is_part; j++)
      {
        if(j < 0) continue;
        if(j > (int)input[0].length()-1) break;

        if(input[i][j] != '.' and not('0' <= input[i][j] and input[i][j] <= '9'))
          number.is_part = 1;
      }
    }

    if(number.is_part == 1)
    {
      number.value = 0;
      int const i{number.start.r};
      for(int j{number.start.c}; j <= number.end.c; j++)
      {
        number.value *= 10;
        number.value += input[i][j]-'0';
      }
      ans += number.value;
    }
  }

  return ans;
}

int part_two(input_t const& input, std::vector<::number> const& numbers)
{
  int ans{0};
  for(int i{0}; i < (int)input.size()-1; i++)  // FIXME style one-liner loops
    for(int j{0}; j < (int)input[0].length(); j++)
    {
      if(input[i][j] == '*')
      {
        int adjacent_part_number_cnt{0};
        int gear_ratio{1};
        for(auto const& number : numbers)
        {
          if(!number.is_part) continue;
          if(i < number.start.r-1 || i > number.start.r+1) continue;
          if(j < number.start.c-1 || j > number.end.c+1) continue;
          adjacent_part_number_cnt++;
          if(adjacent_part_number_cnt > 2) break;
          gear_ratio *= number.value;
        }
        if(adjacent_part_number_cnt==2) ans += gear_ratio;
      }
    }
  return ans;
}
