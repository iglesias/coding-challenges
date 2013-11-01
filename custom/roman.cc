#include <iostream>
#include <unordered_map>
#include <utility>

using namespace std;

unordered_map<char, int> MAP;

void InitializeMap()
{
  MAP.insert(make_pair('C', 100));
  MAP.insert(make_pair('X', 10));
  MAP.insert(make_pair('V', 5));
  MAP.insert(make_pair('I', 1));
}

int RomanToDecimal(const string& str)
{
  int dec = 0;
  unsigned int i = 0;
  while (i < str.size())
  {
    if (i < str.size()-1 && MAP[str[i]] < MAP[str[i+1]])
    {
      dec += MAP[str[i+1]] - MAP[str[i]];
      i += 2;
    }
    else
    {
      dec += MAP[str[i]];
      i++;
    }
  }

  return dec;
}

int main(int argc, char** argv)
{
  InitializeMap();

  if (argc > 1)
    cout << RomanToDecimal(argv[1]) << endl;
  else
    cout << RomanToDecimal("XIX") << endl;

  return 0;
}
