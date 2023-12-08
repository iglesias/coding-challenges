#include <bits/stdc++.h>

void read_input();

std::string instructions;
std::map<std::string, std::pair<std::string, std::string>> M;
std::vector<std::string> vs;

int main(int argc, char* argv[])
{
  read_input();

  std::string s = "AAA";
  if(argc > 1)
  {
    int const idx = std::stoi(argv[1]);
    s = vs.at(idx);
  }

  int i{0}, ans{0};
  while(true)
  {
    //if(s=="ZZZ") break;
    if(s.back()=='Z') break;

    if(instructions[i]=='L')
      s = M.at(s).first;
    else //if(instructions[i]=='R')
      s = M.at(s).second;

    ans++;
    i = (i+1)%(int)(instructions.length());
  }
  std::cout << ans << std::endl;
}

void read_input()
{
  std::getline(std::cin, instructions);

  std::string line;
  std::getline(std::cin, line);
  while(std::getline(std::cin, line))
  {
    std::string org   = line.substr(0, 3);
    std::string left  = line.substr(7, 3);
    std::string right = line.substr(12, 3);
    M.insert(std::make_pair(org, std::make_pair(left, right)));
    if(org.back()=='A') vs.push_back(org);
  }
}
