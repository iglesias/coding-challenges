// clang++ -std=c++23 -O3 -ftime-trace historian_hysteria.cpp
// open output json in Chromium's chrome://tracing

#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
  std::pair<int, int> ans;
  int left_number, right_number;
  std::vector<int> left_numbers, right_numbers;
  std::unordered_map<int, int> right_counter;
  while(std::scanf("%d   %d\n", &left_number, &right_number) != EOF) {
      left_numbers.push_back(left_number);
      right_numbers.push_back(right_number);
      right_counter[right_number] += 1;
  }
  std::ranges::sort(left_numbers);
  std::ranges::sort(right_numbers);
  for (int i = 0; i < std::ssize(left_numbers); i++) {
      ans.first += std::abs(left_numbers[i] - right_numbers[i]);
      ans.second += left_numbers[i] * (right_counter[left_numbers[i]]);
  }
  std::printf("Part one: %d\nPart two: %d\n", ans.first, ans.second);
}
