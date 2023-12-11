#include <bits/stdc++.h>

using image_t = std::vector<std::string>;

image_t image;
int R;
int C;
std::vector<std::pair<int, int>>  galaxies;

void print(image_t const& image)
{
  std::cout << '\n';
  for(int r=0; r<R; r++)
  {
    std::cout << image[r] << '\n';
  }
  std::cout << '\n';
}

std::vector<int> free_rows, free_cols;

void read_input();
std::pair<long long, long long> ans;
void find_free_rows_and_cols();
long long solve_part_one();
long long solve_part_two();
long long fw(std::vector<std::pair<int, int>> const&);

int main()
{
  read_input();
  //std::cout << "Input image:\n"; print(image);
  find_free_rows_and_cols();
  ans.first = solve_part_one();
  ans.second = solve_part_two();
  std::cout << "Part one: " << ans.first << "\nPart two: " << ans.second << '\n';
}

void read_input()
{
  std::string line;
  while(std::getline(std::cin, line))
  {
    image.push_back(line);
    if(C==0) C = (int)line.length();
    assert(C==(int)line.length());
    R++;
  }
}

void find_free_rows_and_cols()
{
  for(int r=0; r<R; r++)
  {
    bool found_galaxy{false};
    for(int c=0; c<C and !found_galaxy; c++)
      found_galaxy = image[r][c] != '.';
    if(!found_galaxy) free_rows.push_back(r);
  }
  for(int c=0; c<C; c++)
  {
    bool found_galaxy{false};
    for(int r=0; r<R and !found_galaxy; r++)
      found_galaxy = image[r][c] != '.';
    if(!found_galaxy) free_cols.push_back(c);
  }
}

long long aggregate_pairwise_distances()
{
  const int N = (int)galaxies.size();
  long long ans = 0;
  for(int i=0; i<N; i++)
  {
    for(int j=i+1; j<N; j++) // Manhattan
    {
      ans += std::abs(galaxies[i].first-galaxies[j].first) + std::abs(galaxies[i].second-galaxies[j].second);
    }
  }
  return ans;
}

long long solve_part_one()
{
  // Suppose the order of expanding rows or cols first doesn't matter

  auto expanded_image = image;
  // expand free rows
  int expanded_rows_counter = 0;
  int expanded_R = R;
  for(int free_row : free_rows)
  {
    expanded_image.insert(expanded_image.begin()+free_row+expanded_rows_counter, std::string(C, '.'));
    expanded_R++;
    expanded_rows_counter++;
  }

  //print(expanded_image);

  // expand free cols
  int expanded_cols_counter = 0;
  int expanded_C = C;
  for(int free_col : free_cols)
  {
    for(int r = 0; r < expanded_R; r++)
      expanded_image[r].insert(free_col+expanded_cols_counter, 1, '.');
    expanded_C++;
    expanded_cols_counter++;
  }

  //std::cout << "Expanded image:\n"; print(expanded_image);

  // find galaxies in expanded image (universe)
  galaxies.clear();
  for(int r=0; r<expanded_R; r++) for(int c=0; c<expanded_C; c++)
    if(expanded_image[r][c]=='#') galaxies.emplace_back(r, c);

  return aggregate_pairwise_distances();
}

long long solve_part_two()
{
  int constexpr expansion_factor = 1000000;
  // find galaxies in image (universe)
  galaxies.clear();
  for(int r=0; r<R; r++) for(int c=0; c<C; c++)
    if(image[r][c]=='#') galaxies.emplace_back(r, c);

  int expanded_rows_counter = 0;
  for(int free_row : free_rows)
  {
    for(auto& galaxy : galaxies)
      if(galaxy.first > free_row+expanded_rows_counter*(expansion_factor-1)) galaxy.first += expansion_factor-1;
    expanded_rows_counter++;
  }

  int expanded_cols_counter = 0;
  for(int free_col : free_cols)
  {
    for(auto& galaxy : galaxies)
      if(galaxy.second > free_col+expanded_cols_counter*(expansion_factor-1)) galaxy.second += expansion_factor-1;
    expanded_cols_counter++;
  }

  return aggregate_pairwise_distances();
}
