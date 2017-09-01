#include <cstring>
#include <iostream>

using ull = uint64_t;

ull factorial_slow(int n) {
  std::cout << "factorial_slow(" << n << ")\n";
  if (n <= 1) return 1;
  else return n * factorial_slow(n-1);
}

ull factorial_fast(int n, ull acc=1) {
  std::cout << "factorial_fast(" << n << ")\n";
  if (n <= 1) return acc;
  else return factorial_fast(n-1, acc*n);
}

int main(int argc, char* argv[]) {
  int n = atoi(argv[1]);
  if (strcmp(argv[2], "slow") == 0)
    std:: cout << factorial_slow(n) << std::endl;
  else if (strcmp(argv[2], "fast") == 0)
    std::cout << factorial_fast(n) << std::endl; 
  else
    std::cout << "Invalid argument " << argv[2] << ". Allowed values are slow, fast.\n";
}
