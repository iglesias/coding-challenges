#include <bits/stdc++.h>

void read_input_and_solve();
std::pair<int_fast64_t, int_fast64_t> ans;

int main()
{
  read_input_and_solve();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

std::vector<int> split(std::string const& str, char sep)
{
  std::string::size_type pos{0}, n;
  std::vector<int> out;

  while((n = str.find(sep, pos)) != std::string::npos)
  {
    out.push_back(std::stoi(str.substr(pos, n-pos)));
    pos = n+1;
  }

  if(pos != 0) out.push_back(std::stoi(str.substr(pos)));

  return out;
}

int_fast64_t solve(int c_i, int r_i, int n_s);

std::string condition;
std::vector<int> record;

void read_input_and_solve()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    std::string record_str;
    std::stringstream ss(line);
    ss >> condition >> record_str;
    record = split(record_str, ',');

    ans.first += solve(0, 0, 0);

    /* part two hack */
    std::string original_condition = condition;
    int record_original_size = (int)record.size();
    for(int i = 0; i < 4; i++)
    {
      condition += "?" + original_condition;
      for(int j = 0; j < record_original_size; j++) record.push_back(record[j]);
    }
    ans.second += solve(0, 0, 0);
  }
}

int_fast64_t solve(int c_i, int r_i, int n_s)
{
  if(c_i >= (int)condition.size())
  {
    if(r_i >= (int)record.size()) return 1;
    else                          return r_i == (int)record.size()-1 and n_s == record[r_i];
  }

  if(r_i >= (int)record.size())
  {
    for(; c_i < (int)condition.size(); c_i++) if(condition[c_i] == '#') return 0;
    return 1;
  }

  if(n_s > record[r_i]) return 0;

  static std::map<std::tuple<int, int, int>, int_fast64_t> solved_backups;
  if(c_i == 0 and r_i == 0 and n_s == 0) solved_backups.clear();
  auto key = std::make_tuple(c_i, r_i, n_s);

  int_fast64_t option_damaged = 0, option_operational = 0;
  switch(condition[c_i])
  {
    case '?':
      if(solved_backups.contains(key)) return solved_backups.at(key);
      option_damaged = solve(c_i+1, r_i, n_s+1);
      option_operational = n_s == record[r_i] ? solve(c_i+1, r_i+1, 0) : !n_s ? solve(c_i+1, r_i, 0) : 0;
      solved_backups.insert(std::make_pair(key, option_damaged + option_operational));
      return option_damaged + option_operational;
    case '.':
      if(!n_s)                    return solve(c_i+1, r_i, 0);
      else if(n_s != record[r_i]) return 0;
      else                        return solve(c_i+1, r_i+1, 0);
    case '#':
      if(n_s+1 > record[r_i]) return 0;
      else                    return solve(c_i+1, r_i, n_s+1);
  }
  return 0; // unreachable
}
