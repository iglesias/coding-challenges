#include <bits/stdc++.h>

int constexpr RIGHT = 0;
int constexpr DOWN  = 1;
int constexpr LEFT  = 2;
int constexpr UP    = 3;
int constexpr NDIRS = 4;

struct beam
{
  // row, column, direction
  int r, c, d;
  beam(int _r, int _c, int _d) : r(_r), c(_c), d(_d) {}
};

// contraption
using grid = std::vector<std::string>;

grid read();

int part_one(grid M, beam starting_beam={0,0,RIGHT});
int part_two(grid M);

void print(grid const& X)
{
  for(int r=0; r<int(X.size()); r++) std::cout << X[r] << '\n';
  std::cout << std::endl;
}

int main()
{

  auto M = read();
  std::cout << "Part one: " << part_one(M) << '\n';
  std::cout << "Part two: " << part_two(M) << '\n';
}

grid read()
{
  grid M;
  std::string line;
  while(std::getline(std::cin, line)) M.push_back(std::move(line));
  return M;
}

char dirInt2Char(int i)
{
  switch(i){
  case RIGHT:
    return '>';
  case DOWN:
    return 'v';
  case LEFT:
    return '<';
  case UP:
    return '^';
  }
  return '\'';
}

int part_one(grid M, beam starting_beam)
{
  int const R{int(M.size())}, C{int(M.at(0).size())};
  // grid to mark energized cells
  auto O = M;
  // mask to record directions taken in the grid
  std::vector<std::vector<std::bitset<NDIRS>>> P;
  for(int r=0; r<R; r++) P.push_back(std::vector<std::bitset<NDIRS>>(C, 0b0000));

  std::queue<beam> beams;
  beams.push(starting_beam);

  while(!beams.empty())
  {
    auto b = beams.front();
    beams.pop();
    bool f = true;
    while(f)
    {
      O[b.r][b.c] = '#';
      // move beam
      switch(M[b.r][b.c]){
      // empty space
      case '.':
      case '2':
      case '3':
      case '4':
      case '>':
      case 'v':
      case '<':
      case '^':
        // check in mask if beam should stop
        if(P[b.r][b.c].test(b.d)){
          f = false;
          break;
        }
        // mark
        M[b.r][b.c] = dirInt2Char(b.d);
        P[b.r][b.c].set(b.d);
        if(P[b.r][b.c].count() > 1) M[b.r][b.c] = char('0' + P[b.r][b.c].count());
        // update position
        switch(b.d){
        case RIGHT:
          if(b.c == C-1) f = false;
          else           b.c++;
          break;
        case DOWN:
          if(b.r == R-1) f = false;
          else           b.r++;
          break;
        case LEFT:
          if(b.c == 0)   f = false;
          else           b.c--;
          break;
        case UP:
          if(b.r == 0)   f = false;
          else           b.r--;
          break;
        }
        break;
      // mirrors
      case '/':
        if(b.d==RIGHT){
          if(b.r == 0)   f = false;
          else           { b.r--; b.d = UP; }
        }else if(b.d==DOWN){
          if(b.c == 0)   f = false;
          else           { b.c--; b.d = LEFT; }
        }else if(b.d==LEFT){
          if(b.r == R-1) f = false;
          else           { b.r++; b.d = DOWN; }
        }else if(b.d==UP){
          if(b.c == C-1) f = false;
          else           { b.c++; b.d = RIGHT; }
        }
        break;
      case '\\':
        if(b.d==RIGHT){
          if(b.r == R-1) f = false;
          else           { b.r++; b.d = DOWN; }
        }else if(b.d==DOWN){
          if(b.c == C-1) f = false;
          else           { b.c++; b.d = RIGHT; }
        }else if(b.d==LEFT){
          if(b.r == 0)   f = false;
          else           { b.r--; b.d = UP; }
        }else if(b.d==UP){
          if(b.c == 0)   f = false;
          else           { b.c--; b.d = LEFT; }
        }
        break;
      // splitters
      case '|':
        // either flat side
        if(b.d == RIGHT or b.d == LEFT){
          bool can_up  = b.r > 0;
          bool can_down = b.r < R-1;
          if(can_up and can_down){
            beams.emplace(b.r+1, b.c, DOWN);
            b.r--; b.d = UP;
          } else if (can_up) {
            b.r--; b.d = UP;
          } else if (can_down) {
            b.r++; b.d = DOWN;
          }
        // or pointy end
        }else{
          if(b.d == DOWN){
            if(b.r == R-1) f = false;
            else           b.r++;
          }else if(b.d == UP){
            if(b.r == 0)   f = false;
            else           b.r--;
          }
        }
        break; // case '|'
      case '-':
        switch(b.d){
        // pointy ends
        case RIGHT:
          if(b.c == C-1) f = false;
          else           b.c++;
          break;
        case LEFT:
          if(b.c == 0) f = false;
          else         b.c--;
          break;
        // flat sides
        case UP:
        case DOWN:
          bool can_left  = b.c > 0;
          bool can_right = b.c < C-1;
          if(can_left and can_right){
            beams.emplace(b.r, b.c+1, RIGHT);
            b.c--; b.d = LEFT;
          } else if (can_left) {
            b.c--; b.d = LEFT;
          } else if (can_right) {
            b.c++; b.d = RIGHT;
          }
        }
      }
    }
  }

  // count energized cells
  int ans = 0;
  for(int r=0; r<R; r++) for(int c=0; c<C; c++) if(O[r][c] == '#') ans++;
  return ans;
}

int part_two(grid M)
{
  int const R{int(M.size())}, C{int(M.at(0).size())};
  std::vector<std::future<int>> values(2*R+2*C);
  std::size_t i = 0;
  for(int r=0; r<R; r++){
    values[i++] = std::async(part_one, M, beam(r, 0, RIGHT));
    values[i++] = std::async(part_one, M, beam(r, C-1, LEFT));
  }
  for(int c=0; c<C; c++){
    values[i++] = std::async(part_one, M, beam(0, c, DOWN));
    values[i++] = std::async(part_one, M, beam(R-1, c, UP));
  }
  int ans = 0;
  for(auto& future : values) ans = std::max(ans, future.get());
  return ans;
}
