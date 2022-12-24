#include <bits/stdc++.h>

constexpr int M{10000};
constexpr int N{9};

constexpr int NUM_ROCK_TYPES{5};

using ull = unsigned long long int;

std::array<std::array<char, N>, M> chamber;
// -4 because the first rock appears so that its
// bottom edge is tree units above from the floor.
int g_next_start_row{M-1-4};
// the tower of rocks will be these many units tall.
int g_ans{0};

ull g_rock_number{0};
int g_pattern_idx{0};

namespace {

  struct p {
    int row, col;
    p() : row(0), col(0) {}
    p(int arow, int acol) : row(arow), col(acol) {}
    p(const p &o) : row(o.row), col(o.col) {}
  };

  using rock = std::vector<p>;
}

void init_chamber();

// n rows starting from the bottom;
void print_chamber(int n);
// n rows starting from the top;
void print_top(int n);

void add_to_chamber(const ::rock &r, char c='#');
void remove_from_chamber(const ::rock &r);

// chamber, 
::rock make_rock();

void simulate(::rock &r);

// -> returns (tall, #rocks)
std::optional<std::pair<int, int>> detect_cycle();

void sample_simulation()
{
  ::rock rock{make_rock()};
  add_to_chamber(rock, '@');
  print_chamber(5);
  remove_from_chamber(rock);
  simulate(rock);

  rock = make_rock();
  add_to_chamber(rock, '@');
  print_chamber(10);
  remove_from_chamber(rock);
  simulate(rock);

  rock = make_rock();
  add_to_chamber(rock, '@');
  print_chamber(10);
  remove_from_chamber(rock);
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  simulate(rock);

  rock = make_rock();
  add_to_chamber(rock, '@');
  print_chamber(30);
}

int main(int argc, char* argv[])
{
  init_chamber();

  /* INITIAL PRINTS
  print_chamber(5);
  std::cout << "\n\n";
  for(int i{0}; i<10; i++)
  {
    ::rock rock{make_rock()};
    add_to_chamber(rock);
    print_chamber(8*(i+1));
    std::cout << "\n" << i << "\n\n" << std::endl;
  }
  */

  //sample_simulation();
  //
  //while(g_next_start_row>2)
  /*
  while(g_next_start_row>9000-4)
  {
    ::rock rock{make_rock()};
    simulate(rock);
  }
  */

  ull target_rock_number{2022};
  if(argc>1)
    target_rock_number = std::stoull(argv[1]);
  std::cout << target_rock_number << std::endl;

  std::optional<std::pair<int, int>> cycle;
  while(g_rock_number<target_rock_number)
  {
    if(cycle = detect_cycle())
    {
      std::cout << "cycle tall=" << cycle->first << " #rocks=" << cycle->second << '\n';
      break;
    }

    ::rock rock{make_rock()};
    simulate(rock);
  }

  ull cycle_ans{0};
  if(g_rock_number<target_rock_number && cycle)
  {
    ull d = (target_rock_number-g_rock_number)/cycle->second;
    cycle_ans = d*cycle->first; 
    g_rock_number += d*cycle->second;

    std::cout << "replicating cycle tall=" << cycle_ans << '\n';
  }

  while(g_rock_number<target_rock_number)
  {
    ::rock rock{make_rock()};
    simulate(rock);
  }

  std::cout << "answer=" << g_ans+cycle_ans << '\n';
}

void init_chamber()
{
  for(int m{0}; m<M; m++)
    for(int n{0}; n<N; n++)
      chamber[m][n] = '.';

  // walls (to the sides)
  for(int m{0}; m<M-1; m++)
  {
    chamber[m][0]   = '|';
    chamber[m][N-1] = '|';
  }

  // bottom row
  for(int n{0}; n<N; n++)
    chamber[M-1][n] = '-';

  // corners
  chamber[M-1][0]   = '+';
  chamber[M-1][N-1] = '+';
}


// n rows starting from the bottom;
void print_chamber(int n)
{
  for(int m{M-n}; m<M; m++)
  {
    std::cout << std::setw(10);
    std::cout << m << "    ";
    for(int n{0}; n<N; n++)
      std::cout << chamber[m][n];

    std::cout << '\n';
  }
}

// n rows starting from the top;
void print_top(int n)
{
  for(int m{M-g_ans}; m<M-g_ans+n; m++)
  {
    std::cout << std::setw(10);
    std::cout << m << "    ";
    for(int n{0}; n<N; n++)
      std::cout << chamber[m][n];

    std::cout << '\n';
  }
}

::rock make_rock()
{
  ::rock rock;
  switch(g_rock_number%NUM_ROCK_TYPES)
  {
    case 0:
      // ####
      // from 3 because each rock appears so that its
      // left edge is two units away from the left wall.
      rock.emplace_back(g_next_start_row, 3);
      rock.emplace_back(g_next_start_row, 4);
      rock.emplace_back(g_next_start_row, 5);
      rock.emplace_back(g_next_start_row, 6);
      break;
    case 1:
      // .#.
      // ###
      // .#.
      // top-to-bottom:
      rock.emplace_back(g_next_start_row-2, 4);
      rock.emplace_back(g_next_start_row-1, 3);
      rock.emplace_back(g_next_start_row-1, 4);
      rock.emplace_back(g_next_start_row-1, 5);
      rock.emplace_back(g_next_start_row,   4);
      break;
      break;
    case 2:
      // ..#
      // ..#
      // ###
      rock.emplace_back(g_next_start_row-2, 5);
      rock.emplace_back(g_next_start_row-1, 5);
      rock.emplace_back(g_next_start_row,   5);
      rock.emplace_back(g_next_start_row,   4);
      rock.emplace_back(g_next_start_row,   3);
      break;
    case 3:
      // #
      // #
      // #
      // #
      rock.emplace_back(g_next_start_row-3, 3);
      rock.emplace_back(g_next_start_row-2, 3);
      rock.emplace_back(g_next_start_row-1, 3);
      rock.emplace_back(g_next_start_row,   3);
      break;
    case 4:
      // ##
      // ##
      rock.emplace_back(g_next_start_row-1, 4);
      rock.emplace_back(g_next_start_row-1, 3);
      rock.emplace_back(g_next_start_row,   4);
      rock.emplace_back(g_next_start_row,   3);
      break;
    default:
      assert(false);
  }

  return rock;
}

