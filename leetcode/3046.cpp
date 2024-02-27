#include <algorithm>
#include <cassert>
#include <functional>
#include <random>
#include <ranges>
#include <vector>

bool f0(std::vector<int> const& nums){
  std::array<uint_fast8_t, 100> counter;
  for(auto& count : counter) count = 0;
  for(int num : nums) if (2 < ++counter[num-1]) return false;
  return true;}

bool f1(std::vector<int>& nums){
  using std::views::chunk_by;
  using std::views::transform;
  using std::ranges::not_equal_to;
  using std::ranges::distance;
  std::ranges::sort(nums);
  auto view = nums
            | chunk_by(std::not_fn(not_equal_to{}))
            | transform([](auto&& chunk){ return distance(chunk); });
  return *std::ranges::max_element(view) <= 2;}

std::vector<int> get_random_input(){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> randi(1, 10);
  int const n = randi(gen);
  std::vector<int> nums(n);
  for(int& num : nums) num = randi(gen);
  return nums;}

int main(){
  for(int i = 0; i < 100; i++){
    auto nums = get_random_input();
    assert(f0(nums) == f1(nums));}}
