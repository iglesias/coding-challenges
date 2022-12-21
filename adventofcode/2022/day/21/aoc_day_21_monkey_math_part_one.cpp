// g++ -std=c++11 -Wall -Wextra -pedantic -o aoc_day21_monkey_math_part_one aoc_day21_monkey_math.cpp
//
// ./aoc_day21_monkey_math_part_one < aoc_day21_monkey_math.1.in
// ./aoc_day21_monkey_math_part_one < aoc_day21_monkey_math.2.in
//

#include <bits/stdc++.h>

namespace
{
  using monkey = std::string;

  struct math_operation
  {
    std::array<monkey, 2> operands;
    char operation;
  };
}

using integer = long long int;

std::unordered_map<::monkey, integer> yelled_number_by;
std::unordered_map<::monkey, std::vector<::monkey>> dependents_of;
std::unordered_map<::monkey, ::math_operation> operation_of;

integer operate(integer a, integer b, char op)
{
  std::cout << a << " " << op << " " << b << std::endl;
  switch(op)
  {
    case '+':
      return a+b;
    case '-':
      return a-b;
    case '*':
      return a*b;
    case '/':
      return a/b;
    default:
      assert(false);
  }

  assert(false);
  return 0;
}

void read_input();
void print_input();
void party_yelling();
void fill_dependents();
void print_dependents();

int main()
{
  read_input();
  //print_input();
  fill_dependents();
  //print_dependents();
  party_yelling();
  std::cout << yelled_number_by.at("root") << std::endl;
}

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    std::string::size_type c{line.find(":")};
    ::monkey a{line.substr(0, c)};

    auto w = line.find(" ", c+2);
    if(w==std::string::npos)
    { // line with a single number
      int n{std::stoi(line.substr(c+2))};
      assert(yelled_number_by.find(a)==yelled_number_by.cend());
      yelled_number_by.insert(std::make_pair(a, n));
    }
    else
    { // line with operation
      ::math_operation math_operation;
      math_operation.operands[0] = line.substr(c+2, w-c-2);
      math_operation.operation = line.at(w+1);
      math_operation.operands[1] = line.substr(w+3);
      operation_of.insert(std::make_pair(a, std::move(math_operation)));
    }
  }
}

void print_input()
{
  for(const auto &kv : yelled_number_by)
    std::cout << kv.first << ": " << kv.second << std::endl;

  for(const auto &kv : operation_of)
    std::cout << kv.first << ": " << kv.second.operands[0] << ' ' << kv.second.operation << ' ' << kv.second.operands[1] << std::endl;
}

void party_yelling()
{
  // start finding the monkeys that are ready to start.
  std::queue<::monkey> monkeys_ready;

  for(const auto &kv : yelled_number_by)
  {
    monkeys_ready.push(kv.first);
  }

  while(!monkeys_ready.empty())
  {
    ::monkey monkey{monkeys_ready.front()};
    monkeys_ready.pop();
    std::cout << ">> Processing ready monkey " << monkey << std::endl;

    if(yelled_number_by.find(monkey)==yelled_number_by.cend())
    {
      assert(operation_of.find(monkey)!=operation_of.cend());
      ::math_operation math_operation = operation_of.at(monkey);

      auto it0 = yelled_number_by.find(math_operation.operands[0]);
      auto it1 = yelled_number_by.find(math_operation.operands[1]);
      assert(it0!=yelled_number_by.cend());
      assert(it1!=yelled_number_by.cend());

      integer n{operate(it0->second, it1->second, math_operation.operation)};
      yelled_number_by.insert(std::make_pair(monkey, n));
      std::cout << "     monkey yells " << n << std::endl;
    }

    if(dependents_of.find(monkey)==dependents_of.cend()) continue;

    for(::monkey dependent : dependents_of.at(monkey))
    {
      assert(operation_of.find(dependent)!=operation_of.cend());
      bool b0 = operation_of[dependent].operands[0]==monkey;
      bool b1 = operation_of[dependent].operands[1]==monkey;
      assert(b0 || b1);
      /*
      if(b0 || b1)
      {
      */
        assert(yelled_number_by.find(dependent)==yelled_number_by.cend());
        //std::cout << "    checking whether " << dependent << " becomes ready\n";
        bool b2 = yelled_number_by.find(operation_of[dependent].operands[0])!=yelled_number_by.cend();
        bool b3 = yelled_number_by.find(operation_of[dependent].operands[1])!=yelled_number_by.cend();
        if(b2 && b3)
        {
          //std::cout << "    " << dependent << " is now ready\n";
          monkeys_ready.push(dependent);
        }
      /*
      }
      */
    }
  }

  assert(yelled_number_by.find("root")!=yelled_number_by.cend());
}

void fill_dependents()
{
  for(const auto &kv : operation_of)
  {
    for(int i{0}; i<2; i++)
    {
      if(dependents_of.find(kv.second.operands[i])==dependents_of.cend())
      {
        dependents_of.insert(std::make_pair(kv.second.operands[i], std::vector<::monkey>(1, kv.first)));
      }
      else
      {
        assert(dependents_of.at(kv.second.operands[i]).size()>=1);
        dependents_of.at(kv.second.operands[i]).push_back(kv.first);
      }
    }
  }
}

void print_dependents()
{
  for(const auto &kv : dependents_of)
  {
    std::cout << kv.first << " <-";
    for(const auto &m : kv.second)
      std::cout << ' ' << m;
    std::cout << std::endl;
  }
}
