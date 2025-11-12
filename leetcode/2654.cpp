class Solution {
    int m_rec_counter{0};
public:
    int minOperations(vector<int>& nums, int rec_counter = 0) {

        // To store the deepest recursion reached, which is enough
        // with overwriting since the deepest will be the last call.
        m_rec_counter = rec_counter;

        /*
        int const n = static_cast<int>(nums.size());
        cout << "[";
        for (int i = 0; i < n; i++) cout << nums[i] << (i == n - 1 ? '\0' : ',');
        cout << "] " << rec_counter << "\n";
        */

        bool const odd_found = ranges::any_of(nums, [](int x) { return x % 2 != 0; });
        if (!odd_found) return -1;

        // Find the minimum that is not 1
        auto it = ranges::min_element(nums, [](int a, int b) {
            if (a == 1) return false;
            if (b == 1) return true;
            return a < b;
        });
        if (it == nums.end() || *it == 1) return 0; // all ones
        int const min_num = *it;

        // Check if there's any element not multiple of min_num
        bool const non_multiple_found = ranges::any_of(nums, [=](int x) {
            return x != min_num && x % min_num != 0;
        });
        if (!non_multiple_found) return -1;

        // Compute gcds. If any of them is 1, then all elements can be
        // made 1 and the answer is the number of elements that aren't
        // already 1. Otherwise, try with recursion in the array of gcds.
        vector<int> gcds(nums.size() - 1);
        transform(
            nums.begin(), nums.end() - 1,  // original
            nums.begin() + 1,              // shifted
            gcds.begin(),
            [](int a, int b) { return std::gcd(a, b); }
        );
        bool const one_gcd_found = ranges::any_of(gcds, [](int g) { return g == 1; });
        if (one_gcd_found) return ranges::count_if(nums, [](int x) { return x != 1; });
        // Recursive case
        if (minOperations(gcds, rec_counter + 1) == -1) return -1;
        return nums.size() + m_rec_counter;
    }
};
