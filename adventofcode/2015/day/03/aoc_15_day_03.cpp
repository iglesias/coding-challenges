#include <cassert>
#include <iostream>
#include <set>
#include <string>

template<class T1, class T2>
std::pair<T1, T2> operator+(std::pair<T1, T2> const& lhs, std::pair<T1, T2> const& rhs)
{
  return {lhs.first+rhs.first, lhs.second+rhs.second};
}

template<class T1, class T2>
std::pair<T1, T2>& operator+=(std::pair<T1, T2>& lhs, std::pair<T1, T2> const& rhs)
{
  lhs = lhs + rhs;
  return lhs;
}

int main()
{
  auto const read = [](){ std::string line; std::getline(std::cin, line); return line; };
  std::string const input = read();

  using location_t = std::pair<int, int>;

  auto const dirchar2Delta = [](char c) -> location_t {
    switch(c){
      case '>':
        return {0,+1};
      case 'v':
        return {+1,0};
      case '<':
        return {0,-1};
      case '^':
        return {-1,0};
      default:
        assert(false);
    }
  };

  std::set<location_t> visited_houses;
  std::pair<location_t, location_t> locations{{0,0}, {0,0}};

  visited_houses.emplace(0,0);
  for(std::size_t i = 0; i < input.length(); i++){
    locations.first += dirchar2Delta(input[i]);
    visited_houses.insert(locations.first);
  }
  std::cout << "Part one: " << visited_houses.size() << "\n";

  visited_houses.clear();
  visited_houses.emplace(0,0);
  locations.first = {0,0};
  for(std::size_t i = 0; i < input.length(); i++){
    location_t& current_location = i%2 ? locations.first : locations.second;
    current_location += dirchar2Delta(input[i]);
    visited_houses.insert(current_location);
  }
  std::cout << "Part two: " << visited_houses.size() << "\n";

}
