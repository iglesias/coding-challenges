#include <bits/stdc++.h>

using integer = long long int;

std::vector<std::string> read_input();
integer snafu2dec(const std::string&);
std::string dec2snafu(integer);

int main(int argc, char* argv[])
{
  if(argc>1)
  {
    std::cout << snafu2dec(argv[1]) << '\n';
    return 0;
  }

  auto input = read_input();
  integer sum{0};
  for (const auto &elem : input)
  {
    //std::cout << elem << "    " << snafu2dec(elem) << "\n";
    sum += snafu2dec(elem);
  }

  std::cout << "sum=" << sum << '\n';
  std::cout << dec2snafu(sum) << '\n';
}

std::vector<std::string> read_input()
{
  std::vector<std::string> input;
  std::string line;
  while(std::getline(std::cin, line))
    input.push_back(line);

  return input;
}

int snafu2dec(char c)
{
  switch(c)
  {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '-':
      return -1;
    case '=':
      return -2;
    default:
      assert(false);
  }
}

integer snafu2dec(const std::string &str)
{
  integer base = 1;
  integer result = 0;
  for(int i{static_cast<int>(str.length())-1}; i>=0; i--)
  {
    result += snafu2dec(str[i])*base;
    base *= 5;
  }

  return result;
}

void gen_snafus_brute(std::string&, int d=0);
bool search(std::string&, int d=0);
int num_digits;
integer nn;
std::string dec2snafu(integer n)
{
  num_digits = 0;
  nn = n;
  std::string snafu;
  do{
    num_digits++;
    std::cout << ">> dec2snafu(n) num_digits=" << num_digits << std::endl;
    snafu = std::string(num_digits, '0');
  } while(!search(snafu));


  return snafu;
}

void gen_snafus_brute(std::string& snafu, int d)
{
  for(int v{-2}; v<=2; v++)
  {
    if(v==-2)
    {
      snafu[d] = '=';
    }
    else if(v==-1)
    {
      snafu[d] = '-';
    }
    else
    {
      snafu[d] = char('0'+v);
    }

    if(d<num_digits-1)
      gen_snafus_brute(snafu, d+1);
    else
    {
//      std::cout << ">> " << snafu << std::endl;
      if(snafu2dec(snafu)==nn)
      {
        std::cout << ">> found match! " << snafu << '\n';
        return;
      }
    }
  }
}

bool search(std::string& snafu, int d)
{
  if(d==0)
  {
    for(int v=1; v<=2; v++)
    {
      snafu[d] = char('0'+v);

      if(d<num_digits-1)
        if(search(snafu, d+1)) return true;
    }

    return false;
  }

  for(int dd{d}; dd<=num_digits-1; dd++) snafu[dd] = '0';
  integer remainder = nn-snafu2dec(snafu);

  std::vector<char> digits;
  {
    integer power5=1;
    for(int dd{num_digits-2}; dd>=d; dd--) power5 *= 5;
    if(remainder>0)
    {
      if(remainder>2*power5)
        digits = {'2'};
      else
        digits = {'2', '1', '0', '-', '='};
    }
    else if(remainder<0)
    {
      if(remainder<-2*power5)
        digits = {'='};
      else
        digits = {'=', '-', '0', '1', '2'};
    }
    else
    {
      std::cout << ">> found match via remainder! " << snafu << '\n';
      return true;
    }
  }

  auto check_match = [&]()
  {
    integer guess{snafu2dec(snafu)};
    std::cout << ">> " << snafu << " " << guess << std::endl;
    if(guess==nn)
    {
      std::cout << ">> found match! " << snafu << '\n';
      return true;
    }
    return false;
  };


  for(char digit : digits)
  {
    snafu[d] = digit;
    if(d<num_digits-1)
    {
      if(search(snafu, d+1)) return true;
    }
    else 
    {
      if(check_match()) return true;
    }
  }

  return false;
}