void add_to_chamber(const ::rock &r, char c)
{
  for(const auto &p : r)
    chamber[p.row][p.col] = c;
}

void remove_from_chamber(const ::rock &r)
{
  for(const auto &p : r)
    chamber[p.row][p.col] = '.';
}

void simulate(::rock &r)
{
  static std::string pattern;
  if(pattern.length()==0)
  {
    std::getline(std::cin, pattern);
    assert(pattern.length()>0);
  }

  int i{0};
  bool continue_falling{true};
  while(continue_falling)
  {
    p delta;
    if(i%2)
    {
      delta.row = 1;
      delta.col = 0;
      //std::cout << "Rock falls 1 unit";
    }
    else
    {
      //std::cout << "Jet of gas pushes rock ";
      delta.row = 0;
      switch(pattern[g_pattern_idx])
      {
        case '>':
          //std::cout << "right";
          delta.col = 1;
          break;
        case '<':
          //std::cout << "left";
          delta.col = -1;
          break;
        default:
          assert(false);
      }
      g_pattern_idx = (g_pattern_idx+1)%pattern.length();
    }

    bool possible_to_move{true};
    for(auto &p : r)
    {
      if(chamber[p.row+delta.row][p.col+delta.col]!='.')
      {
        possible_to_move = false;
      }
    }

    if(possible_to_move)
    {
      for(auto &p : r)
      {
        p.row += delta.row;
        p.col += delta.col;
      }
    }

    if(possible_to_move)
    {
      //std::cout << ":\n";
      ;
    }
    else
    {
      if(delta.row==1)
      {
        //std::cout << ", causing it to come to rest:\n";
        continue_falling = false;
      }
      else
      {
        assert(delta.row==0);
        assert(delta.col==1 or delta.col==-1);
        //std::cout << ", but nothing happens:\n";
      }
    }

    if(!continue_falling)
    {
      int min_row{M-1}, max_row{0};
      for(auto &p : r)
      {
        max_row = std::max(max_row, p.row);
        min_row = std::min(min_row, p.row);
      }

      // -4 because each rock appears so that its
      // bottom edge is tree units the highest rock.
      g_next_start_row = std::min(g_next_start_row, min_row-4);

      g_ans = std::max(g_ans, M-1-4-g_next_start_row);

      g_rock_number++;
    }

    if(continue_falling)
    {
      add_to_chamber(r, '@');
      //print_chamber(10);
      remove_from_chamber(r);
    }
    else
    {
      add_to_chamber(r, '#');
      //print_chamber(10);
    }

    i++;
  }
}

struct state
{
  int rock_type_idx;
  std::size_t hash_top_chamber;
  int pattern_idx;
  bool operator==(const state &other) const
  {
    return hash_top_chamber==other.hash_top_chamber && rock_type_idx==other.rock_type_idx && pattern_idx==other.pattern_idx;
  }
  bool operator<(const state &other) const
  {
    if(rock_type_idx==other.rock_type_idx)
    {
      if(pattern_idx==other.pattern_idx)
        return hash_top_chamber<other.hash_top_chamber;
      else
        return pattern_idx<other.pattern_idx;
    }
    else
      return rock_type_idx<other.rock_type_idx;
  }
};

std::optional<std::pair<int, int>> detect_cycle()
{
  static std::map<state, std::pair<int, int>> visited; // state -> {tall, after how many rocks}

  // we only run the state algo once to detect a cycle
  static bool work_done{false};
  if(work_done) return std::nullopt;

  // how many top rows to check for state equality.
  const int L{30};
  if(g_ans<L) return std::nullopt;

  state s;
  s.rock_type_idx = (g_rock_number%NUM_ROCK_TYPES);
  s.pattern_idx = g_pattern_idx;
  //
  // string used for hashing, I couldn't use the std::array<char, N>
  // from the chamber with std::hash directly.
  // At least we reuse the allocation in the string.
  std::string str;
  //copy tallest row in chamber to string for hashing.
  for(int i{0}; i<N; i++) str += chamber[M-g_ans][i];
  s.hash_top_chamber = std::hash<std::string>{}(str);
  for(int l{1}; l<L; l++)
  {
    //copy next row in chamber to string for hashing.
    for(int i{0}; i<N; i++) str[i] = chamber[M-g_ans+l][i];
    s.hash_top_chamber = s.hash_top_chamber ^ std::hash<std::string>{}(str);
  }

  // check if this state has already happened
  if(visited.find(s)!=visited.cend())
  {
    std::cout << ">> detect_cycle() match found!\n";
    std::cout << ">>   now:   tall=" << g_ans << " after #rocks=" << g_rock_number << "\n";
    std::cout << ">>   match: tall=" << visited.at(s).first << " after #rocks=" << visited.at(s).second << '\n';
    work_done = true;
    return std::make_pair(g_ans-visited.at(s).first, g_rock_number-visited.at(s).second);
  }

  // cache current state
  assert(visited.find(s)==visited.cend());
  visited.insert(std::make_pair(s, std::make_pair(g_ans, g_rock_number)));

  return std::nullopt;
}
