#include <iostream>
#include <cstdlib>

typedef unsigned int uint;

template <typename T>
void swap(T* lhs, T* rhs)
{
	T tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

template <typename T>
struct matrix {
	uint num_rows;
	uint num_cols;
	T* data;
	void display() const;
	void transpose();
};

template <typename T>
void matrix<T>::display() const
{
	for (uint i = 0; i < num_rows; i++) {
		for (uint j = 0; j < num_cols-1; j++)
			std::cout << data[j*num_rows + i] << ' ';

		std::cout << data[(num_cols-1)*num_rows + i] << std::endl;
	}
}

template <typename T>
void matrix<T>::transpose()
{
	for (uint i = 0; i < num_rows-1; i++) {
		for (uint j = i+1; j < num_cols; j++)
			swap(data+(j*num_rows+i), data+(i*num_cols+j));
	}
}

int main()
{
	matrix<int> M;
	M.num_rows = 5;
	M.num_cols = 5;
// 	M.data = static_cast<int*>(std::malloc(sizeof(int)*M.num_rows*M.num_cols));
	M.data = new int[M.num_rows*M.num_cols];

	if (M.data == NULL)
		return -1;

	for (uint i = 0; i < M.num_rows*M.num_cols; i++)
		M.data[i] = i;

	M.display();
	M.transpose();
	std::cout << std::endl;
	M.display();

// 	free(M.data);
	delete[] M.data;
	return 0;
}
