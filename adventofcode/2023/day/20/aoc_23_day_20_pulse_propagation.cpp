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

// Specialization for std::pair a la std::vector
// FIXME done quickly while doing day 10 (do T1, T2, probably etc.)
template<typename T> std::ostream& operator<<(std::ostream& os, std::pair<T,T> const& p)
{
  os << "pair(";

  // When the pair is of strings or so, they can contain commas and
  // the separation between elements of the vector won't be clear.
  // Solve it by printing strings between quotes (or an opening and a
  // closing character in general).
  char opening{}, closing{};
  if constexpr(std::is_same<T, std::string>() or std::is_same<T, char const*>()) opening = closing = '\'';
    os << opening << p.first << closing << ", " << opening << p.second << closing << ")";

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

auto split(std::string const& str, std::string const& sep) -> std::vector<std::string>
{
  std::string::size_type pos{0}, n;
  std::vector<std::string> out;

  while((n = str.find(sep, pos)) != std::string::npos)
  {
    out.push_back(str.substr(pos, n-pos));
    pos = n + std::size(sep);
  }

  if(pos != 0) out.push_back(str.substr(pos));

  return out;
}

// ------------------------------------------------------------------------------> End of Boilerplate }}}

//prefix %
struct flip_flop
{
  bool on;
  flip_flop() : on(false) {}
  //high pulse -> nothing
  //low pulse  -> flips on/off
  //  if off: turn on and send high pulse
  //  if on:  turn off and send low pulse
};

enum class Pulse
{
  LOW, HIGH
};

//prefix &
struct conjunction
{
  std::vector<Pulse> input_pulses; // input module -> pulse
  conjunction(int num_inputs) : input_pulses(num_inputs, Pulse::LOW) {}
  //
};

//broadcaster: list of destination modules

//utton module: it sends a low pulse to the broadcaster module


void read_input();
ii ans;
void solve();

int main()
{
  read_input();
  solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    auto const vs0 = split(line, std::string(" -> "));
    assert(vs0.size() == 2);
    auto const vs1 = vs0.at(1).find(',') == std::string::npos ?
      std::vector{vs0.at(1)} : split(vs0.at(1), std::string(","));
    if(vs0.at(0) == "broadcaster"){
      std::cout << "[read_input] broadcaster       to";
      for(auto const& item : vs1) std::cout << " " << std::setw(5) << item;
      std::cout << "\n";
      continue;
    }

    char prefix = vs0.at(0).at(0);
    std::string modname = vs0.at(0).substr(1, vs0.at(0).length()-1);
    if(prefix == '&'){
      std::cout << "[read_input] conjunction " << std::setw(5) << modname << " to";
      for(auto const& item : vs1) std::cout << " " << std::setw(5) << item;
      std::cout << "\n";
    }else if(prefix == '%'){
      std::cout << "[read_input] flip-flop   " << std::setw(5) << modname << " to";
      for(auto const& item : vs1) std::cout << " " << std::setw(5) << item;
      std::cout << "\n";
    }else assert(false);
  }
}

void solve()
{
}
