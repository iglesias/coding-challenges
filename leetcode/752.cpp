#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

using namespace std;

using namespace string_literals;

vector<string> generate(string_view in) {
  vector<string> out;
  for (int i = 0; i <= 3; i++) {
    string s(in);
    if (s[i] == '0')
      s[i] = '9';
    else
      s[i] = char(s[i] - 0 - 1);
    out.push_back(s);
    s[i] = in[i];
    if (s[i] == '9')
      s[i] = '0';
    else
      s[i] = char(s[i] - 0 + 1);
    out.push_back(std::move(s));
  }
  return out;
}

int openLock(vector<string> &deadends, string target) {
  if (target == "0000"s)
    return 0;
  unordered_set<string_view> deadendsLookup;
  for (string const &deadend : deadends)
    deadendsLookup.insert(deadend);
  if (deadendsLookup.contains("0000"s))
    return -1;
  unordered_map<string, std::size_t> Qed;
  Qed.emplace("0000"s, 0);
  queue<string> Q;
  Q.push("0000"s);
  int ans{-1};
  while (!Q.empty()) {
    string s = Q.front();
    Q.pop();
    if (s == target) {
      ans = static_cast<int>(Qed[s]);
      break;
    }
    for (auto const &t : generate(s)) {
      if (deadendsLookup.contains(t) or Qed.contains(t))
        continue;
      Q.push(t);
      Qed.emplace(t, Qed[s] + 1);
    }
  }
  return ans;
}

TEST(openLock, a) {
  vector<string> deadends{"0201", "0101", "0102", "1212", "2002"};
  string target{"0202"};
  EXPECT_EQ(openLock(deadends, target), 6);
}

TEST(openLock, b) {
  vector<string> deadends{"8888"};
  string target{"0009"};
  EXPECT_EQ(openLock(deadends, target), 1);
}

TEST(openLock, c) {
  vector<string> deadends{"8887", "8889", "8878", "8898",
                          "8788", "8988", "7888", "9888"};
  string target{"8888"};
  EXPECT_EQ(openLock(deadends, target), -1);
}

TEST(openLock, cannotStart) {
  vector<string> deadends{"0000"};
  string target{"8888"};
  EXPECT_EQ(openLock(deadends, target), -1);
}

TEST(openLock, noNeedToStart) {
  vector<string> deadends{"0000"};
  string target{"0000"};
  EXPECT_EQ(openLock(deadends, target), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
