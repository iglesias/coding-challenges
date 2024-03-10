#include <chrono>
#include <iostream>
#include <random>
#include <map>
#include <vector>

int solven21(std::vector<int> const&);
int solvenlnn(std::vector<int> const&);
int solven1(std::vector<int> const&);
std::vector<int> get_input(int argc, char* argv[]);

int main(int argc, char* argv[])
{
  auto const input{get_input(argc, argv)};
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  auto ans = solven21(input);
  end = std::chrono::system_clock::now();
  double time = std::chrono::duration<double>(end-start).count();
  std::cout <<  ans << ' ' << time << '\n';
  start = std::chrono::system_clock::now();
  ans = solvenlnn(input);
  end = std::chrono::system_clock::now();
  time = std::chrono::duration<double>(end-start).count();
  std::cout << ans << ' ' << time << '\n';
  start = std::chrono::system_clock::now();
  ans = solven1(input);
  end = std::chrono::system_clock::now();
  time = std::chrono::duration<double>(end-start).count();
  std::cout << ans << ' ' << time << '\n';
}

std::vector<int> get_input(int argc, char* argv[])
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> values_distribution(1, 1000000000);

  int const N = [argc, argv]() {
    if (argc > 1) {
      try {
        return std::max(1, std::abs(std::stoi(argv[1])));
      } catch(...) {
        return 10000;
      }
    } else {
      return 10000;
    }}();

  std::vector<int> values(N);
  for(int& value : values) value = values_distribution(gen);
  return values;
}

int solven21(std::vector<int> const& v)
{
  int ans = 0;
  int const N = static_cast<int>(v.size());
  for(int i = 0; i < N; i++) for(int j = i+1; j < N; j++)
    ans = std::max(ans, v[j]-v[i]);
  return ans;
}

int solvenlnn(std::vector<int> const& v)
{
  std::map<int, int> baile;
  for(int const i : v) {
    if(baile.contains(i)) baile[i]++;
    else                  baile.emplace(i, 1);
  }
  int ans = 0;
  for(int const i : v) {
    auto const jt = baile.rbegin();
    ans = std::max(ans, jt->first-i);
    if(baile[i] == 1) {
      baile.erase(i);
    } else {
      baile[i]--;
    }
  }
  return ans;
}

int solven1(std::vector<int> const& v)
{
  if(v.size() == 0) return 0;
  int minsofar = v[0];
  int ans = 0;
  for(int const i : v) {
    ans = std::max(ans, i-minsofar);
    minsofar = std::min(minsofar, i);
  }
  return ans;
}
