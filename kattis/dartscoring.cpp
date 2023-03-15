//https://www.geeksforgeeks.org/perimeter-of-convex-hull-for-a-given-set-of-points/
#include <bits/stdc++.h>
using scalar = double;
using namespace std;

struct Point {

	scalar x, y;

	bool operator<(Point p)
	{
		return x < p.x || (x == p.x && y < p.y);
	}
};

// Cross product of two vectors OA and OB
// returns positive for counter clockwise
// turn and negative for clockwise turn
scalar cross_product(Point O, Point A, Point B)
{
	return (A.x - O.x) * (B.y - O.y)
		- (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull
// in counter-clockwise order
vector<Point> convex_hull(vector<Point> A)
{
	int n = A.size(), k = 0;

	if (n <= 3)
		return A;

	vector<Point> ans(2 * n);

	// Sort points lexicographically
	sort(A.begin(), A.end());

	// Build lower hull
	for (int i = 0; i < n; ++i) {

		// If the point at K-1 position is not a part
		// of hull as vector from ans[k-2] to ans[k-1]
		// and ans[k-2] to A[i] has a clockwise turn
		while (k >= 2
			&& cross_product(ans[k - 2],
							ans[k - 1], A[i]) <= 0)
			k--;
		ans[k++] = A[i];
	}

	// Build upper hull
	for (size_t i = n - 1, t = k + 1; i > 0; --i) {

		// If the point at K-1 position is not a part
		// of hull as vector from ans[k-2] to ans[k-1]
		// and ans[k-2] to A[i] has a clockwise turn
		while (k >= t
			&& cross_product(ans[k - 2],
						ans[k - 1], A[i - 1]) <= 0)
			k--;
		ans[k++] = A[i - 1];
	}

	// Resize the array to desired size
	ans.resize(k - 1);

	return ans;
}

// Function to return the distance between two points
double dist(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x)
				+ (a.y - b.y) * (a.y - b.y));
}

// Function to return the perimeter of the convex hull
double perimeter(vector<Point> ans)
{
	double perimeter = 0.0;

	// Find the distance between adjacent points
	for (int i = 0; i < ans.size() - 1; i++) {
		perimeter += dist(ans[i], ans[i + 1]);
	}

	// Add the distance between first and last point
	perimeter += dist(ans[0], ans[ans.size() - 1]);

	return perimeter;
}

// Driver code
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  std::string line;
  while(std::getline(std::cin, line))
  {
    std::stringstream ss(line);
    vector<Point> points;
    while(!ss.eof())
    {
      float x, y;
      ss >> x >> y;
      points.push_back({x, y});
    }

    const int n{static_cast<int>(points.size())};
    const scalar s{perimeter(convex_hull(points))};
    std::cout << std::setprecision(10) << 100*n/(1+s) << '\n';
  }
}
