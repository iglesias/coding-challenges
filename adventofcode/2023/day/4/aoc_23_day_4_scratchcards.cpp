// Compiled flagging -std=c++23 with g++ (GCC) 13.2.1 20230801

#include <algorithm>
#include <format>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

int main()
{
  auto ans{std::make_pair(0,0)};
  std::vector<int> matching_numbers_per_card;
  std::string line;
  while(std::getline(std::cin, line))
  {
    auto const colon_pos{ line.find(':') };
    auto const pipe_pos{ line.find('|') };
    auto const s1{ line.substr(colon_pos+2, pipe_pos-(colon_pos+2)) };
    auto const s2{ line.substr(pipe_pos+1) };

    std::set<int> winning_numbers, my_numbers;
    {
      std::stringstream ss1(s1), ss2(s2);
      while(!ss1.eof()) { int n; ss1 >> n; winning_numbers.insert(n); }
      while(!ss2.eof()) { int n; ss2 >> n; my_numbers.insert(n); }
    }

    matching_numbers_per_card.push_back(0);
    for(auto number : my_numbers)
      if(winning_numbers.contains(number)) matching_numbers_per_card.back()++;

    ans.first += matching_numbers_per_card.back() ? (1 << (matching_numbers_per_card.back()-1)) : 0;
  }

  int const num_card_types{static_cast<int>(matching_numbers_per_card.size())};
  std::vector<int> num_cards(num_card_types, 1);
  for(int i{0}; i < num_card_types; i++)
    for(int j{0}; j < matching_numbers_per_card[i]; j++)
      num_cards[i+1+j] += num_cards[i];

  ans.second = std::ranges::fold_left(num_cards, 0, std::plus<int>());

  std::cout << std::format("Part one: {}.\n", ans.first);
  std::cout << std::format("Part two: {}.\n", ans.second);
}

