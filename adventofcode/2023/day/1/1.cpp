#include <bits/stdc++.h>

int main()
{
  std::string line;
  int ans{0};
  for(;;)
  {
    std::getline(std::cin, line);
    if(line.empty()) break;
    int first_digit, last_digit;
    for(int i{0}; i<line.length(); i++)
    {
      if(line[i]>='0' && line[i]<='9')
      {
        first_digit = line[i]-'0';
        break;
      }
    }
    for(int i{static_cast<int>(line.length())-1}; i>=0; i--)
    {
      if(line[i]>='0' && line[i]<='9')
      {
        last_digit = line[i]-'0';
        break;
      }
    }
    //std::cout << ">> from " << line << " adding " << first_digit << " and " << last_digit << '\n';
    ans += first_digit*10 + last_digit;
    std::cout << ans << ' ';
  }

  std::cout << '\n' << ans << '\n';
}
