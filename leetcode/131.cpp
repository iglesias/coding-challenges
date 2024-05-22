#include <string>
#include <vector>

#include <gtest/gtest.h>

using std::string;
using std::vector;

bool is_palindrome(const string& s, int start, int end) {
    while (start <= end) if (s[start++] != s[end--]) return false;
    return true;
}

void solve(const string& s, vector<vector<string>>& ans, vector<int>& v, int v_index = 0) {
    if (!v.size()) {
        if (is_palindrome(s, 0, s.length() - 1)) ans.push_back(vector<string>{s});
    } else {
        bool not_palindrome_found = false;
        int start = 0;
        vector<string> candidate;
        for (size_t i = 0; i < v.size() && !not_palindrome_found; i++) {
            not_palindrome_found |= !is_palindrome(s, start, v[i]);
            candidate.push_back(s.substr(start, v[i] - start + 1));
            start = v[i] + 1;
        }
        not_palindrome_found |= !is_palindrome(s, start, s.length() - 1);
        candidate.push_back(s.substr(start));
        if (!not_palindrome_found) ans.push_back(candidate);
    }
    for (size_t j = v_index; j < s.length() - 1; j++) {
        v.push_back(j);
        solve(s, ans, v, j + 1);
        v.pop_back();
    }
}

vector<vector<string>> partition(const string& s) {
    vector<vector<string>> ans;
    vector<int> v;
    solve(s, ans, v);
    return ans;
}

TEST(PalindromePartitioning, a) {
  const vector<string> ans{{"a", "a", "b"}, {"aa", "b"}};
  EXPECT_EQ(partition("aab", ans));
}

TEST(PalindromePartitioning, b) {
  EXPECT_EQ(partition("a", vector<vector<string>>{{"a"}}));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
