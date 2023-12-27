#include <bits/stdc++.h>

void read_input();
std::pair<int, int> ans;
void solve();

std::vector<std::string> m;
int R, C;

int main()
{
  read_input();
  solve();
}

std::map<size_t, long long> visited;

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    m.push_back(line);
    R++;
    C = (int)line.length();
  }

  for(int r=0; r<R; r++) std::cout << m[r] << '\n';
}

long long cycle_number;

void cycle();

void try_wrap()
{
  size_t h = std::hash<std::string>{}(m[0]);
  for(int r=1; r<R; r++) h ^= std::hash<std::string>{}(m[r]);
  if(visited.contains(h))
  {
    for(int i = 0; i < (1000000000-cycle_number)%visited.at(h); i++) cycle();
    for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans.second += (R-r);
    std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
    std::exit(0);
  }
  else
  {
    visited.insert(std::make_pair(h, cycle_number));
  }
}

void tilt_north()
{
  for(int r=0;r<R;r++) for(int c=0;c<C;c++)
  {
    if(m[r][c]=='O' and r>0 and m[r-1][c]=='.')
    {
      int dr=r-1;
      while(dr > 0 and m[dr-1][c] == '.') dr -= 1;
      m[r][c] = '.';
      m[dr][c] = 'O';
    }
  }
}

void tilt_west()
{
  for(int r=0;r<R;r++) for(int c=0;c<C;c++)
  {
    if(m[r][c]=='O' and c>0 and m[r][c-1]=='.')
    {
      int dc=c-1;
      while(dc > 0 and m[r][dc-1] == '.') dc -= 1;
      m[r][c] = '.';
      m[r][dc] = 'O';
    }
  }
}

void tilt_south()
{
  for(int r=R-1;r>=0;r--) for(int c=0;c<C;c++)
  {
    if(m[r][c]=='O' and r<R-1 and m[r+1][c]=='.')
    {
      int dr=r+1;
      while(dr < R-1 and m[dr+1][c] == '.') dr += 1;
      m[r][c] = '.';
      m[dr][c] = 'O';
    }
  }
}

void tilt_east()
{
  for(int r=0;r<R;r++) for(int c=C-1;c>=0;c--)
  {
    if(m[r][c]=='O' and c<C-1 and m[r][c+1]=='.')
    {
      int dc=c+1;
      while(dc < C-1 and m[r][dc+1] == '.') dc += 1;
      m[r][c] = '.';
      m[r][dc] = 'O';
    }
  }
}

void cycle()
{
  tilt_north();
  tilt_west();
  tilt_south();
  tilt_east();
}

void solve()
{
  tilt_north();
  for(int r=0;r<R;r++) for(int c=0;c<C;c++) if(m[r][c]=='O') ans.first += (R-r);
  tilt_west();
  tilt_south();
  tilt_east();
  cycle_number = 1;

  for(long long i=0; i<1000000000; i++)
  {
    cycle();
    cycle_number++;
    try_wrap();
  }
}
