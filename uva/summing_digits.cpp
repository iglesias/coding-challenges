#include <iostream>

using namespace std;

typedef unsigned long long ull;

ull sum_digits(ull n)
{
	// base case -  1-digit numbers
	if (n / 10 == 0) return n;

	// recursion
	int digits_sum = 0;
	while (n > 0)
	{
		digits_sum += n%10;
		n /= 10;
	}

	return sum_digits(digits_sum);
}

int main(int, char**)
{
	int n;

	for (;;)
	{
		cin >> n;
		if (n == 0) break;
		else cout << sum_digits(n) << endl;
	}
}
