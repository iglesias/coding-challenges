#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using std::string;
using std::vector;

vector<string> sort_strings_by_ints(vector<string> strings, vector<int> ints) {
    vector<int> indices(ints.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::ranges::stable_sort(indices,
                             [&ints](int a, int b) {
                                return ints[a] > ints[b]; });
    vector<string> result(strings.size());
    for (int i = 0; i < static_cast<int>(strings.size()); i++)
        result[i] = strings[indices[i]];
    return result; }

TEST(sort_strings_by_ints, a) {
    EXPECT_EQ((vector<string>{"foo", "bar", "baz"}),
              sort_strings_by_ints({"bar", "foo", "baz"}, {1, 3, 1})); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); }
