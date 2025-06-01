#include <cmath>
#include <unordered_map>
#include <vector>

using std::max;

int
solve(std::vector<int> const& nums, int& curMax, std::unordered_map<int ,int>& freqs,
      int const curOr = 0, std::vector<int>::size_type const i = 0)
{
        freqs[curOr]++;
        curMax = max(curMax, curOr);
        if (i == nums.size()) return curMax;
        for (std::vector<int>::size_type j = i; j < nums.size(); j++)
            curMax = max(curMax, solve(nums, curMax, freqs, curOr | nums[j], j + 1));
        return curMax;
}

int
countMaxOrSubsets(std::vector<int>& nums)
{
    std::unordered_map<int, int> freqs;
    int curMax = 0;
    return freqs[solve(nums, curMax, freqs)];
}

int main() {}
