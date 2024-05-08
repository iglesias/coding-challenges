#include <algorithm>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

vector<string> find_relative_ranks(vector<int>&& scores) {
    vector<int> copy(scores);
    ranges::sort(copy);
    vector<string> placements;
    for (const int score : scores) {
        placements.emplace_back([&](){
            const unsigned position =
                    scores.size() - distance(copy.begin(), ranges::lower_bound(copy, score));
            using namespace literals;
            switch (position) {
                case 1:  return "Gold Medal"s;
                case 2:  return "Silver Medal"s;
                case 3:  return "Bronze Medal"s;
                default: return to_string(position);
            }
        }());
    }
    return placements;
}

TEST(RelativeRanks, Case_1) { EXPECT_EQ(find_relative_ranks({ 5, 4, 3, 2, 1}), (vector<string>{"Gold Medal", "Silver Medal", "Bronze Medal",            "4", "5"})); }
TEST(RelativeRanks, Case_2) { EXPECT_EQ(find_relative_ranks({10, 3, 8, 9, 4}), (vector<string>{"Gold Medal",            "5", "Bronze Medal", "Silver Medal", "4"})); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
