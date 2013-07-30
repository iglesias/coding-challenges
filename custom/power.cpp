#include <iostream>

using namespace std;

int power(int a, int k)
{
	if (k == 1)
	{
		return a;
	}
	else
	{
		int x = power(a, k/2);
		x = x*x;

		if ((k % 2) == 0)
		{
			return x;
		}
		else
		{
			return a*x;
		}
	}
}

int main()
{
	int a,k;
	cin >> a >> k;
	cout << power(a,k) << endl;	

	return 0;
}
