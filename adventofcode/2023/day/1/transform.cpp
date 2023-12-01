#include <bits/stdc++.h>

std::vector<std::string> const digits = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

char transform_digit(std::string const& digit)
{
  if(digit=="one")      return '1';
  else if(digit=="two") return '2';
  else if(digit=="three") return '3';
  else if(digit=="four") return '4';
  else if(digit=="five") return '5';
  else if(digit=="six") return '6';
  else if(digit=="seven") return '7';
  else if(digit=="eight") return '8';
  else if(digit=="nine") return '9';
  else assert(false);
}

bool is_digit_prefix(std::string const& try_digit)
{
  for(int i{0}; i<digits.size(); i++)
  {
    int j{0};
    while(j<try_digit.length() && digits[i][j] == try_digit[j]) j++;
    if(j==try_digit.length()) return true;
  }

  return false;
}

bool is_digit(std::string const& try_digit)
{
  for(int i{0}; i<digits.size(); i++)
    if(try_digit == digits[i]) return true;

  return false;
}

int main()
{
  std::string line;
  for(;;)
  {
    std::getline(std::cin, line);
    if(line.length()==0) break;

    std::string transformed_line;
    std::string try_digit = "\0";
    int start_try_digit;
    int i{0};
    while(i<line.length())
    {
      if(is_digit_prefix(try_digit + line[i]))
      {
        if(try_digit.length()==0) start_try_digit = i;

        try_digit += line[i];
        if(is_digit(try_digit))
        {
          transformed_line += transform_digit(try_digit);
          try_digit = "\0";
        }
	      else
	      {
	        // fixed adding this else branch after
          // https://www.reddit.com/r/adventofcode/comments/1884fpl/2023_day_1for_those_who_stuck_on_part_2/
          i++;
	      }
      }
      else
      {
        if(try_digit.length()>0)
        {
          transformed_line += line[start_try_digit];
          i = start_try_digit+1;
          try_digit = "\0";
        }
        else
        {
          transformed_line += line[i];
          i++;
        }
      }
    }

    if(try_digit.length()>0) transformed_line += try_digit;

    //std::cout << line << ' ' << transformed_line << '\n';
    std::cout << transformed_line << '\n';
  }
}
