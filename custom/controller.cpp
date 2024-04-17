#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <queue>
#include <set>

// global variables
std::vector<std::string> panel;

struct Node
{
	int row;
	int col;
	std::string prefix;

	Node(int row, int col, std::string prefix)
	{
		this->row = row;
		this->col = col;
		this->prefix = prefix;
	}
};

struct NodeWithVisitedList : public Node
{
	std::vector<std::vector<bool>> visited;

	NodeWithVisitedList(int row, int col, std::string prefix, std::vector<std::vector<bool>> visited)
		: Node(row, col, prefix)
	{
		this->visited = visited;
	}
};


std::vector<std::string> bfs(NodeWithVisitedList start)
{
	std::vector<std::string> words;
	std::queue<NodeWithVisitedList> q;
	q.push(start);

	while (!q.empty())
	{
		NodeWithVisitedList n = q.front();
		q.pop();

		n.visited[n.row][n.col] = true;
		n.prefix += panel[n.row][n.col];

		if (n.prefix.length() >= 3)
			words.push_back(n.prefix);

		// up
		if (n.row > 0 && !n.visited[n.row-1][n.col])
			q.push(NodeWithVisitedList(n.row-1, n.col, n.prefix, n.visited));
		// down
		if (n.row < static_cast<int>(panel.size())-1 && !n.visited[n.row+1][n.col])
			q.push(NodeWithVisitedList(n.row+1, n.col, n.prefix, n.visited));
		// left
		if (n.col > 0 && !n.visited[n.row][n.col-1])
			q.push(NodeWithVisitedList(n.row, n.col-1, n.prefix, n.visited));
		// right
		if (n.col < static_cast<int>(panel[n.row].length())-1 && !n.visited[n.row][n.col+1])
			q.push(NodeWithVisitedList(n.row, n.col+1, n.prefix, n.visited));
	}

	return words;
}

std::vector<std::vector<bool>> visited;

void Merge(std::set<std::string>& in, const std::set<std::string>& list)
{
	for (auto it = list.begin(); it != list.end(); ++it)	in.insert(*it);
}

std::set<std::string> dfs(Node n)
{
	std::set<std::string> words;
	n.prefix.push_back(panel[n.row][n.col]);

	if (n.prefix.length() > 3)
		words.insert(n.prefix);

	if (visited.size() == 0)
	{
		visited.resize(panel.size());
		for (int i = 0; i < static_cast<int>(visited.size()); i++)
		{
			visited[i].resize(panel[i].length());
			for (int j = 0; j < static_cast<int>(visited[i].size()); j++)
				visited[i][j] = false;
		}
	}

	visited[n.row][n.col] = true;

	// up
	if (n.row > 0 && !visited[n.row-1][n.col])
		Merge(words, dfs(Node(n.row-1, n.col, n.prefix)));
	// down
	if (n.row < static_cast<int>(panel.size())-1 && !visited[n.row+1][n.col])
		Merge(words, dfs(Node(n.row+1, n.col, n.prefix)));
	// left
	if (n.col > 0 && !visited[n.row][n.col-1])
		Merge(words, dfs(Node(n.row, n.col-1, n.prefix)));
	// right
	if (n.col < static_cast<int>(panel[n.row].length()) && !visited[n.row][n.col+1])
		Merge(words, dfs(Node(n.row, n.col+1, n.prefix)));

	return words;
}

int main(int, char**)
{
	panel.push_back("AB");
	panel.push_back("DE");


	std::set<std::string> words = dfs(Node(0,0,std::string()));
	for (auto it = words.begin(); it != words.end(); ++it)
		std::cout << *it << ' ' << it->length() << std::endl;

	return 0;
}
