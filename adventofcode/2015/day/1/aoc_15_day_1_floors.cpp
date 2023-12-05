// g++ -std=c++14 aoc_15_day_1_floors.cpp

#include <bits/stdc++.h>

auto paren2sign(char c) -> int
{
  if(c == '(')            return +1;
  else{ assert(c == ')'); return -1; }
}

int main()
{
  std::string input;
  std::getline(std::cin, input);

  bool basement_found{false};
  int floor{0};
  for(int i{0}; i<input.length(); i++)
  {
    floor += paren2sign(input[i]);
    if(floor==-1 and not basement_found) { basement_found = true; std::cout << "Part two: " << i+1 << '\n'; }
  }
  std::cout << floor << '\n';
}

  // no support of partial sum for ranges in the standard, it appears to ve in ranges-v3
  /*
  auto parenthesisToSign = [](const char c) -> int { if(c == '(') return +1; else { assert(c == ')'); return -1; } };
  auto x = input | std::views::transform(parenthesisToSign);
  for(auto v : x) std::cout << v << ' ';
  std::cout << '\n';
  auto y = x | ranges::views::partial_sum(); // std::views:partial_sum();
  */



  //std::vector<int> signs;
  /*
  std::transform(input.cbegin(), input.cend(), std::back_inserter(signs),
		 [](char c){
		   if(c == '(')            return +1;
		   else{ assert(c == ')'); return -1; }});
		   */
  //std::ranges::transform(input, signs, paren2sign);

  /*
  std::vector<int> partial_signs_sums;
  std::partial_sum(signs.cbegin(), signs.cend(), std::back_inserter(partial_signs_sums));

  std::cout << "Part one: " << *partial_signs_sums.crbegin() << '\n';

  auto const i = std::find(partial_signs_sums.cbegin(), partial_signs_sums.cend(), -1);
  if(i != partial_signs_sums.cend())
  std::cout << "Part two: " << std::distance(partial_signs_sums.cbegin(), i)+1 << '\n';
  */
