#include <iostream>
#include <set>

using namespace std;

int main()
{
  set<int> s;
  for (int n = -1000; n <= 1000; n++)
    s.insert((63*n + 7492)*5 - 498);

  for (auto it = s.begin(); it != s.end(); ++it)
    cout << *it << endl;  

  return 0;
}
