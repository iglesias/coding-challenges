#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
  // read the number of vertices and links
  int num_vertices, num_links;
//   cin >> num_vertices >> num_links;
  ifstream fin;
  fin.open("floyd.in");
  fin >> num_vertices >> num_links;

  // initialize matrix of distances with 1-based indices
//   int distance[num_vertices+1][num_vertices+1];
  vector<vector<int> > distance(num_vertices+1);
  for (int i = 1; i <= num_vertices; ++i) distance[i] = vector<int>(num_vertices+1);

  for (int i = 1; i <= num_vertices; ++i)
    for (int j = 1; j <= num_vertices; ++j)
    {
      if (i == j) distance[i][j] = 0;
      else        distance[i][j] = INT_MAX/2; // without /2, there is overflow in the min equation
    }

  // read links
  for (int i = 0; i < num_links; ++i)
  {
    int org, dst;
//     cin >> org >> dst;
    fin >> org >> dst;
    distance[org][dst] = 1;
  }
  
  fin.close();

  // Floyd-Warhsall
  for (int k = 1; k <= num_vertices; ++k)
    for (int i = 1; i <= num_vertices; ++i)
      for (int j = 1; j <= num_vertices; ++j)
          distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j]);

  // compute the diameter of the graph
  int diameter = 0;
  for (int i = 1; i <= num_vertices; ++i)
    for (int j = 1; j <= num_vertices; ++j)
      diameter = max(diameter, distance[i][j]);

  // display results
/*
  for (int i = 1; i <= num_vertices; ++i)
  {
    for (int j = 1; j <= num_vertices; ++j)
      cout << distance[i][j] << ' ';
    cout << endl;
  }
  cout << endl;
*/

  cout << "diameter=" << diameter << endl;

  return 0;
}
