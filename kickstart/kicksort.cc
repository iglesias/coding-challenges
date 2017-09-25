#include <cmath>
#include <cstdio>
#include <ext/pb_ds/assoc_container.hpp>
#include <set>
#include <string>
#include <vector>

using namespace __gnu_pbds;

template <typename T>
using order_statistics_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

std::string solve(const std::vector<int>& A)
{
  order_statistics_set<size_t> indices;
  for (size_t i = 0; i < A.size(); i++) indices.insert(i);

  std::set<int> items_left;
  for (const auto& a : A) items_left.insert(a);

  while (indices.size() > 2)
  {
    size_t pivot_position = std::floor((1.0*indices.size() - 1)/2);
    size_t index = *indices.find_by_order(pivot_position);
    int pivot = A[index];
    if (pivot == *items_left.begin() || pivot == *items_left.rbegin()) {
      indices.erase(index);
      items_left.erase(pivot); 
    } else {
      return "NO";
    }
  }

  return "YES";
}

int main()
{
  int T, N;
  scanf("%d", &T);

  for (int t = 0; t < T; t++) {
    scanf("%d", &N);

    std::vector<int> A(N);
    for (int n = 0; n < N; n++) scanf("%d ", &A[n]);
    printf("Case #%d: %s\n", t+1, solve(A).c_str());
  }
}
