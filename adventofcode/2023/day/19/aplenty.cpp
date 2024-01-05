#include <bits/stdc++.h>
#include <boost/numeric/interval.hpp>

struct step
{
  std::string str;
  char cat;
  char op;
  int bound;
  std::string dest;
};

struct workflow
{
  std::string name;
  std::vector<step> steps;
  std::string dest;
};

struct rating
{
  int x, m, a, s;
  rating(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}

  int get_category_v(char catchar) const
  {
    if(catchar=='x') return x;
    if(catchar=='m') return m;
    if(catchar=='a') return a;
    else             return s;
  }
};

using workflows_t = std::unordered_map<std::string, workflow>;

auto read_input() -> std::pair<workflows_t, std::vector<rating>>;

int       solve_part_one(workflows_t const& workflows, std::vector<rating> const& ratings);
long long solve_part_two(workflows_t const& workflows);

int main()
{
  auto const [workflows, ratings] = read_input();
  std::cout << "Part one: " << solve_part_one(workflows, ratings);
  std::cout<<".\nPart two: " << solve_part_two(workflows) << ".\n";
}

auto read_input() -> std::pair<workflows_t, std::vector<rating>>
{
  std::string line;
  workflows_t workflows;

  while(std::getline(std::cin, line)){
    if(line.empty()) break;

    auto pos = line.find('{');
    workflow wf;
    wf.name = line.substr(0, pos);
    for(;;){
      auto comma_pos = line.find(',', pos+1);
      if(comma_pos != std::string::npos){
        step s;
        s.str = line.substr(pos+1, comma_pos-pos-1);
        auto op_pos = s.str.find('<');
        if(op_pos == std::string::npos){
          op_pos = s.str.find('>');
        }
        s.cat = s.str.at(op_pos-1);
        s.op = s.str.at(op_pos);
        auto colon_pos = s.str.find(':');
        s.bound = std::stoi(s.str.substr(op_pos+1, colon_pos-op_pos-1));
        s.dest = s.str.substr(colon_pos+1);
        wf.steps.push_back(s);
      }else{
        wf.dest = line.substr(pos+1, line.length()-1-pos-1);
        workflows.insert(std::make_pair(wf.name, wf));
        break;
      }

      pos = comma_pos;
    }
  }

  std::regex const regex("\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}");
  std::vector<rating> ratings;
  while(std::getline(std::cin, line)){
    std::smatch matches;
    std::regex_match(line, matches, regex);
    ratings.emplace_back(std::stoi(matches[1].str()), std::stoi(matches[2].str()),
                         std::stoi(matches[3].str()), std::stoi(matches[4].str()));
  }

  return std::make_pair(workflows, ratings);
}

auto apply(workflow const& wf, rating const& r) -> std::string
{
  for(step const& s : wf.steps){
    int const v = r.get_category_v(s.cat);
    if(s.op=='>' and v > s.bound) return s.dest;
    if(s.op=='<' and v < s.bound) return s.dest;
  }
  return wf.dest;
}

int solve_part_one(workflows_t const& workflows, std::vector<rating> const& ratings)
{
  int ans = 0;
  for(rating const& r : ratings){
    auto ret = apply(workflows.at("in"), r);
    while(ret != "A" and ret != "R"){
      ret = apply(workflows.at(ret), r);
    }
    if(ret == "A") ans += r.x + r.m + r.a + r.s;
  }
  return ans;
}

using boost::numeric::interval;

struct rating_intervals
{
  static int constexpr max_rating = 4000;

  std::vector<interval<int>> x, m, a, s;

  rating_intervals()
  {
    x.emplace_back(1, 4000);
    m.emplace_back(1, 4000);
    a.emplace_back(1, 4000);
    s.emplace_back(1, 4000);
  }

  rating_intervals(rating_intervals const& other) : x(other.x), m(other.m), a(other.a), s(other.s){}

  std::vector<interval<int>>& get_category(char catchar)
  {
    if(catchar=='x') return x;
    if(catchar=='m') return m;
    if(catchar=='a') return a;
    else             return s;
  };
};

auto make_step_interval(char op, int bound) -> interval<int> {
  if(op=='>') return interval(bound+1, rating_intervals::max_rating);
  else        return interval(1, bound-1);
}

auto make_other_step_interval(char op, int bound) -> interval<int> {
  if(op=='>') return interval(1, bound);
  else        return interval(bound, rating_intervals::max_rating);
}

long long apply(std::string const& cur_wf_name, rating_intervals ri, workflows_t const& workflows)
{
  static long long ans = 0;

  auto const f = [](std::vector<interval<int>>& v, interval<int> const& to_intersect) {
      std::vector<interval<int>> w;
      for(interval<int> const& i : v)
        try{
          w.push_back(boost::numeric::intersect(i, to_intersect));
        }catch(std::runtime_error& runtime_error){
          assert(std::strcmp(runtime_error.what(),"boost::interval: empty interval created") == 0);
          ;
        }
      v = w;
  };

  if(cur_wf_name == "A"){
    ans += (long long)(ri.x.at(0).upper()-ri.x.at(0).lower()+1) *
           (long long)(ri.m.at(0).upper()-ri.m.at(0).lower()+1) *
           (long long)(ri.a.at(0).upper()-ri.a.at(0).lower()+1) *
           (long long)(ri.s.at(0).upper()-ri.s.at(0).lower()+1);
    return 0;
  }

  if(cur_wf_name == "R") return 0;

  workflow const& wf = workflows.at(cur_wf_name);

  for(step const& s : wf.steps){
    rating_intervals newri(ri);
    f(newri.get_category(s.cat), make_step_interval(s.op, s.bound));
    apply(s.dest, newri, workflows);

    f(ri.get_category(s.cat), make_other_step_interval(s.op, s.bound));
  }

  apply(wf.dest, ri, workflows);

  return ans;
}

long long solve_part_two(workflows_t const& workflows)
{
  return apply("in", rating_intervals(), workflows);
}
