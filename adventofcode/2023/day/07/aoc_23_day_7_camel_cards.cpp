#include <bits/stdc++.h>

struct hand_t
{
  std::string cards;
  int bid;
  std::map<char, uint> counter;
  std::string type;
  uint type_strength;

  bool operator<(hand_t const& that) const
  {
    if(this->type_strength != that.type_strength)
    {
      return this->type_strength < that.type_strength;
    }
    else
    {
      for(int i{0}; i < 5; i++)
      {
        if(this->cards[i]==that.cards[i]) continue;
        else
        {
          std::string_view constexpr ordered{"AKQT98765432J"};
          auto const this_idx{ordered.find(this->cards[i])};
          auto const that_idx{ordered.find(that.cards[i])};
          return this_idx > that_idx;
        }
      }
    }

    return false;
  }
};


std::vector<hand_t> read_input();
void process(hand_t& h);

int main()
{
  auto hands = read_input();
  for(auto& h : hands) process(h);
  std::sort(hands.begin(), hands.end());
  int rank{1};
  long long ans{0};
  for(auto const& h : hands) ans += rank++*h.bid;
  std::cout << ans << '\n';
}

std::vector<hand_t> read_input()
{
  std::vector<hand_t> hands;
  std::string line;
  while(std::getline(std::cin, line))
  {
    hand_t h;
    std::stringstream ss(line);
    ss >> h.cards >> h.bid;
    hands.push_back(h);
  }
  return hands;
}

void process(hand_t& h)
{
  uint max{0};
  for(char c : h.cards)
  {
    if(h.counter.contains(c)) h.counter.at(c) += 1;
    else                      h.counter.insert(std::make_pair(c, 1));
    max = std::max(max, h.counter.at(c));
  }

  uint num_pairs{0};
  for(uint n : std::views::values(h.counter)) num_pairs += (n == 2);

  std::string& type = h.type;
  switch(max)
  {
    case 5:
      type = "Five of a kind";
      h.type_strength = 7;
      break;
    case 4:
      if(h.counter.contains('J'))
      {
        type = "Five of a kind";
        h.type_strength = 7;
        break;
      }
      type = "Four of a kind";
      h.type_strength = 6;
      break;
    case 3:
      if(h.counter.contains('J'))
      {
        if(h.counter.at('J')==3)
        {
          if(num_pairs)
          {
            type = "Five of a kind";
            h.type_strength = 7;
            break;
          }
          else
          {
            type = "Four of a kind";
            h.type_strength = 6;
            break;
          }
        }
        else if(h.counter.at('J')==2)
        {
          type = "Five of a kind";
          h.type_strength = 7;
          break;
        }
        else
        {
          type = "Four of a kind";
          h.type_strength = 6;
          break;
        }
      }
      if(num_pairs)
      {
        type = "Full house";
        h.type_strength = 5;
      }
      else
      {
        type = "Three of a kind";
        h.type_strength = 4;;
      }
      break;
    case 2:
      if(h.counter.contains('J'))
      {
        if(h.counter.at('J') == 1)
        {
          if(num_pairs == 2)
          {
            type = "Full house";
            h.type_strength = 5;
            break;
          }
          else
          {
            type = "Three of a kind";
            h.type_strength = 4;;
            break;
          }
        }
        if(num_pairs == 2)
        {
          type = "Four of a kind";
          h.type_strength = 6;
          break;
        }
        else
        {
          type = "Three of a kind";
          h.type_strength = 4;;
          break;
        }
      }
      if(num_pairs==2) { type = "Two pair"; h.type_strength = 3; }
      else             { type = "One pair"; h.type_strength = 2; }
      break;
    case 1:
      if(h.counter.contains('J'))
      {
        type = "One pair";
        h.type_strength = 2;
        break;
      }
      type = "High card";
      h.type_strength = 1;
      break;
  }
}
