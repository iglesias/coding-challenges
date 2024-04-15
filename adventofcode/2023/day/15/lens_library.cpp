#include <bits/stdc++.h>

int hash(int v, char c) { return ((v+int(c))*17)%256; }

int main()
{
  std::pair<int, int> ans;
  std::string step;
  int constexpr num_boxes = 256;
  std::array<std::vector<std::pair<std::string, int>>, num_boxes> boxes;
  while(std::getline(std::cin, step, ','))
  {
    step.erase(std::remove(step.begin(), step.end(), '\n'), step.end());

    {
      int v = 0;
      for(char c : step) v = hash(v, c);
      ans.first += v;
    }

    auto equals = step.find('=');
    auto dash   = step.find('-');
    auto label  = equals != std::string::npos ? step.substr(0, equals) : step.substr(0, dash);
    auto box = 0;
    for(char c : label) box = hash(box, c);

    if(equals != std::string::npos){
      int focal_length = std::stoi(step.substr(equals+1));
      bool break_for{false};
      for(std::size_t j = 0; j < boxes.at(box).size() and !break_for; j++)
      {
        if(boxes.at(box)[j].first == label)
        {
          boxes.at(box)[j].second = focal_length;
          break_for = true;
        }
      }
      if (!break_for)
        boxes[box].emplace_back(label, focal_length);
    }
    else if(dash != std::string::npos)
      std::erase_if(boxes.at(box), [&label](auto const& item){ return item.first == label; });
  }

  for(int j = 0; j < num_boxes; j++)
  {
    if(boxes[j].empty()) continue;
    for(int k = 0; k < (int)boxes[j].size(); k++) ans.second += (j+1)*(k+1)*boxes[j][k].second;
  }
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}
