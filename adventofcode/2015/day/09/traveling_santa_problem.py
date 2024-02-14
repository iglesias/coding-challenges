#!/bin/python
#
# python -m unittest

import numpy as np
import unittest

from collections import deque

''' Over-engineered input reading '''

# FIXME class input reader wrapping readinput call
# with toidx and fromidx members
def updatemaps(loc: str, toidx: dict[str, int], fromidx: [str]) -> None:
    if not loc in toidx:
        toidx[loc] = len(toidx)
        fromidx.append(loc)

def updatelist(loc0: str, loc1: str, cost: int,
               toidx: dict[str, int],adjlist) -> None:
    while toidx[loc0] >= len(adjlist) or toidx[loc1] >= len(adjlist):
        adjlist.append([])
    adjlist[toidx[loc0]].append((toidx[loc1], cost))
    adjlist[toidx[loc1]].append((toidx[loc0], cost))

def transform(line: str) -> tuple[str, str, np.uint16]:
    org, rest = line.split(' to ')
    dst, cost = rest.split(' = ')
    cost = np.uint16(cost)
    #print(f'{org} -> {dst}: {cost}')
    return org, dst, cost

def readinput() -> tuple[dict[str, int], [str]]:
    toidx: dict[str, int] = {}
    fromidx: [str] = [] # FIXME needed? With toidx.keys() I am
                        # obtaining the same as it seems that the
                        # dict preserves order of insertion, which
                        # is how fromidx is filled.
    adjlist: [[(int, np.uint16)]] = []
    for line in open(0).readlines():
        org, dst, cost = transform(line)
        updatemaps(org, toidx, fromidx)
        updatemaps(dst, toidx, fromidx)
        updatelist(org, dst, cost, toidx, adjlist)
    return toidx, fromidx, adjlist

class TestInput(unittest.TestCase):
    def runTest(self):
        self.assertEqual(transform('A to B = 3'), ('A', 'B', 3))
        self.assertEqual(transform('edhv to ams = 200'), ('edhv', 'ams', 200))


def adjmat(adjlist) -> np.array:
    N = len(adjlist)
    A = np.zeros((N, N), dtype=np.uint16)
    for i in range(N):
        for (j, cost) in adjlist[i]:
            A[i,j] = cost
    return A

def main():
    toidx, fromidx, adjlist = readinput()
    print(toidx)
    print(fromidx)
    print(toidx.keys())
    print(adjlist)
    # Number of nodes.
    assert(len(toidx) == len(fromidx) and len(toidx) == len(adjlist))
    N = len(toidx)
    # Adjacency matrix.
    A = adjmat(adjlist)
    print(A)
    q = deque()
    for i in range(N):
        q.append((i, {i}, set(range(N))-{i}, 0))
    while len(q) > 0:
        loc, visited, pending, cost = q.pop()
        print(loc, visited, pending, cost)
        #TODO

if __name__ == '__main__':
    main()
