#include <bits/stdc++.h>

int main()
{
  std::vector<std::string> pattern;
  std::string line;
  std::pair<int, int> ans{0,0};

  auto find_reflection = [&pattern](bool dimflag, bool two) -> int {
    const int DIM = dimflag ? pattern[0].size() : pattern.size(), ODIM = dimflag ? pattern.size() : pattern[0].size();
    for(int x{1}; x < DIM; x++)
    {
      int y = x, z = x-1;
      int deltas = 0;
      while(--y >= 0 and ++z < DIM) for(int j{0}; j < ODIM; j++)
      {
        if(dimflag) deltas += pattern[j][y] != pattern[j][z];
        else        deltas += pattern[y][j] != pattern[z][j];
      }

      if( two and deltas == 1) return x;
      if(!two and deltas == 0) return x;
    }
    return 0;
  };

  while(std::getline(std::cin, line))
  {
    std::cout << line << std::endl;
    if(line.empty())
    {
      ans.first  += find_reflection(1,0) + 100*find_reflection(0, 0);
      ans.second += find_reflection(1,1) + 100*find_reflection(0, 1);

      pattern.clear();
      continue;
    }

    pattern.push_back(line);
  }

  ans.first  += find_reflection(1,0) + 100*find_reflection(0,0);
  ans.second += find_reflection(1,1) + 100*find_reflection(0,1);
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}
