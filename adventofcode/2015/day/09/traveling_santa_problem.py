#!/bin/python
#
# python -m unittest

import unittest

to_index: dict[str, int] = {}
from_index: [str] = []

def record(loc: str) -> None:
    if not loc in to_index:
        to_index[loc] = len(to_index)
        from_index.append(loc)

#def transform(line: str) -> (org: str, dst: str, cost: int):
def transform(line: str) -> tuple:
    org, rest = line.split(' to ')
    dst, cost = rest.split(' = ')
    cost = int(cost)
    #print(f'{org} -> {dst}: {cost}')
    return org, dst, cost

def readinput():
    for line in open(0).readlines():
        org, dst, cost = transform(line)
        record(org), record(dst)

def main():
    readinput()
    print(to_index)
    print(from_index)
    print(to_index.keys())

class Test_TravelingSantaProblem(unittest.TestCase):
    def runTest(self):
        self.assertEqual(transform('A to B = 3'), ('A', 'B', 3))
        self.assertEqual(transform('edhv to ams = 200'), ('edhv', 'ams', 200))

if __name__ == '__main__':
    main()
