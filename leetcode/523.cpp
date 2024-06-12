#include <vector>

#include <gtest/gtest.h>

// Check if the arary has a (continuous) subarray of at least
// two elements whose sum is a multiple of k.
bool check_subarray_sum(const std::vector<int>& nums, int k)
{
    const int n = static_cast<int>(nums.size());
    if (n < 2) return false;
    int i = 0, j = 1;
    unsigned sum = 0;
    while (i < n) {
        sum += nums[i];
        while (j < n) {
            sum += nums[j];
            if (!(sum % k)) return true;
            j ++;
        }
        j--;
        sum -= nums[i];
        i++;
        if (i == n) break;
        while (j >= i + 1) {
            if (!(sum % k)) return true;
            sum -= nums[j];
            j--;
        }
        sum -= nums[i];
        i++, j +=2;
    }
    return false;
}

TEST(CheckSubarraySumDivisibility, a)
{
    const std::vector nums{23, 2, 6, 4, 7};
    EXPECT_EQ(check_subarray_sum(nums,  6), true);
    EXPECT_EQ(check_subarray_sum(nums, 13), false);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
