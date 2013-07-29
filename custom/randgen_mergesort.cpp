#include <iostream>
#include <cstdlib>

using namespace std;

typedef unsigned int uint;

int main()
{
	srand(0);	
	
	// read from standard input the length of the sequence to generate
	uint N;
	cin >> N;	

	// generate N numbers in the closed interval [-100,100] and write them to stdout
	for (uint i=0; i<N; i++)
	{
		cout << (rand() % 201) - 100;

		if (i==N-1)
			cout << endl;
		else
			cout << ' ';
	}

	return 0;
}
