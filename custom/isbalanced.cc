#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>

unsigned int linear;
unsigned int quadratic;

class BinaryTree {
	public:
		BinaryTree(int d);
		~BinaryTree();
		void insert(int d);
		void insertIterative(int d);
		static bool isBalanced(BinaryTree* tree);
		static int height(BinaryTree* tree);
		static bool isBalancedLinear(BinaryTree* tree);
		static int heightLinear(BinaryTree* tree);
		static void traverse(BinaryTree*);

	private:
		BinaryTree* left;
		BinaryTree* right;
		int data;
};

BinaryTree::BinaryTree(int d) {
	data = d;
	left = right = NULL;
}

BinaryTree::~BinaryTree() {
	if (left != NULL) {
		delete left;
	}

	if (right != NULL) {
		delete right;
	}

	left = right = NULL;
}

void BinaryTree::insert(int d) {
	if (d < data) {
		if (left == NULL) {
			left = new BinaryTree(d);
		} else {
			left->insert(d);
		}
	} else { // d > data -- assume no duplicate elements are inserted
		if (right == NULL) {
			right = new BinaryTree(d);
		} else {
			right->insert(d);
		}
	}
}

void BinaryTree::insertIterative(int d) {
	BinaryTree* cur = this;

	for (;;) {
		if (d < cur->data) {
			if (cur->left == NULL) {
				cur->left = new BinaryTree(d);
				break;
			} else {
				cur = cur->left;
			}
		} else { // d > data assume no duplicate elements are inserted
			if (cur->right == NULL) {
				cur->right = new BinaryTree(d);
				break;
			} else {
				cur = cur->right;
			}
		}
	}
}

bool BinaryTree::isBalanced(BinaryTree* tree) {
	if (tree == NULL)
		return true;

	int leftHeight = height(tree->left);
	int rightHeight = height(tree->right);
	std::cout << leftHeight << ' ' << rightHeight << std::endl;

	if (std::abs(leftHeight-rightHeight) > 1) {
		return false;
	} else {
		return isBalanced(tree->left) && isBalanced(tree->right);
	}
}

int BinaryTree::height(BinaryTree* tree) {
	quadratic++;

	if (tree == NULL)
		return 0;
	else
		return 1 + std::max(height(tree->left),height(tree->right));
}

bool BinaryTree::isBalancedLinear(BinaryTree* tree) {
	if (tree == NULL)
		return true;

	if (heightLinear(tree) == -1) {
		return false;
	} else {
		return true;
	}
}

int BinaryTree::heightLinear(BinaryTree* tree) {
	linear++;

	if (tree == NULL)
		return 0;

	int leftHeight = heightLinear(tree->left);
	if (leftHeight == -1)
		return -1;

	int rightHeight = heightLinear(tree->right);
	if (rightHeight == -1)
		return -1;

	int diffHeight = std::abs(leftHeight - rightHeight);	
	if (diffHeight > 1) {
		return -1;
	} else {
		return 1 + std::max(leftHeight,rightHeight);
	}
}

void BinaryTree::traverse(BinaryTree* tree) {
	if (tree != NULL) {
		std::cout << "traversing left" << std::endl;
		traverse(tree->left);
		std::cout << tree->data << std::endl;
		std::cout << "traversing right" << std::endl;
		traverse(tree->right);
	} else {
		std:: cout << "empty" << std::endl;
	}
}

int main() {
	srand(time(NULL));

/*
	BinaryTree* balancedTree = new BinaryTree(100);

	balancedTree->insert(90);
	balancedTree->insert(80);
	balancedTree->insert(120);
	balancedTree->insert(110);

	std::cout << BinaryTree::isBalanced(balancedTree) << std::endl;

	delete balancedTree;
*/

	int N;
	std::cin >> N;
	clock_t beginning;
	BinaryTree* unbalancedTree = new BinaryTree(0);
	for (int i = 1; i <= N; i++)
		unbalancedTree->insertIterative(rand()%10000);
	std::cout << "Tree built!" << std::endl;

	beginning = clock();
	std::cout << BinaryTree::isBalancedLinear(unbalancedTree) << std::endl;
	printf("%f\n",float(clock()-beginning)/CLOCKS_PER_SEC); 

	beginning = clock();
	std::cout << BinaryTree::isBalanced(unbalancedTree) << std::endl;
	printf("%f\n",float(clock()-beginning)/CLOCKS_PER_SEC); 

	std::cout << linear << ' ' << quadratic << std::endl;

// 	BinaryTree::traverse(unbalancedTree);

	delete unbalancedTree;

	return 0;
}
