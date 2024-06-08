/*
    Given an integer array and an integer k, return true if the arary has a
    (continuous) subarray with length at least two whose elements of the subarray
    is a multiple of k.
  */

  bool checkSubarraySum(vector<int>& nums, int k) {
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
