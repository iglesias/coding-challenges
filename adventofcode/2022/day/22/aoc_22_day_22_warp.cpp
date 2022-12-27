#include <bits/stdc++.h>

// 90293 is too low

std::array<char[151], 200> map; int N;
std::string path;
std::string fname;

void read_input(const std::string &fname);

namespace
{
  struct p { int r, c; p() : r(0), c(0) {} p(int ar, int ac) : r(ar), c(ac) {} };

//  void print(const p &ap) { std::cout << "(" << ap.r << ", " << ap.c << ")\n"; }

  struct instruction
  {
    enum{MOVE, ROTATE} tag;
    union
    {
      int n;
      char c;
    };
  };
}

::p find_begin_in_map()
{
  int c = std::find(map[0], map[0] + sizeof(map[0]) / sizeof(map[0][0]), '.') - map[0];
  return ::p(0, c);
}

void print_map();
bool get_next_instruction_from_path(::instruction &instruction);
void move(::p &pos, int n, bool part_two=false);
void rotate(::p &pos, char c);
void reset_instructions();

int part_one();
int part_two();

int main(int argc, char* argv[])
{
  if(argc<2)
  {
    fname = "aoc_day22_map.1.in";
  }
  else
  {
    fname = argv[1];
  }

  std::cout << "Part One: " << part_one() << '\n';
  std::cout << "Part Two: " << part_two() << '\n';
  print_map();
}

void read_input(const std::string &fname)
{
  std::ifstream fstream;
  auto count_num_lines = [&]() -> int
  {
    int num_lines{0};
    std::string line;
    while(std::getline(fstream, line)) num_lines++;
    return num_lines;
  };

  fstream.open(fname);
  const int num_lines{count_num_lines()};
  fstream.close();

  // the map has (num_lines-2) rows
  fstream.open(fname);
  std::string line;
  for(int i{0}; i<num_lines-2; i++)
  {
    assert(std::getline(fstream, line));
    //std::strcpy(map[i], line.c_str());
    for(size_t j{0}; j<line.length(); j++) map[i][j] = line[j];
    for(size_t j{line.length()}; j<150; j++) map[i][j] = ' ';
    map[i][150] = '\0';
  }

  assert(std::getline(fstream, line));
  assert(line.empty());
  assert(std::getline(fstream, line));
  path = line;

  fstream.close();

  N = num_lines-2;
}

void print_map()
{
  for(int i{0}; i<N; i++)
    std::cout << map[i] << '\n';
}

// reading instructions from path.
int idx{0};
void reset_instructions()
{
  idx = 0;
}
bool get_next_instruction_from_path(::instruction &instruction)
{
//  std::cout << ">> path=" << path << ' ' << idx << '\n';
  if(idx>=static_cast<int>(path.size())) return false;

  if(path[idx]=='L' or path[idx]=='R')
  {
    instruction.tag = ::instruction::ROTATE;
    instruction.c = path[idx++];
  }
  else
  {
    int num_tiles{0};
    while(idx<static_cast<int>(path.size()) and path[idx]!='L' and path[idx]!='R')
    {
      assert('0'<=path[idx] and path[idx]<='9');
      num_tiles *= 10;
      num_tiles += (path[idx]-'0');
      idx++;
    }
    instruction.tag = ::instruction::MOVE;
    instruction.n = num_tiles;
  }

  return true;

}

bool part_one_warp(::p &pos, const ::p &dir);
bool part_two_warp(::p &pos, ::p &dir, char &dirc);

::p dirc2dir(char dirc)
{
  ::p dir;
  switch(dirc)
  {
  case '>':
    dir.r =  0;
    dir.c = +1;
    break;
  case '<':
    dir.r =  0;
    dir.c = -1;
    break;
  case 'v':
    dir.r = +1;
    dir.c =  0;
    break;
  case '^':
    dir.r = -1;
    dir.c =  0;
    break;
  default:
    assert(false);
  }
  return dir;
}

