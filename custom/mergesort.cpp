#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ctime>


using namespace std;

typedef unsigned int uint;

// main functions
vector<int> read();
void mergesort(vector<int>& v, vector<int>& aux, uint start=0, uint end=-1);

// helpers
void display(const vector<int>& v);

int main()
{
	// read input sequence of numbers
	vector<int> v = read();
	// auxiliary vector for mergesort
	vector<int> aux;
	aux.resize(v.size());

	// vector copy used to test result for correctness
	vector<int> vv(v);

	// sort the vector
	time_t start, end;
	time(&start);
	mergesort(v,aux);
	time(&end);
	cout << "Custom mergesort took " << difftime(end,start) << " seconds\n";

	// DEBUG, display the sorted vector
//	display(v);

	// test that the vector has been correctly sorted
	time(&start);
	sort(vv.begin(), vv.end());
	time(&end);
	cout << "STL sort took " << difftime(end,start) << " seconds\n";
	if (v==vv)
		cout << ":)\n";
	else
		cout << ":(\n";

	return 0;
}

vector<int> read()
{
	const uint SIZE = 1<<22;
	char line[SIZE];
	cin.getline(line, SIZE);
	// count the number of whitespace characters read
	int N=0;
	int i=0;
	while (line[i]!='\0')
	{
		if (line[i]==' ')
			N++;
		i++;
	}
	// the number of elements in the sequence is the number of spaces plus one
	N++;

	// DEBUG
	cout << "There are " << N << " elements in the sequence.\n";
	
	// put the number in the input sequence into a vector
	istringstream iss(line);
	vector<int> v(N);
	for (i=0; i<N; i++)
		iss >> v[i];

 	// DEBUG, display the sequence of numbers read	
//	cout << "Sequence in the vector:\n";
//	display(v);

	return v;
}

void display(const vector<int>& v)
{
	for (uint i=0; i < v.size(); i++)
		cout << v[i] << ' ';
	cout << endl;
}

void merge(vector<int>& v, vector<int>& aux, uint start, uint m, uint end);

void mergesort(vector<int>& v, vector<int>& aux, uint start, uint end)
{
	if (end==-1)
		end = v.size()-1;

	// base cases
	if (end==start+1)
	{
		if (v[start]>v[end])
		{
			int tmp = v[start];
			v[start] = v[end];
			v[end] = tmp;
		}
		return;
	}
	
	if (end==start)
		return;

	// compute the middle point
	uint m = end - (end-start+1)/2;

	mergesort(v,aux,start,m);
	mergesort(v,aux,m+1,end);
	merge(v,aux,start,m,end);
}

void merge(vector<int>& v, vector<int>& aux, uint start, uint m, uint end)
{
	for (uint i = start; i <= end; ++i)
		aux[i] = v[i];

	uint i=start;
	uint j=m+1;
	uint idx=start;

	while (i <= m && j <= end)
	{
		if (aux[i] <= aux[j])
		{
			v[idx] = aux[i];
			i++, idx++;
		}
		else
		{
			v[idx] = aux[j];
			j++, idx++;
		}
	}

	assert(i>m || j>end);

	while (i <= m)
	{
		v[idx] = aux[i];
		i++, idx++;
	}
}


void assign(vector<int>& lhs, const vector<int>& rhs)
{
	for (uint i=0; i<lhs.size(); i++)
		lhs[i] = rhs[i];
}
