#include <cstring>
#include <iostream>
#include <vector>

using ull = uint64_t;

ull fib_cache(short n) {
  //std::cout << "fib_cache(" << n << ")\n";
  static std::vector<ull> cache = std::vector<ull>(51, 0);
  if (n == 0) return 0;
  if (n == 1) return 1;
  if (cache[n]) return cache[n];
  auto ret = fib_cache(n-1) + fib_cache(n-2);
  cache[n] = ret;
  return ret;
}

ull fib_tail(short n, ull a=0, ull b=1) {
  //std::cout << "fib_tail(" << n << ")\n";
  if (n == 0) return a;
  return fib_tail(n-1, b, a+b);
}

ull fib_slow(short n) {
  //std::cout << "fib_slow(" << n << ")\n";
  if (n == 0) return 0;
  if (n == 1) return 1;
  return fib_slow(n-1) + fib_slow(n-2);
}

int main(int argc, char* argv[]) {
  int n = atoi(argv[1]);
  if (strcmp(argv[2], "tail") == 0)
    std::cout << fib_tail(n) << std::endl;
  else if (strcmp(argv[2], "cache") == 0)
    std::cout << fib_cache(n) << std::endl;
  else if (strcmp(argv[2], "slow") == 0)
    std::cout << fib_slow(n) << std::endl;
  else
    std::cerr << "Invalid argument " << argv[2] << ". Valid values are cache, tail, slow.\n";
}
