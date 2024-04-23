#include <deque>
#include <queue>
#include <stack>
#include <vector>

#include <gtest/gtest.h>

using std::deque;
using std::queue;
using std::stack;
using std::vector;

int countStudents(vector<int> const &students, vector<int> const &sandwiches) {
  queue studentsQ(deque(students.begin(), students.end()));
  stack sandwichesStack(deque(sandwiches.rbegin(), sandwiches.rend()));
  int numAttempted = 0;
  while (numAttempted <= static_cast<int>(studentsQ.size()) and
         studentsQ.size() and sandwichesStack.size()) {
    auto const student = studentsQ.front();
    studentsQ.pop();
    if (student == sandwichesStack.top()) {
      sandwichesStack.pop();
      numAttempted = 0;
    } else {
      studentsQ.push(student);
      numAttempted++;
    }
  }
  return static_cast<int>(studentsQ.size());
}

TEST(countStudents, a) {
  const vector<int> students = {0, 0, 0, 1, 1, 1, 1, 0, 0, 0};
  const vector<int> sandwiches = {1, 0, 1, 0, 0, 1, 1, 0, 0, 0};
  EXPECT_EQ(countStudents(students, sandwiches), 0);
}

TEST(countStudents, b) {
  const vector<int> students = {1, 1, 1, 0, 0, 1};
  const vector<int> sandwiches = {1, 0, 0, 0, 1, 1};
  EXPECT_EQ(countStudents(students, sandwiches), 3);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
