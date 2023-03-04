#include <bits/stdc++.h>

// ordered intervals of free memory
//
// [0, n-1]


using ii = std::pair<int,int>;


void solve(int n, int q);

int main()
{
  int n, q;
  std::cin >> n >> q;

  solve(n, q);
}

std::set<ii> mem;

void falloc(int l)
{
  for(auto const& memi : mem)
  {
    const int memi_size{memi.second-memi.first+1};
    if(memi_size>=l)
    {
      if(memi_size>l)
      {
        assert(memi.first+l<=memi.second);
        mem.insert(std::make_pair(memi.first+l, memi.second));
      }
      std::cout << memi.first << '\n';
      mem.erase(memi);
      return;
    }
  }

  std::cout << -1 << '\n';
}

void ffree(int x, int l)
{
  // Look for "digitally overlapping" intervals.
  // Actually, not only overlapping, but "direct
  // neighbors" are merged as well.

  std::set<ii>::const_iterator it{mem.cbegin()};

  // skip intervals too far to the left.
  while(it!=mem.cend())
  {
    if(x-1<=it->second) break;
    it++;
  }

///  std::cout << "  it->first=" << it->first << " x=" << x << " l=" << l << std::endl;
  if(it==mem.cend() or x+l<it->first)
  {
    mem.insert(std::make_pair(x, x+l-1));
    std::cout << l << '\n';
    return;
  }

///  std::cout << "    overlapping interval found" << std::endl;

  // Merge overlapping intervals and
  // compute #cells actually freed.
  //
  // it points at the left-most
  // interval with some overlap.
  ii to_insert;
  to_insert.first = std::min(x, it->first);
  to_insert.second = std::max(x+l-1, it->second);

  std::set<ii> to_erase;
  to_erase.insert(*it);

  int actual_l{l};
  actual_l -= std::min(it->second,x+l-1)-std::max(it->first,x)+1;

  it++;

  // go through all intervals completely
  // contained.
///  std::cout << "    it->second=" << it->second  << " x=" << x << " l=" << l << std::endl;
  while(it!=mem.cend() and it->second<=x+l-1)
  {
///    std::cout << "    interval completely contained!" << std::endl;
    to_erase.insert(*it);
    actual_l -= it->second-it->first+1;
    it++;
  }

  // check if there's a right-most
  // overlapping interval not completely
  // contained.
///  std::cout << "    it->first=" << it->first << " x+l=" << x+l << " it!=mem.cend()=" << (it!=mem.cend()) << std::endl;
  if(it!=mem.cend() and it->first-1<=x+l)
  {
///    std::cout << "   We are here!" << std::endl;
    to_erase.insert(*it);
    actual_l -= x+l-it->first;
    to_insert.second = it->second;
  }

  for(ii const& interval : to_erase) mem.erase(interval);
  mem.insert(to_insert);
  std::cout << actual_l << '\n';
}

void print(std::set<ii> const &mem)
{
  std::cout << "mem: ";
  for(auto const &memi : mem)
  {
    std::cout << "(" << memi.first << ", " << memi.second << ") ";
  }

  std::cout << std::endl;
}

void solve(int n, int q)
{
  mem.insert(std::make_pair(0, n-1));

  //TODO improve loop:
  for(int qi{0}; qi<q; qi++)
  {
    int op, l, x;
    std::cin >> op;

    switch(op)
    {
    case 1:
      std::cin >> l;
///      std::cout << "allocation " << l << std::endl;
      falloc(l);
      break;
    case 2:
      std::cin >> x >> l;
///      std::cout << "free " << x << ' ' << l << std::endl;
      ffree(x, l);
      break;
    default:
      assert(false);
    }

    //print(mem);
  }
}
