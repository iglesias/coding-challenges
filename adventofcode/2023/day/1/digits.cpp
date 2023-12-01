#include <bits/stdc++.h>

using uint = unsigned int;

std::vector<std::string> const digits = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

char transform_digit(std::string const& digit)
{
  if     (digit=="one")      return '1';
  else if(digit=="two")      return '2';
  else if(digit=="three")    return '3';
  else if(digit=="four")     return '4';
  else if(digit=="five")     return '5';
  else if(digit=="six")      return '6';
  else if(digit=="seven")    return '7';
  else if(digit=="eight")    return '8';
  else if(digit=="nine")     return '9';
  else assert(false);
}

bool is_digit_prefix(std::string const& try_digit)
{
  for(uint i{0}; i<digits.size(); i++)
  {
    uint j{0};
    while(j<try_digit.length() && digits[i][j] == try_digit[j]) j++;
    if(j==try_digit.length()) return true;
  }

  return false;
}

bool is_digit(std::string const& try_digit)
{
  for(uint i{0}; i<digits.size(); i++)
    if(try_digit == digits[i]) return true;

  return false;
}

// parser and converter from spelled-out digits
std::string transform(std::string const& in)
{
  std::string out, try_digit;
  int start_try_digit;
  uint i{0};
  while(i<in.length())
  {
    if(is_digit_prefix(try_digit + in[i]))
    {
      if(try_digit.length()==0) start_try_digit = i;

      try_digit += in[i];
      if(is_digit(try_digit))
      {
        out += transform_digit(try_digit);
        try_digit = "\0";
      }
      else // without else branch for considering non-overlapping.
      {
        i++;
      }
    }
    else
    {
      if(try_digit.length()>0)
      {
        out += in[start_try_digit];
        i = start_try_digit+1;
        try_digit = "\0";
      }
      else
      {
        out += in[i];
        i++;
      }
    }
  }

  return out;
}

int find_first(std::string const& in, bool reverse=false)
{
  int const start = reverse ? in.length()-1 : 0;
  int const delta = reverse ? -1 : +1;
  int const end   = reverse ? -1 : in.length();
  int i{start};
//  std::cout << ">> find_first(" << in << ", " << reverse << ")" << std::endl;
  do
  {
    if(in[i] >= '0' && in[i] <= '9') return in[i]-'0';
    i += delta;
  } while(i != end);

  assert(false);
  return -1;
}

std::pair<int, int> find_first_and_last_digits(std::string const& in)
{
  return std::make_pair(find_first(in), find_first(in, true));
}

int main()
{
  std::string line;
  int ans_a{0}, ans_b{0};
  for(;;)
  {
    std::getline(std::cin, line);
    if(line.empty()) break;

    auto a{ find_first_and_last_digits(line) };
    auto b{ find_first_and_last_digits(transform(line)) };

    ans_a += a.first*10 + a.second;
    ans_b += b.first*10 + b.second;
  }

  std::cout << "Part one: " << ans_a << '\n';
  std::cout << "Part two: " << ans_b << '\n';
}

//g++ -Wall -Wextra -pedantic -o digits digits.cpp
