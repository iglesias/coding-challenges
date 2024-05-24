#include <algorithm>
#include <array>
#include <string>
#include <vector>

using std::string;
using std::vector;

std::array<int, 'z' - 'a' + 1> letters_left;  // global

namespace loop {

int solve(const vector<string>& words, const vector<int>& scores, vector<string>& subset, const size_t words_i = 0, int ans = 0)
{
    {
        int score = 0;
        for (const string& word : subset) for (const char letter : word) score += scores[letter - 'a'];
        ans = std::max(ans, score);
    }

    for (size_t j = words_i; j < words.size(); j++) {
        bool jump = false;
        for (size_t i = 0; i < words[j].size() and !jump; i++)
            if (letters_left[words[j][i] - 'a']) letters_left[words[j][i] - 'a']--;
            else {
                jump = true;
                for (size_t k = 0; k < i; k++) letters_left[words[j][k] - 'a']++;
            }
        if (jump) continue;
        subset.push_back(words[j]);
        ans = std::max(ans, solve(words, scores, subset, j + 1));
        subset.pop_back();
        for (size_t i = 0; i < words[j].size(); i++) letters_left[words[j][i] - 'a']++;
    }

    return ans;
}

}  // namespace loop

namespace without_or_with {

int solve(const vector<string>& words, const vector<int>& scores, vector<string>& subset, const size_t words_i = 0, int ans = 0)
{
    {
        int score = 0;
        for (const string& word : subset) for (const char letter : word) score += scores[letter - 'a'];
        ans = std::max(ans, scores);
    }

    if (words_i == words.size()) return ans;

    ans = std::max(ans, solve(words, scores, subset, words_i + 1, ans));

    for (size_t i = 0; i < words[words_i].size(); i++)
        if (letters_left[words[words_i][i] - 'a']) letters_left[words[words_i][i] - 'a']--;
        else {
            for (size_t k = 0; k < i; k++) letters_left[words[words_i][k] - 'a']++;
            return ans;
        }
    subset.push_back(words[words_i]);
    ans = std::max(ans, solve(words, scores, subset, words_i + 1, ans));
    subset.pop_back();
    for (size_t i = 0; i < words[words_i].size(); i++) letters_left[words[words_i][i] - 'a']++;

    return ans;
}

}  // namespace without_or_with

int max_score(const vector<string>& words, const vector<char>& letters, const vector<int>& scores, bool loop = true)
{
    std::ranges::fill(letters_left, 0);
    for (const char letter : letters) letters_left[letter - 'a']++;
    vector<string> subset;
    if (loop) return loop::solve(words, scores, subset);
    else      return without_or_with::solve(words, scores, subset);
}

TEST(MaximizeWordsScore, SixteenOfFiftytwo)
{
    const vector<string> words{"baa", "bba", "ccb", "ac"};
    const vector<char> letters{'a', 'b', 'b', 'b', 'b', 'c'};
    const vector<int> scores{2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_EQ(max_score(words, letters, scores), 6);
    EXPECT_EQ(max_score(words, letters, scores, /* loop = */ false), 6);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
