#include <bits/stdc++.h>

using std::string;

void read_input();
template<typename integer> integer solve(bool is_part_two=false);

string instructions;
//FIXME compare timing std::map vs std::unordered_map.
std::map<string, std::pair<string, string>> M;
std::vector<string> nodes_starting_with_A;

int main(int argc, char* argv[])
{
  read_input();
  //std::pair<int_fast16_t, int_fast64_t> ans;
  std::cout << solve<int_fast16_t>() << std::endl;
  return 0;

  string s = "AAA";
  if(argc > 1)
  {
    int const idx = std::stoi(argv[1]);
    s = nodes_starting_with_A.at(idx);
  }

  int i{0}, ans{0};
  while(true)
  {
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
  std::getline(std::cin, line); /* ignore empty line */
  while(std::getline(std::cin, line))
  {
    std::string org   = line.substr(0, 3);
    std::string left  = line.substr(7, 3);
    std::string right = line.substr(12, 3);
    M.insert(std::make_pair(org, std::make_pair(left, right)));
    if(org.back()=='A') nodes_starting_with_A.push_back(org);
  }
}

template<typename integer> integer solve(bool is_part_two)
{
  integer ans{0};
  int i{0};
  string s{"AAA"};
  assert(!is_part_two);
  for(;;)
  {
    if(s=="ZZZ") break;

    if(instructions[i]=='L')
      s = M.at(s).first;
    else if(instructions[i]=='R')
      s = M.at(s).second;
    else
      throw std::domain_error(std::format("Instruction is neither L nor R: {}.", instructions[i]));

    i = (i+1)%static_cast<int>((instructions.length()));
    ans++;
  }

  return ans;
}
