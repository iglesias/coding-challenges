// g++ -std=c++20 -fmodules-ts -x c++-system-header array
// g++ -std=c++20 -fmodules-ts -x c++-system-header algorithm
// g++ -std=c++20 -fmodules-ts %

import <algorithm>;
import <array>;

template<typename Range> constexpr int solve(const Range& nums){
  const auto min             = *std::ranges::min_element(nums);
  const auto min_count       =  std::ranges::count(nums, min);
  for(const auto num : nums) if((num % min) != 0) return 1;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshift-op-parentheses"
  return (static_cast<int>(min_count) + 1) >> 1;
#pragma clang diagnostic pop
}

int main(){
  constexpr std::array nums{17, 34, 17, 17, 51, 17};
  static_assert(solve(nums) == 2);
}