void move(::p &pos, int n, bool part_two)
{
  //std::cout << ">> move(pos, " << n << ")\n";


  char dirc{map[pos.r][pos.c]};
  ::p dir{dirc2dir(dirc)};

  /*
  std::cout << ">>>> pos=";
  ::print(pos);
  std::cout << ">>>> dir=";
  ::print(dir);
  */

  auto in_map = [](const ::p &p) -> bool
  {
    return p.r>=0 and p.r<N and p.c>=0 and p.c<150;
  };

  // move while possible
  for(int i{0}; i<n; i++)
  {
    ::p trypos(pos.r+dir.r, pos.c+dir.c);
//    std::cout << ">>>> map[...][...]=" << map[trypos.r][trypos.c] << std::endl;

    if(in_map(trypos) && map[trypos.r][trypos.c]=='#') break;

    // check if warps
    if(!in_map(trypos) or map[trypos.r][trypos.c]==' ')
    {
      if(part_two)
      {
        if (part_two_warp(pos, dir, dirc)) break;
      }
      else
      {
        if (part_one_warp(pos, dir)) break;
      }
    }
    else
    {
      // if it doesn't warp, just add the direction
      pos.r += dir.r;
      pos.c += dir.c;
      assert(trypos.r==pos.r and trypos.c==pos.c);
    }

    map[pos.r][pos.c] = dirc;
    //print_map();
  }
}

void rotate(::p &pos, char c)
{
  //std::cout << ">> rotate(pos, " << c << ")\n";
  const char dirc{map[pos.r][pos.c]};
  switch(dirc)
  {
    case '>':
      if(c=='R') map[pos.r][pos.c] = 'v';
      else { assert(c=='L'); map[pos.r][pos.c] = '^'; }
      break;
    case '<':
      if(c=='R') map[pos.r][pos.c] = '^';
      else { assert(c=='L'); map[pos.r][pos.c] = 'v'; }
      break;
    case 'v':
      if(c=='R') map[pos.r][pos.c] = '<';
      else { assert(c=='L'); map[pos.r][pos.c] = '>'; }
      break;
    case '^':
      if(c=='R') map[pos.r][pos.c] = '>';
      else { assert(c=='L'); map[pos.r][pos.c] = '<'; }
      break;
    default:
      assert(false);
  }
}

int decode(const ::p &pos)
{
  int ans = 1000*(pos.r+1) + 4*(pos.c+1);
  switch(map[pos.r][pos.c])
  {
    case '>':
      ans += 0;
      break;
    case '<':
      ans += 2;
      break;
    case 'v':
      ans += 1;
      break;
    case '^':
      ans += 3;
      break;
    default:
      assert(false);
  
  }
  return ans;
}

int solve(const bool part_two=false)
{
  read_input(fname);

  ::p begin = find_begin_in_map();
  map[begin.r][begin.c] = '>';
  //print_map();

  ::instruction instruction;
  ::p pos{begin};
  reset_instructions();
  while(get_next_instruction_from_path(instruction))
  {
    switch(instruction.tag)
    {
      case ::instruction::ROTATE:
        rotate(pos, instruction.c);
        break;
      case ::instruction::MOVE:
        move(pos, instruction.n, part_two);
        break;
      default:
        assert(false);
    }
  }

  return decode(pos);
}

int part_one()
{
  return solve();
}

int part_two()
{
  const bool part_two{true};
  return solve(part_two);
}

bool part_one_warp(::p &pos, const ::p &dir)
{
  if(dir.r==+1)
  {
    assert(dir.c==0);
    assert(pos.c>=0 and pos.c<150);
    int r{0};
    while(map[r][pos.c] == ' ') r++;
    assert(r<N);
    if(map[r][pos.c] == '#') return true;
    pos.r = r;
  }
  else if(dir.r==-1)
  {
    assert(dir.c==0);
    assert(pos.c>=0 and pos.c<150);
    // part one
    int r{pos.r};
    while(r<N && map[r][pos.c] != ' ') r++;
    if(r==0) return true;
    if(map[r-1][pos.c] == '#') return true;
    pos.r = r-1;
    //
  }
  else if(dir.c==+1)
  {
    assert(dir.r==0);
    assert(pos.r>=0 and pos.r<N);
    // part one
    int c{0};
    while(map[pos.r][c] == ' ') c++;
    assert(c<150);
    if(map[pos.r][c] == '#') return true;
    pos.c = c;
  }
  else if(dir.c==-1)
  {
    assert(dir.r==0);
    assert(pos.r>=0 and pos.r<N);
    // part one
    int c{pos.c};
    while(c<150 && map[pos.r][c] != ' ') c++;
    if(c==0) return true;
    if(map[pos.r][c-1] == '#') return true;
    pos.c = c-1;
    //
  }
  return false;
}


