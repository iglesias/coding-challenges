#include <bits/stdc++.h>

// ordered intervals of free memory
//
// [0, n-1]
//
// 0. Didn't consider intervals "touching" without overlapping.
// 1. TLE -> use linked list instead of std::set, keeping the
//    binary search tree (BST) updated may be too expensive.


using ii = std::pair<int,int>;


void solve(int n, int q);

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);
  int n, q;
  std::cin >> n >> q;

  solve(n, q);
}

std::forward_list<ii> mem;

void falloc(int l)
{
  for(auto it{mem.cbefore_begin()}; std::next(it)!=mem.cend(); it++)
  {
    auto const memi{*std::next(it)};
    int const memi_size{memi.second-memi.first+1};
    if(memi_size>=l)
    {
      mem.erase_after(it);
      if(memi_size>l)
      {
        assert(memi.first+l<=memi.second);
        mem.emplace_after(it, memi.first+l, memi.second);
      }
      std::cout << memi.first << '\n';
      return;
    }
  }

  std::cout << -1 << '\n';
}

//TODO generalize to work both with std::set and std::forward_list
//simultaneously.
void print(std::forward_list<ii> const &mem)
{
  std::cout << "mem: ";
  for(auto const &memi : mem)
  {
    std::cout << "(" << memi.first << ", " << memi.second << ") ";
  }

  std::cout << std::endl;
}


void ffree(int x, int l)
{
  // Look for "digitally overlapping" intervals.
  // Actually, not only overlapping, but "direct
  // neighbors" are merged as well.

  std::forward_list<ii>::const_iterator it{mem.cbefore_begin()};

  // skip intervals too far to the left.
  while(std::next(it)!=mem.cend())
  {
    if(x-1<=std::next(it)->second) break;
    it++;
  }

///  std::cout << "  it->first=" << it->first << " x=" << x << " l=" << l << std::endl;
  if(std::next(it)==mem.cend() or x+l<std::next(it)->first)
  {
    mem.emplace_after(it, x, x+l-1);
    std::cout << l << '\n';
    return;
  }

///  std::cout << "    overlapping interval found" << std::endl;

  // Merge overlapping intervals and
  // compute #cells actually freed.
  //
  // at this moment, 'it' points at the left-most
  // interval with some overlap.
  ii to_insert;
  to_insert.first = std::min(x, std::next(it)->first);
  to_insert.second = std::max(x+l-1, std::next(it)->second);

  int actual_l{l};
  actual_l -= std::min(std::next(it)->second,x+l-1)-std::max(std::next(it)->first,x)+1;

  mem.erase_after(it);

  // go through all intervals completely
  // contained.
///  std::cout << "    it->second=" << it->second  << " x=" << x << " l=" << l << std::endl;
  while(std::next(it)!=mem.cend() and std::next(it)->second<=x+l-1)
  {
///    std::cout << "    interval completely contained!" << std::endl;
    actual_l -= std::next(it)->second-std::next(it)->first+1;
    mem.erase_after(it);
  }

  // check if there's a right-most
  // overlapping interval not completely
  // contained.
  if(std::next(it)!=mem.cend() and std::next(it)->first<=x+l)
  {
///    std::cout << "    right-most overlapping not completely contained!" << std::endl;
    actual_l -= x+l-std::next(it)->first;
    to_insert.second = std::next(it)->second;
    mem.erase_after(it);
  }

  mem.insert_after(it, to_insert);
  std::cout << actual_l << '\n';
}

void solve(int n, int q)
{
  mem.emplace_front(0, n-1);

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

///    print(mem);
  }
}