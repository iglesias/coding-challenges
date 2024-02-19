# Approach

Dynamic programming to compute the deliveries. Then, search for a route going through the computed deliveries (respecting the problem restrictions, e.g. start and end in depot at origin) and one pickup, iterating through all pickups and keeping the shortest route.

# Usage instructions

Requirements:
- C++ compiler with C++23 (developed with gcc 13.2.1 20230821 on Arch).
- The data generator (dagaten.cpp) uses [fmtlib](https://github.com/fmtlib/fmt).
- gtest for building and running the test.

Compile:

`g++ -std=c++23 -o datagen datagen.cpp`
`g++ -std=c++23 -o main main.cpp solver.cpp`

Run:

`./datagen | ./main`

Note that the data generation may include a delivery or pickup co-located with the depot. 
The solver is robust enough to handle it.
Using the hardcoded values in datagen.cpp, expect 4-5 deliveries (increasing the hardcoded Q in datagen.cpp will increase them, as expected - recall to recompile datagen if doing so).

Unit tests:

`g++ -std=c++23 -o test test.cpp solver.cpp -lgtest`


# Extensions

- Leverage that the vehicle’s used capacity is dynamic. Then, simultaneously optimize the deliveries together with the pickup to choose. This could be done adding an additional dimension to the memoization table for the pickups as well as whether a pickup has been chosen and if affirmative the cost it involves in the route.
- After selecting deliveries, build the route and incorporate pickup greedily.
- Further test coverage.
- Refactor usage to CMakeLists.txt.
- Other TODO comments in the code.
