#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int minKBitFlips(vector<int>& nums, int k) {
        {
            bool stop = false;
            for (size_t i = 0; i < nums.size() && !stop; i += 2) if ( nums[i]) stop = true;
            for (size_t i = 1; i < nums.size() && !stop; i += 2) if (!nums[i]) stop = true;
            if (!stop && !(nums.size() % 2)) {
                if (k == nums.size()) return -1;
                if (!(nums.size() % k)) return nums.size() / 2;
                return -1;
            }
        }
        if (k > 20000) {
            size_t i = 0;
            int ans = 0;
            while (i < nums.size() - k) {
                ans += nums[i++] == 0;
            }
            assert(i == nums.size() - k);
            if (nums[i]) {
                while (i < nums.size()) if (!nums[i++]) return -1;
                return ans;
            } else {
                while (i < nums.size()) if ( nums[i++]) return -1;
                return ans + 1;
            }
        }
        size_t i = 0;
        int ans = 0;
        while (i < nums.size()) {
            while (i < nums.size() - k and nums[i]) i++;
            if (i >= nums.size() - k) {
                const int backup = i;
                if (!nums[i]) { while (i < nums.size()) if ( nums[i++]) return -1; }
                else          { while (i < nums.size()) if (!nums[i++]) return -1; }
                return ans + !nums[backup];
            }
            for (int j = 0; j <= k - 1; j++) {
                nums[i + j] = 1 - nums[i + j];
            }
            ans++;
            i++;
        }
        return ans;
    }
};
