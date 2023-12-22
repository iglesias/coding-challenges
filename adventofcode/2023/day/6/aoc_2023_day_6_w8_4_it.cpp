#include <bits/stdc++.h>

#define   ALL(a)          (a).begin(), (a).end()

using integer = long long;
using time_and_distance = std::pair<integer, integer>;

using input_t = std::pair<std::vector<time_and_distance>, time_and_distance>;
// Prepares the input for both parts separately.
auto read_input() -> input_t;
integer solve_counting(std::vector<time_and_distance> const& vector_input);
integer solve_math(std::vector<time_and_distance> const& vector_input);
// Overloads for part two. They will be wrappers.
integer solve_counting(time_and_distance const&);
integer solve_math(time_and_distance const&);

int main()
{
  auto const input{read_input()};
  std::cout << "Part one: " << solve_counting(input.first) << " (counting)\n";
  std::cout << "Part one: " << solve_math(input.first) << " (w/ algebra)\n";
  std::cout << "Part two: " << solve_counting(input.second) << " (counting)\n";
  std::cout << "Part two: " << solve_math(input.second) << " (w/ algebra)\n";
}

auto read_input() -> std::pair<std::vector<time_and_distance>, time_and_distance>
{
  std::string time_data, distance_data;
  std::getline(std::cin, time_data);
  std::getline(std::cin, distance_data);

  time_data = time_data.substr(time_data.find(':')+1);
  distance_data = distance_data.substr(distance_data.find(':')+1);

  std::vector<time_and_distance> part_one;
  {
    std::stringstream ss(time_data);
    int number;
    while(!ss.eof())
    {
      ss >> number;
      part_one.push_back(std::make_pair(number, 0));
    }
    ss = std::stringstream(distance_data);
    std::size_t i{0};
    while(!ss.eof())
    {
      ss >> number;
      part_one[i++].second = number;
    }
  }

  time_data.erase(std::remove(ALL(time_data), ' '), time_data.end());
  distance_data.erase(std::remove(ALL(distance_data), ' '), distance_data.end());

  time_and_distance part_two{std::make_pair(stoll(time_data), stoll(distance_data))};

  return std::make_pair(part_one, part_two);
}

integer solve_counting(time_and_distance const& td)
{
  std::vector vector{td};
  return solve_counting(vector);
}

integer solve_counting(std::vector<time_and_distance> const& vector_input)
{
  std::vector<integer> num_ways;
  int const n{static_cast<int>(vector_input.size())};
  for(int i{0}; i < n; i++)
  {
    integer counter{0};
    auto const T{vector_input[i].first};
    auto const D{vector_input[i].second};
    for(integer t{1}; t < T; t++)
      if(t*(T-t) > D) counter++;
    num_ways.push_back(counter);
  }

  return std::accumulate(ALL(num_ways), 1, std::multiplies<unsigned long long>());
}

integer solve_math(time_and_distance const& td)
{
  std::vector vector{td};
  return solve_math(vector);
}

/*
 * For a race lasting `T` [milliseconds], if the button is held for
 * `time holding the button`, let's call it `t`, [milliseconds], then
 * the boat will have a speed of `t` [millimeters per millisecond]
 * for `T-t` [milliseconds].
 *
 * So, if the current record is `D` [millimeters], there will be a new
 * record if `t*(T-t) > D`, i.e.,
 *
 *           `t*T - t*t     > D`
 *           `t*t - T*t + D < 0`
 *
 */
integer solve_math(std::vector<time_and_distance> const& vector_input)
{
  std::vector<integer> num_ways;
  int const n{static_cast<int>(vector_input.size())};
  for(int i{0}; i < n; i++)
  {
    auto const T{vector_input[i].first};
    auto const D{vector_input[i].second};

    auto const T2{T*T};
    //dbp(T, T2, 4*D);
    auto const t1{((static_cast<double>(T)-std::sqrt(T2-4*D))/2)};
    auto const t2{((static_cast<double>(T)+std::sqrt(T2-4*D))/2)};
    //std::cout << "Solutions to the equation: " << t1 << ' ' << t2 << '\n';
    //if(std::round(t1)==t1){dbp(t1+1);} else{dbp(std::floor(t1));};
    auto const x{std::round(t1)==t1 ? t1+1 : std::floor(t1)};
    num_ways.push_back(static_cast<integer>(t2)-static_cast<integer>(x));
    //dbp(std::floor(t2));
  }

  return std::accumulate(ALL(num_ways), 1, std::multiplies<unsigned long long>());
}
