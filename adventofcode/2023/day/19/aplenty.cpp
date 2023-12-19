// {{{ Boilerplate Code <---------------------------------------------------------------------------

#include <bits/stdc++.h>

#define   PRINT(...)      print(#__VA_ARGS__, __VA_ARGS__)

using ii = std::pair<int, int>;
using ll = long long;

template<typename T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  os << "vector(" << (v.size() ? '\0' : ')');

  for(size_t i{0}; i < v.size(); i++)
    os << v[i] << ((i == v.size()-1)? ")" : ", ");

  return os;
}

template<typename... Args> void print(std::string vars, Args&&... values)
{
  std::cout << "[" << vars << " = ";
  std::string delim = "";
  (..., (std::cout << delim << values, delim = ", "));
  std::cout << "]\n";
}

// -------------------------------------------------------------------------> End of Boilerplate }}}

struct step
{
  std::string str;
  // category
  char cat;
  // > or <
  char op;
  int bound;
  // one of destination workflow (e.g. "abc"), (2) rejected ("R"), or accepted ("A")
  std::string dest;
};

struct workflow
{
  std::string name;
  std::vector<step> steps;
  // destination if no other rules match the part
  std::string dest;
};

struct rating
{
  int x, m, a, s;
};

std::ostream& operator<<(std::ostream& os, step const& s)
{
  os << s.cat << s.op << s.bound << ":" << s.dest;
  return os;
}

std::ostream& operator<<(std::ostream& os, workflow const& wf)
{
  os << wf.name << "{";
  for(auto const& s : wf.steps) os << s << ", ";
  os << wf.dest << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, std::unordered_map<std::string, workflow> workflows)
{
  for(auto it = workflows.cbegin(); it != workflows.cend(); it++){
    auto const& [name, workflow] = *it;
    assert(name == workflow.name);
    os << workflow << (std::next(it)==workflows.cend() ? "" : ", ");
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, rating const& r)
{
  os << "{x=" << r.x << ",m=" << r.m << ",a=" << r.a << ",s=" << r.s << "}";
  return os;
}

auto read_input() -> std::pair<std::unordered_map<std::string, workflow>, std::vector<rating>>;
ii ans;
void solve(std::unordered_map<std::string, workflow> const& workflows, std::vector<rating> const& ratings);

int main()
{
  auto const [workflows, ratings] = read_input();
  //PRINT(workflows, ratings);
  solve(workflows, ratings);
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

auto read_input() -> std::pair<std::unordered_map<std::string, workflow>, std::vector<rating>>
{
  std::string line;
  std::unordered_map<std::string, workflow> workflows;
  // Parse workflows:
  while(std::getline(std::cin, line)){
    if(line.empty()) break;

    auto pos = line.find('{');
    workflow wf;
    wf.name = line.substr(0, pos);
    for(;;){
      //PRINT(pos+1);
      auto comma_pos = line.find(',', pos+1);
      if(comma_pos != std::string::npos){
        step s;
        //PRINT(pos+1, comma_pos-pos-1);
        s.str = line.substr(pos+1, comma_pos-pos-1);
        auto op_pos = s.str.find('<');
        if(op_pos == std::string::npos){
          op_pos = s.str.find('>');
          assert(op_pos != std::string::npos);
        }
        s.cat = s.str.at(op_pos-1);
        s.op = s.str.at(op_pos);
        auto colon_pos = s.str.find(':');
        assert(colon_pos != std::string::npos);
        s.bound = std::stoi(s.str.substr(op_pos+1, colon_pos-op_pos-1));
        s.dest = s.str.substr(colon_pos+1);
        //PRINT(op_pos, colon_pos, s);
        wf.steps.push_back(s);
      }else{
        wf.dest = line.substr(pos+1, line.length()-1-pos-1);
        workflows.insert(std::make_pair(wf.name, wf));
        break;
      }

      pos = comma_pos;
    }
    //PRINT(line, wf);
  }

  // Parse part ratings ("{x=288,m=156,a=17,s=52}"):
  std::regex const regex("\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}");
  std::vector<rating> ratings;
  while(std::getline(std::cin, line)){
    std::smatch matches;
    assert(std::regex_match(line, matches, regex));
    assert(matches.size()==5);
    ratings.emplace_back(std::stoi(matches[1].str()), std::stoi(matches[2].str()),
                         std::stoi(matches[3].str()), std::stoi(matches[4].str()));
  }

  return std::make_pair(workflows, ratings);
}

auto apply(workflow const& wf, rating const& r) -> std::string
{
  for(step const& s : wf.steps){
    int v;
    if(s.cat=='x'){
      v = r.x;
    }else if(s.cat=='m'){
      v = r.m;
    }else if(s.cat=='a'){
      v = r.a;
    }else if(s.cat=='s'){
      v = r.s;
    }else assert(false);

    if(s.op=='>'){
      if(v > s.bound) return s.dest;
    }else if(s.op=='<'){
      if(v < s.bound) return s.dest;
    }else assert(false);
  }
  return wf.dest;
}

void solve(std::unordered_map<std::string, workflow> const& workflows, std::vector<rating> const& ratings)
{
  //PRINT(ratings[0]);
  for(rating const& r : ratings){
    std::cout << r << ":";
    auto ret = apply(workflows.at("in"), r);
    std::cout << "in -> " << ret << " ";
    while(ret != "A" and ret != "R"){
      ret = apply(workflows.at(ret), r);
      std::cout << "-> " << ret << (ret == "A" or ret == "R" ? "\n" : " ");
    }
    if(ret == "A") ans.first += r.x + r.m + r.a + r.s;
  }
}
