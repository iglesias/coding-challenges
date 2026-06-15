CXX      := clang++
COVFLAGS := -fprofile-instr-generate -fcoverage-mapping
CXXFLAGS := -std=c++23 -Wall -Wextra $(COVFLAGS) -O0 -g
AR       := llvm-ar
ARFLAGS  := rc
LDFLAGS  := $(COVFLAGS)

all: coverage

solver.o: solver.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

libsolver.a: solver.o
	$(AR) $(ARFLAGS) $@ $<

test: test.cpp libsolver.a
	$(CXX) $(CXXFLAGS) $^ -lgtest -o $@

run: test
	./test

coverage: run
	llvm-profdata merge -sparse -o default.profdata default.profraw
	llvm-cov report libsolver.a -instr-profile=default.profdata

#llvm-cov show libsolver.a -instr-profile=default.profdata -show-line-counts-or-regions -output-dir=coverage-report -format=html
#llvm-cov export libsolver.a -instr-profile=default.profdata > coverage.json

clean:
	rm -v *.o *.a test default.profdata default.profraw

.PHONY: all clean coverage run
