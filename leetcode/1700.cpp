#include <deque>
#include <queue>
#include <random>
#include <stack>
#include <vector>

#include <gtest/gtest.h>

using std::deque;
using std::queue;
using std::stack;
using std::vector;

namespace sim {

int countStudents(vector<int>& students, vector<int>& sandwiches) {
    int i = 0, j = 0;
    for (int k = 0; k < 100 and i < static_cast<int>(sandwiches.size()); k++) {
        if (sandwiches[i] == students[j]) {
            i++;
            students.erase(students.cbegin() + j);
        } else {
            j = (j + 1)%static_cast<int>(students.size());
        }
    }
    return static_cast<int>(students.size());
}

}  // namespace sim

namespace dts {

int countStudents(vector<int> const& students, vector<int> const& sandwiches) {
    queue studentsQ(deque(students.begin(), students.end()));
    stack sandwichesStack(deque(sandwiches.rbegin(), sandwiches.rend()));
    int numAttempted = 0;
    while (numAttempted <= static_cast<int>(studentsQ.size()) and studentsQ.size() and sandwichesStack.size()) {
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

}  // namespace dts

TEST(countStudents, SampleInput) {
    vector<int> students = {0,0,0,1,1,1,1,0,0,0};
    vector<int> sandwiches = {1,0,1,0,0,1,1,0,0,0};
    EXPECT_EQ(dts::countStudents(students, sandwiches), sim::countStudents(students, sandwiches));
}

TEST(countStudents, RandomizedInput) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randi(50, 100);
    vector<int> students(randi(gen));
    for(auto& student : students) student = randi(gen)%2;
    vector<int> sandwiches(randi(gen));
    for(auto& sandwich : sandwiches) sandwich = randi(gen)%2;
    EXPECT_EQ(dts::countStudents(students, sandwiches), sim::countStudents(students, sandwiches));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
