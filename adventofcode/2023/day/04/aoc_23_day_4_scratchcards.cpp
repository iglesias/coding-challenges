#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
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

    // Materializing the intersection (it uses more space than needed).
    std::vector<int> intersection;
    std::ranges::set_intersection(winning_numbers, my_numbers, std::back_inserter(intersection));
    matching_numbers_per_card.push_back(static_cast<int>(intersection.size()));

    ans.first += matching_numbers_per_card.back() ? (1 << (matching_numbers_per_card.back()-1)) : 0;
  }

  int const num_card_types{static_cast<int>(matching_numbers_per_card.size())};
  std::vector<int> num_cards(num_card_types, 1);
  for(int i{0}; i < num_card_types; i++)
    for(int j{0}; j < matching_numbers_per_card[i]; j++)
      num_cards[i+1+j] += num_cards[i];

  ans.second = std::ranges::fold_left(num_cards, 0, std::plus<int>());

  std::cout << "Part one: " << ans.first
            << ".\nPart two: " << ans.second << ".\n";
}