bool part_two_warp(::p &pos, ::p &dir, char &dirc)
{
  const int K{50}; // part two puzzle input cube size
  if(dir.r==+1)
  {
    assert(dir.c==0);
    assert(pos.c>=0 and pos.c<150);

    if(fname!="aoc_day22_map.2.in")
    {
      // sample input 5 goes to 2 facing up
      int r = 4*1 + 3;
      int c = 4*0 + (4*3-1-pos.c);
      dirc = '^';
      //TODO rest of the parts and guard this part
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      dir = dirc2dir(dirc);
      return false;
    }

    // puzzle input - warp cases going down (dir.r==+1)
    if(pos.r<K)
    {
      // face 1
      assert(2*K<=pos.c);
      // 1 v -> 3 <, first col -> last row
      int r = 4*K-1-pos.c;
      int c = 2*K-1;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(K<=pos.r && pos.r<2*K && K<=pos.c && pos.c<2*K);
      assert(dirc=='v');
      dirc = '<';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<3*K)
    {
      // face 4
      assert(pos.c<2*K && K<pos.c);
      // 4 v -> 6 <, first col -> first row
      int c = K-1;
      int r = pos.c+2*K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      assert(3*K<=pos.r && pos.c<K);
      assert(dirc=='v');
      dirc = '<';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<4*K)
    {
      // face 6
      assert(pos.c<K);
      // 6 v -> 1 v, first col -> first col
      int r = 0;
      int c = 2*K+pos.c;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      assert(pos.r<K && 2*K<=pos.c);
      assert(dirc=='v');
      dir = dirc2dir(dirc);
    }
    else
    {
      assert(false);
    }
  }
  else if(dir.r==-1)
  {
    assert(dir.c==0);
    assert(pos.c>=0 and pos.c<150);

    if(fname!="aoc_day22_map.2.in")
    {
      //TODO part two warping going up with sample input
      // placeholder equal to part one
      int r{pos.r};
      while(r<N && map[r][pos.c] != ' ') r++;
      if(r==0) return true;
      if(map[r-1][pos.c] == '#') return true;
      pos.r = r-1;
      return false;
    }

    // puzzle input - warp cases going up (dir.r==-1)
    if(2*K<=pos.c)
    {
      // face 1
      assert(pos.r<K);
      // 1 ^ -> 6 ^, first col -> first col
      int r  = 4*K-1;
      int c = pos.c-2*K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      assert(3*K<=pos.r && pos.c<K);
      assert(dirc=='^');
      dir = dirc2dir(dirc);
    }
    else if(K<=pos.c)
    {
      // face 2
      assert(pos.r<K);
      // 2 ^ -> 6 >, first col -> first row
      int r = pos.c+2*K;
      int c = 0;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      assert(3*K<=pos.r && pos.r<4*K && pos.c<K);
      assert(dirc=='^');
      dirc = '>';
      dir = dirc2dir(dirc);
    }
    else if(pos.c<K)
    {
      // face 5
      assert(2*K<=pos.r && pos.r<3*K);
      // 5 ^ -> 3 >, first col -> first row
      int r = K+pos.c;
      int c = K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c; 
      assert(K<=pos.r && pos.r<2*K && K<=pos.c && pos.c<2*K);
      assert(dirc=='^');
      dirc = '>';
      dir = dirc2dir(dirc);
    }
    else
    {
      assert(false);
    }
  }
  else if(dir.c==+1)
  {
    assert(dir.r==0);
    assert(pos.r>=0 and pos.r<N);

    if(fname!="aoc_day22_map.2.in")
    {
      // sample input 4 goes to 6 facing down
      int r = 4*2 + 0;
      int c = 4*3 + (4*2-1-pos.r);
      //TODO rest of the parts and guard this part
      if(map[r][c] == '#') return true;
      pos.c = c;
      pos.r = r;
      dirc = 'v';
      dir = dirc2dir(dirc);
      return false;
    }

    // puzzle input - warp cases going right (dir.c==+1)
    if(pos.r<K)
    {
      // face 1
      assert(2*K<pos.c);
      // 1 > -> 4 >, first row -> last row
      int r = 3*K-1-pos.r;
      int c = 2*K-1;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(2*K<=pos.r && pos.r<3*K && K<=pos.c && pos.c<2*K);
      assert(dirc=='>');
    }
    else if(pos.r<2*K)
    {
      // face 3
      assert(pos.c<2*K && K<pos.c);
      // 3 > -> 1 ^, first row -> first col
      int r = K-1;
      int c = K+pos.r;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.r<K && 2*K<=pos.c);
      assert(dirc=='>');
      dirc = '^';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<3*K)
    {
      // face 4
      assert(K<=pos.c && pos.c<2*K);
      // 4 > -> 1 <, first row -> last row
      int r = 3*K-1-pos.r;
      int c = 3*K-1;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.r<K && 2*K<=pos.c && pos.c<3*K);
      assert(dirc=='>');
      dirc = '<';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<4*K)
    {
      // face 6
      assert(pos.c<K);
      // 6 > -> 4 ^, first row -> first col
      int r = 3*K-1;
      int c = pos.r-2*K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(2*K<=pos.r && pos.r<3*K && K<=pos.c && pos.c<2*K);
      assert(dirc=='>');
      dirc = '^';
      dir = dirc2dir(dirc);
    }
    else
    {
      assert(false);
    }
  }
  else if(dir.c==-1)
  {
    assert(dir.r==0);
    assert(pos.r>=0 and pos.r<N);

    if(fname!="aoc_day22_map.2.in")
    {
      //TODO part two warping going up with sample input
      // placeholder equal to part one
      int c{pos.c};
      while(c<150 && map[pos.r][c] != ' ') c++;
      if(c==0) return true;
      if(map[pos.r][c-1] == '#') return true;
      pos.c = c-1;
      return false;
    }

    // puzzle input - warp cases going left (dir.c==-1)
    if(pos.r<K)
    {
      // face 2
      assert(pos.c<2*K && K<=pos.c);
      // 2 < -> 5 >, first row -> last row
      int r = 3*K-1-pos.r;
      int c = 0;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.c<K && 2*K<=pos.r && pos.r<3*K);
      assert(dirc=='<');
      dirc = '>';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<2*K)
    {
      // face 3
      assert(pos.c<2*K && K<=pos.c);
      // 3 < -> 5 v, first row -> first col
      int r = 2*K;
      int c = pos.r-K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.c<K && 2*K<=pos.r && pos.r<3*K);
      assert(dirc=='<');
      dirc = 'v';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<3*K)
    {
      // face 5
      assert(pos.c<K);
      // 5 < -> 2 >, first row -> last row
      int r = 3*K-1-pos.r;
      int c = K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.r<K && K<=pos.c && pos.c<2*K);
      assert(dirc=='<');
      dirc = '>';
      dir = dirc2dir(dirc);
    }
    else if(pos.r<4*K)
    {
      // face 6
      assert(pos.c<K);
      // 6 < -> 2 v, first row -> first col
      int r = 0;
      int c = pos.r-2*K;
      if(map[r][c] == '#') return true;
      pos.r = r;
      pos.c = c;
      assert(pos.r<K && K<=pos.c && pos.c<2*K);
      assert(dirc=='<');
      dirc = 'v';
      dir = dirc2dir(dirc);
    }
    else
    {
      assert(false);
    }
  }
  return false;
}
