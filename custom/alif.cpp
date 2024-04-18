#include <algorithm>
#include <ios>
#include <iostream>
#include <string>

int main()
{
  int O, Z;
  std::cin >> Z >> O;
  if (std::cin.rdstate() != std::ios_base::goodbit) {
    std::cerr << "alif: wrong input.\nUsage: alif integer integer\n"
              << "                ^       ^\n            #zeros   #ones\n";
    return 1;
  }
  std::string str(Z, '0');
  str.append(O, '1');
  std::cout << str << '\n';
  int permutation_i = 0;
  while(std::next_permutation(str.begin(), str.end())){
    permutation_i++;
    if(not permutation_i%1000000) std::cout << ">> " << str << '\n';
    int last_char_count = 1;
    char last_char = str[0];
    bool found_invalid = false;
    for(int i = 1; i < static_cast<int>(str.length()) and not found_invalid; i++){
      if(str[i] == last_char) last_char_count += 1;
      else {
        last_char_count = 1;
        last_char = str[i];
      }
      if(last_char_count >= 3) found_invalid = true;
    }
    if(found_invalid) continue;
    std::cout << str << '\n';
    break;
  }
}
