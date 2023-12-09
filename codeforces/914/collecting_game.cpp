// {{{ Boilerplate Code <--------------------------------------------------------------------------
//
//
// Basic ------------------------------------------------------------------------------------------

#include <bits/stdc++.h>

#define   ALL(a)          (a).begin(), (a).end()
#define   FOR(i, a, b)    for ( typeof(a) i = (a) ; i < (b) ; ++i )
#define   REP(i, n)       FOR(i, 0, n)

using ii = std::pair<int, int>;
using ll = long long;

// I/O --------------------------------------------------------------------------------------------

#include <cxxabi.h>

// Specialization to PRINT std::vector<T>.
// This type of function wouldn't be needed using fmt.
template<typename T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  /*
  // Print the vector's template type name demangled.
  // It is somewhat polluting in common cases.
  //
  // FIXME actually, what'd be more useful for printing
  // with the vector instead of the type are the indices
  // to the items.
  int status;
  char* demangled_name =
    abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
  assert(!status);
  os << "vector<" << demangled_name << ">(" << (v.size() ? '\0' : ')'); // taking care of closing () if v=[]
  std::free(demangled_name);
  */
  os << "vector(" << (v.size() ? '\0' : ')');

  // When the vector is of strings or so, they can contain commas and
  // the separation between elements of the vector won't be clear.
  // Solve it by printing strings between quotes (or an opening and a
  // closing character in general).
  char opening{}, closing{};
  if constexpr(std::is_same<T, std::string>() or std::is_same<T, char const*>()) opening = closing = '\'';
  for(size_t i{0}; i < v.size(); i++)
    os << opening << v[i] << closing << ((i == v.size()-1)? ")" : ", ");

  return os;
}

#define   PRINT(...)      print(#__VA_ARGS__, __VA_ARGS__)
// TODO   separate print w/ & w/o template type names (PRINTT)

template<typename... Args> void print(std::string vars, Args&&... values)
{
  std::cout << "[" << vars << " = ";
  std::string delim = "";
  (..., (std::cout << delim << values, delim = ", "));
  std::cout << "]\n";
}

// String Utils --------------------------------------------------------------------------------------------

//FIXME improve template with concept or traits.
template<typename string> auto split(std::string const& str, string sep) -> std::vector<std::string>
{
  std::string::size_type pos{0}, n;
  std::vector<std::string> out;

  while((n = str.find(sep, pos)) != std::string::npos)
  {
    out.push_back(str.substr(pos, n-pos));
    pos = n+1;
  }

  if(pos != 0) out.push_back(str.substr(pos));

  return out;
}

// ------------------------------------------------------------------------------> End of Boilerplate }}}

std::vector<int> a;

std::vector<int> solve()
{
  std::vector<int> ans;
  return ans;
}

int main()
{
  std::cin.tie(0)->sync_with_stdio(0);
  std::string line;
  std::getline(std::cin, line); // # test cases
  while(std::getline(std::cin, line)) // # array elements per test case
  {
    PRINT(line); std::cout << std::endl;
    int n = std::stoi(line);
    a.clear();
    a = std::vector<int>(n);
    for(int i{0}; i<n; i++) std::cin >> a[i];
    std::getline(std::cin, line);
    PRINT(n, a);
    PRINT(solve());
  }
}

//TODO clean template and PRINT before committing
//
//https://codeforces.com/problemset/submission/1904/236576327
//my submission, one can see the test cases and analize why the single pass was not fast enough while looping upper bound yeah
//
//solution from someone that cleared all hacker attempts,
//it seems to be using dynamic programming,
//  it's also sorting and partial sum or prefix sum array, but then 
//  it clearly uses some basic with temp, the looping reverse
//  direction and indexing into ans, of course as the DP table
//nice template stuff with the fori, rfori, and maybe prarr too
