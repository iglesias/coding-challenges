#include <algorithm>
#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <utility>
#include <vector>

auto const start = std::chrono::high_resolution_clock::now();

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
  for(int i = 0; i < std::ssize(left_numbers); i++) {
      ans.first += std::abs(left_numbers[i] - right_numbers[i]);
      ans.second += left_numbers[i] * right_counter[left_numbers[i]];
  }
  auto const end = std::chrono::high_resolution_clock::now();
  std::printf("Part one: %d\nPart two: %d\n", ans.first, ans.second);
  std::printf("%lf milliseconds\n", std::chrono::duration<double, std::milli>(end - start).count());
}
