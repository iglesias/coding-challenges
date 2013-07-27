#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

int loop(int n)
{
	int count = 1;

	while (n != 1)
	{
		if ((n%2)==1)
			n = 3*n+1;
		else
			n = n/2;

		count++;
	}

	return count;
}

int main()
{
	int i,j;
	char line[256];

	for (;;)
	{
		cin.getline(line, 256);
		if (strlen(line)==0)
			break;

		istringstream iss(line);

		iss >> i >> j;

		int result = 0;
		for (int n = min(i,j); n <= max(i,j); n++)
			result = max(result, loop(n));

		cout << i << ' ' << j << ' ' << result << endl;
	}

	return 0;
}
