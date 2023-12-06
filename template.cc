// {{{ Boilerplate <--------------------------------------------------

#include <bits/stdc++.h>

#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)
#define   ALL(a)          (a).begin(), (a).end()

#define dbp(...) dblog(#__VA_ARGS__, __VA_ARGS__)

template<typename... Args>
void dblog(std::string vars, Args&&... values)
{
    std::cout << "[" << vars << " = ";
    std::string delim = "";
    (..., (std::cout << delim << values, delim = ", "));
    std::cout << "]\n";
}

using ii = std::pair<int, int>;
using ll = long long;

// }}}            <--------------------------------------------------

int main()
{
  std::cout << "Hello template!\n";
}
