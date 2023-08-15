
uint32_t g_count{0};

void through_subsets(vector<int> const& nums, int k) {
    int const n = nums.size();
    if (nums.empty() || n < k) {
        return;
    }
    vector<int> indices(k);
    std::iota(indices.begin(), indices.end(), 0);
    int i, j;
    array<uint16_t, 3> subset;
    while (true) {
        for (i = 0; i < k; i++) {
            subset[i] = nums[indices[i]];
        }
        g_count++;
        for (i = k - 1; i >= 0; i--) {
            if (indices[i] != i + n - k) {
                break;
            }
        }
        if (i < 0) {
            break;
        }
        indices[i]++;
        for (j = i + 1; j < k; j++) {
            indices[j] = indices[j - 1] + 1;
        }
    }
}

//FIXME ints (indices, i, j, etc.?) -> int16_ts
void through_subsets_implicit(int n) {
    constexpr int k{3};
    if (n < k) {
        return;
    }
    array<int, k> indices;
    std::iota(indices.begin(), indices.end(), 0);
    int i, j;
    array<uint16_t, 3> subset;
    while (true) {
        for (i = 0; i < k; i++) {
            subset[i] = indices[i];
        }
        g_count++;
        for (i = k - 1; i >= 0; i--) {
            if (indices[i] != i + n - k) {
                break;
            }
        }
        if (i < 0) {
            break;
        }
        indices[i]++;
        for (j = i + 1; j < k; j++) {
            indices[j] = indices[j - 1] + 1;
        }
    }
}

int main(int argc, char *argv[]) {
    int N{10};
    if(argc>1)
      N = std::stoi(argv[1]);

    /*
    vector<int> nums(N);
    std::iota(nums.begin(), nums.end(), 0);

    auto const& subsets = gen_subsets(nums, 3);
    for(auto&& subset : subsets)
    {
      for(int item : subset) std::cout << item << ' ';
      std::cout << '\n';
    }
    */

    //through_subsets(nums, 3);
    through_subsets_implicit(N);
    std::cout << g_count << '\n';

}
