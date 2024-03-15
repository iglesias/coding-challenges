// Comparing a million elements bitset with unordered_map.

#include <bitset>
#include <chrono>
#include <deque>
#include <exception>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

struct InputError : std::exception {};

// The algorithm counts how many sushi pieces (items in D) can be picked
// in total such that they are different to the last K elements picked.

int f(int N, std::vector<int> D, int K) {
  if (N <= 0 or N > 500000 or K <= 0 or K > N) throw InputError{};

  int ans = 0;
  std::unordered_set<int> set;
  std::vector<int> q(K);

  int qhead = 0;
  for (int Di : D) {
    if (Di <= 0 or 1000000 < Di) throw InputError();

    if (!set.contains(Di)) {
      if (static_cast<int>(set.size()) == K) {
        int const least_recent = q[qhead];
        set.erase(least_recent);
      }
      set.insert(Di);
      q[qhead] = Di;
      qhead = (qhead + 1) % K;
      ans++;
    }
  }
  return ans;
}

int g(std::vector<int> D, int K) {
  int ans = 0;
  std::bitset<1000000+1> set;
  std::vector<int> q(K);

  int qhead = 0;
  for (int Di : D) {
    if (!set.test(Di)) {
      if (static_cast<int>(set.count()) == K) {
        int const least_recent = q[qhead];
        set.reset(least_recent);
      }
      set.set(Di);
      q[qhead] = Di;
      qhead = (qhead + 1) % K;
      ans++;
    }
  }
  return ans;
}

int h(std::vector<int> D, int K) {
  int ans = 0;
  std::unordered_set<int> set;
  std::deque<int> q;

  for (int Di : D) {
    if (!set.contains(Di)) {
      if (static_cast<int>(set.size()) == K) {
        int const least_recent = q.front();
        q.pop_front();
        set.erase(least_recent);
      }
      set.insert(Di);
      q.push_back(Di);
      ans++;
    }
  }
  return ans;
}

void time(auto f, auto... args) {
  using namespace std::chrono;
  auto const start = system_clock::now();
  std::cout << f(args...) << '\n';
  std::cout << duration<double>(system_clock::now()-start).count() << '\n';
}

int main() {
  int N{123456}, K{2583};
  std::vector<int> D(N, 1);
  std::iota(D.begin(), D.end(), 1);
  time(f, N, D, K);
  time(h, D, K);
  time(g, D, K);
  K = N;
  time(f, N, D, K);
  time(h, D, K);
  N = 500000;
  D = std::vector<int>(N, 1);
  std::iota(D.begin(), D.end(), 1);
  time(f, N, D, K);
  time(h, D, K);
  K = N;
  time(f, N, D, K);
  time(h, D, K);
}
