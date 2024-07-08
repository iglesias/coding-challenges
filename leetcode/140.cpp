#include <string>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

using std::string;
using std::unordered_set;
using std::vector;

void solve(vector<string>& ans, const string& s, const unordered_set<string>& words, string& sentence, int idx = 0) {
    const int n = static_cast<int>(s.size());
    if (idx >= n) {
        ans.push_back(sentence);
    } else {
        for (int i = idx; i < n; i++) {
            const string t = s.substr(idx, i - idx + 1);
            if (words.contains(t)) {
                string new_sentence = sentence;
                if (sentence.empty()) new_sentence += t;
                else {
                    new_sentence.push_back(' ');
                    new_sentence += t;
                }
                solve(ans, s, words, new_sentence, i + 1);
            }
        }
    }
}

vector<string> wordbreak(const string& s, const vector<string>& wordDict) {
    unordered_set<string> words;
    for (const string& word : wordDict) words.insert(word);
    vector<string> ans;
    {
        string sentence;
        solve(ans, s, words, sentence);
    }
    return ans;
}

TEST(BreakIntoWords, Example_1)
{
    EXPECT_EQ(wordbreak("catsanddog", vector<string>{"cat", "cats", "and", "sand", "dog"}),
              (vector<string>{"cat sand dog", "cats and dog"}));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
