#include <bits/stdc++.h>

int main()
{
  int T;
  std::cin >> T;
  for(int t{0}; t<T; t++)
  {
    long long N;
    std::cin >> N;
    std::cout << "N=" << N << '\n';

    // Find the right i, running sum.
    /* Loop for the small data set.
    int cumsum{0}, i{0};
    while(N>cumsum+(i+1)*26)
    {
      cumsum += (i+1)*26;
      i++;
    }
    std::cout << "cumsum=" << cumsum << std::endl;

    std::cout << ">>> " << std::ceil(1.*(N-cumsum)/(i+1)) << std::endl;
    std::cout << ">>> " << static_cast<int>(std::ceil(1.*(N-cumsum)/((i+1)))) << std::endl;
    std::cout << "Case #" << (t+1) << ": " << char(static_cast<int>(std::ceil(1.*(N-cumsum)/(i+1))) + 'A' - 1) << '\n';
    */

    double x = (-1+std::sqrt(1+4.*(N/13.)))/2;
    auto i = std::floor(x);
    std::cout << ">> " << std::setprecision(20) << x << ' ' << i << ' ' << i*(i+1)*13 << ' ' << (i+1)*(i+2)*13 <<  std::endl;
    if(i*(i+1)*13 == N)
      std::cout << "Case #" << (t+1) << ": " << 'Z' << '\n';
    else
      std::cout << "Case #" << (t+1) << ": " << char(static_cast<int>(std::ceil(1.*(N-i*(i+1)*13)/(i+1))) + 'A' - 1) << '\n';
  }
}
