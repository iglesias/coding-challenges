// {{{ Boilerplate Code <---------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#define   ALL(a)          (a).begin(), (a).end()

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

// -------------------------------------------------------------------------> End of Boilerplate }}}

//prefix %
struct flip_flop
{
  bool on;
  std::vector<std::string> outputs;
  std::string name;
  flip_flop(std::vector<std::string>&& outs, std::string const& _name)
    : on(false), outputs(outs), name(_name) {}
  //high pulse -> nothing
  //low pulse  -> flips on/off
  //  if off: turn on and send high pulse
  //  if on:  turn off and send low pulse
};

enum class Pulse { LOW, HIGH };
auto to_string(Pulse pulse){ if(pulse == Pulse::LOW) return "LOW"; return "HIGH"; };

//prefix &
struct conjunction //remember pulse of inputs
{
  std::unordered_map<std::string, Pulse> input_pulses;
  std::vector<std::string> outputs;
  std::string name;
  conjunction(std::vector<std::string>&& outs, std::string const& _name)
    : outputs(outs), name(_name) {}
};

using module = std::variant<flip_flop, conjunction>;

//broadcaster: list of destination modules
std::vector<std::string> broadcaster;
std::unordered_map<std::string, module> modules;
std::unordered_set<std::string> output_modules;
//FIXME scope globals.

void read_input();
int solve_part_one();
int64_t solve_part_two(int num_button_presses);

int main(int argc, char* argv[])
{
  const int num_button_presses = argc > 1 ? std::stoi(argv[1]) : 10000;
  read_input();
  std::cout << "Part one: " << solve_part_one() << ".\nPart two: " << solve_part_two(num_button_presses) << ".\n";
}

void read_input()
{
  auto get_vectors_from_first_split = [](std::string const& line){
    auto const vs0 = split(line, std::string(" -> "));
    auto vs1 = vs0.at(1).find(',') == std::string::npos ? std::vector{vs0.at(1)} : split(vs0.at(1), std::string(","));
    for(std::string& s : vs1) s.erase(std::remove(ALL(s), ' '), s.end());
    return std::make_pair(vs0, vs1);
  };

  std::vector<std::string> lines; // store input for a second pass

  std::string line;
  while(std::getline(std::cin, line)){
    lines.push_back(line);
    auto [vs0, vs1] = get_vectors_from_first_split(line);

    if(vs0.at(0) == "broadcaster"){
      broadcaster = std::move(vs1);
      continue;
    }

    char const prefix = vs0.at(0).at(0);
    std::string modname = vs0.at(0).substr(1, vs0.at(0).length()-1);
    if(prefix == '&'){
      modules.insert(std::make_pair(modname, conjunction(std::move(vs1), modname)));
    }else if(prefix == '%'){
      modules.insert(std::make_pair(modname, flip_flop(std::move(vs1), modname)));
    }
  }

  for(std::string const& line : lines){
    auto [vs0, vs1] = get_vectors_from_first_split(line);
    for(auto const& output : vs1){
      if(!modules.contains(output)){
        output_modules.insert(output);
        continue;
      }
      try{
        conjunction& c = std::get<conjunction>(modules.at(output));
        c.input_pulses.emplace(vs0.at(0).substr(1, vs0.at(0).length()-1), Pulse::LOW);
      }catch(std::bad_variant_access const& ex){
        continue;
      }
    }
  }
}

using queue = std::queue<std::tuple<std::string, Pulse, std::string>>;

void process(queue& q,      module& m, Pulse pulse, std::string const& from);
void process(queue& q, conjunction& c, Pulse pulse, std::string const& from);
void process(queue& q,   flip_flop& f, Pulse pulse);

void process(queue& q, conjunction& c, Pulse pulse, std::string const& from)
{
  c.input_pulses.at(from) = pulse;
  bool low_found = false;
  for(auto const& kv : c.input_pulses)
    if(kv.second == Pulse::LOW){ low_found = true; break; }
  Pulse pulse_to_send = low_found ? Pulse::HIGH : Pulse::LOW;
  for(std::string const& output : c.outputs)
    q.emplace(output, pulse_to_send, c.name);
}

void process(queue& q,  flip_flop& f, Pulse pulse)
{
  if(pulse == Pulse::HIGH) return;

  Pulse pulse_to_send = f.on ? Pulse::LOW : Pulse::HIGH;
  f.on = !f.on;
  for(std::string const& output : f.outputs)
    q.emplace(output, pulse_to_send, f.name);
}

template<class> inline constexpr bool always_false_v = false;
void process(queue& q, module& m, Pulse pulse, std::string const& from)
{
  std::visit([&q, &pulse, &from, &m](auto&& arg)
  {
    using T = std::decay_t<decltype(arg)>;
    if constexpr(std::is_same_v<T, flip_flop>)
      process(q, std::get<flip_flop>(m), pulse);
    else if constexpr(std::is_same_v<T, conjunction>)
      process(q, std::get<conjunction>(m), pulse, from);
  }, m);
}

int64_t solve(int num_button_presses, bool part_one)
{
  //aptly button module: it sends a low pulse to the broadcaster module.
  queue q;
  int num_low_pulses = num_button_presses, num_high_pulses = 0;
  int ans = 0;
  std::vector<int64_t> high_pulses_pl, high_pulses_mz, high_pulses_lz, high_pulses_zm;
  for(int n = 0; n < num_button_presses; n++){
    for(std::string const& m : broadcaster) q.emplace(m, Pulse::LOW, "broadcaster");
    while(!q.empty()){
      auto const& [m, pulse, from] = q.front();
      std::string const input_to_rx = "bn";
      if(m == input_to_rx){
        if(pulse == Pulse::HIGH){
          if(from == "pl") high_pulses_pl.push_back(n);
          if(from == "mz") high_pulses_mz.push_back(n);
          if(from == "lz") high_pulses_lz.push_back(n);
          if(from == "zm") high_pulses_zm.push_back(n);
        }
      }
      if(pulse == Pulse::LOW) num_low_pulses++; else num_high_pulses++;
      //std::cout << from << (pulse == Pulse::LOW ? " -low" : " -high") << "-> " << m << "\n";
      if(!output_modules.contains(m)) process(q, modules.at(m), pulse, from);
      q.pop();
    }
    if(n == num_button_presses-1) ans = num_low_pulses*num_high_pulses;
  }
  if(part_one) return ans;
  std::adjacent_difference(ALL(high_pulses_pl), high_pulses_pl.begin());
  std::adjacent_difference(ALL(high_pulses_mz), high_pulses_mz.begin());
  std::adjacent_difference(ALL(high_pulses_lz), high_pulses_lz.begin());
  std::adjacent_difference(ALL(high_pulses_zm), high_pulses_zm.begin());
  return high_pulses_pl[1]*high_pulses_mz[1]*high_pulses_lz[1]*high_pulses_zm[1];
}

int solve_part_one(){
  return solve(1000, true);
}

int64_t solve_part_two(int num_button_presses){
  return solve(num_button_presses, false);
}
