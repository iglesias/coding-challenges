// {{{ Boilerplate Code <--------------------------------------------------------------------------

#include <bits/stdc++.h>

template<typename string> auto split(std::string str, string sep) -> std::vector<std::string>
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

int hash(int v, char c) { return ((v+int(c))*17)%256; }

int main()
{
  std::pair<int, int> ans;
  std::string line;
  std::getline(std::cin, line);
  auto const steps = split(line, ',');
  int constexpr num_boxes = 256;
  std::array<std::vector<std::pair<std::string, int>>, num_boxes> boxes;
  for(auto const& step : steps) // range-based for loop
  {
    int v = 0;
    for(char c : step) v = hash(v, c);
    ans.first += v;

    auto equals = step.find('=');
    auto dash = step.find('-');
    auto label = equals != std::string::npos ? step.substr(0, equals) : step.substr(0, dash);
    auto box = 0;
    for(char c : label) box = hash(box, c);
    int focal_length = -1;
    if(equals != std::string::npos) focal_length = std::stoi(step.substr(equals+1));

    if(equals != std::string::npos){
      for(int j = 0; j < (int)boxes[box].size(); j++)
      {
        if(boxes[box][j].first == label)
        {
          boxes[box][j].second = focal_length;
          goto endloop;
        }
      }
      boxes[box].emplace_back(label, focal_length);
    } else if(dash != std::string::npos)
      std::erase_if(boxes[box], [&label](std::pair<std::string, int> const& item){return item.first == label; });
endloop:
  }

  for(int j = 0; j < num_boxes; j++)
  {
    if(boxes[j].empty()) continue;
    for(int k = 0; k < (int)boxes[j].size(); k++) ans.second += (j+1)*(k+1)*boxes[j][k].second;
  }
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}
