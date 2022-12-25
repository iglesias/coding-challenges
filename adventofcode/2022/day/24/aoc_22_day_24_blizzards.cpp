#include <bits/stdc++.h>

using coord = int;

// anonymous
namespace {
  struct p
  {
    p() : r(0), c(0) {}
    p(coord _r, coord _c) : r(_r), c(_c) {}
    bool operator==(const p& q) const
    {
      return r == q.r and c == q.c;
    }
    bool operator<(const p& q) const
    {
      return (r == q.r) ? c < q.c : r < q.r;
    }
    friend std::ostream& operator<<(std::ostream& os, const p& a_p)
    {
      os << "(" << a_p.r << ", " << a_p.c << ")";
      return os;
    }
    // row, column
    coord r, c;
  };
}

using blizzards = std::multimap<::p, char>;

class bfs
{
 public:
  static void search(blizzards &poss, bool reverse=false);
 private:
  struct state
  {
    ::p dude;
    int min;
    bool operator==(const state& other) const
    {
      return min == other.min && dude == other.dude;
    }
    bool operator<(const state& other) const
    {
      return (min == other.min) ? dude < other.dude : min < other.min;
    }
  };
};

// globals
std::vector<std::string> map;
int M, N;

// functions
blizzards read_input();
void print(const blizzards&);
void move_blizzards(blizzards&);

int main()
{
  auto poss = read_input();
  //print(poss);
  /*
  for(int min{0}; min<18; min++)
  {
    move_blizzards(poss);
    print(poss);
  }
  */
  bfs::search(poss);
  bfs::search(poss, true);
  bfs::search(poss);
}

blizzards read_input()
{
  std::string line;
  int r{0};
  while(std::getline(std::cin, line))
  {
    if(r!=0) assert(N==static_cast<int>(line.length()));
    else     N = line.length();
    r++;
    map.push_back(line);
  }
  M = r;

  blizzards poss;
  for(int i{1}; i<M-1; i++)
  {
    for(int j{1}; j<N-1; j++)
    {
      if(map[i][j] != '.')
      {
        poss.insert(std::make_pair(::p(i,j), map[i][j]));
      }
    }
  }
  return poss;
}

void print(const blizzards &poss)
{
  // clear ground
  for(int i{0}; i<M; i++) for(int j{0}; j<N; j++) map[i][j] = '.';
  // walls
  for(int i{0}; i<M; i++) { map[i][0] = '#'; map[i][N-1] = '#'; }
  for(int j{2}; j<N-1; j++) { map[0][j] = '#'; map[M-1][N-1-j] = '#'; }

  for(const auto &kv : poss)
  {
    if(poss.count(kv.first)>1)
    {
      map[kv.first.r][kv.first.c] = char('0'+poss.count(kv.first));
      continue;
    }

    assert(poss.count(kv.first)==1);
    map[kv.first.r][kv.first.c] = kv.second;
  }

  for(int i{0}; i<M; i++)
    std::cout << map[i] << '\n';
  std::cout << '\n';
}

::p dir2delta(char dir)
{
  switch(dir)
  {
    case '>':
      return ::p(+0, +1);
    case '<':
      return ::p(+0, -1);
    case '^':
      return ::p(-1, +0);
    case 'v':
      return ::p(+1, +0);
    default:
      assert(false);
  }
  assert(false);
  return ::p();
}

bool inside_bounds(const ::p &p)
{
  return p.r>0 and p.r<M-1 and p.c>0 and p.c<N-1;
}

void move_blizzards(blizzards &poss)
{
  blizzards next_poss;
  for(const auto &kv : poss)
  {
    const ::p delta{dir2delta(kv.second)};
    ::p trypos(kv.first.r+delta.r, kv.first.c+delta.c);

    if(!inside_bounds(trypos))
    {
      // conservation of blizzard energy
      if(trypos.r==M-1)
        trypos.r = 1;
      else if(trypos.r==0)
        trypos.r = M-2;
      else if(trypos.c==N-1)
        trypos.c = 1;
      else if(trypos.c==0)
        trypos.c = N-2;
      else
        assert(false);
    }

    next_poss.insert(std::make_pair(trypos, kv.second));
  }

  poss = std::move(next_poss);
}

void bfs::search(blizzards &poss, bool reverse)
{
  state init;
  ::p goal;
  if(reverse)
  {
    init.dude.r = M-1;
    init.dude.c = N-2;
    goal.r = 0;
    goal.c = 1;
  }
  else
  {
    init.dude.r = 0;
    init.dude.c = 1;
    goal.r = M-1;
    goal.c = N-2;
  }
  init.min = 0;

  const int MAX_MIN{20};
  std::queue<state> q;
  q.push(init);
  std::set<state> visited;
  static int i{0};
  for(; true or i<MAX_MIN; i++)
  {
    std::cout << ">> bfs::search(...) minute " << i << "\n";
    std::queue<state> newq;
    assert(!q.empty());
    while(!q.empty())
    {
      auto s = q.front();
      q.pop();

      if(s.dude.r == goal.r && s.dude.c == goal.c)
      {
        std::cout << ">> bfs::search(...) goal reached!\n";
        return;
      }

      if(visited.find(s)!=visited.cend())
        continue;

      if(!(s.dude.r==init.dude.r and s.dude.c==init.dude.c) /* if not starting position and */ and !inside_bounds(s.dude))
        continue;

      if(poss.count(s.dude)>0)
        continue;

      std::vector<::p> deltas{{-1,+0},{+1,+0},{+0,-1},{+0,+1},{+0,+0}};
      for(const auto &delta : deltas)
      {
        state news;
        news.dude.r = s.dude.r+delta.r;
        news.dude.c = s.dude.c+delta.c;
        news.min = s.min+1;
        newq.push(news);
      }

      visited.insert(s);
    }
    move_blizzards(poss);
    q = std::move(newq);
  }
}
