#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int solve(int n)
{
  n = (n*63 + 7492)*5 - 498;
  n = abs(n);
  n /= 10;
  return n%10;
}

int main()
{
  cout << "#include <iostream>" << endl;
  cout << "using namespace std;" << endl;
  cout << "int ar[]={";
  for (int n = -1000; n <= 1000; n++)
    printf("%d%s", solve(n), (n == 1000)? "" : ",");

  cout << "};" << endl;
  cout << "int main()\n{" << endl;
  cout << "int t;" << endl;
  cout << "cin >> t;" << endl;
  cout << "for (int i = 0; i < t; i++) { int n; cin >> n; cout << ar[n+1000] << endl; }" << endl;
  cout << "return 0;\n}" << endl;
  return 0;
}
