#!/bin/python
#
# python -m unittest

import numpy as np
import unittest

from collections import deque
from math import factorial
from time import time

''' Over-engineered input reading '''

class InputReader:
    toidx: dict[str, int] = {}
    fromidx: [str] = [] # FIXME needed? With toidx.keys() I am
                        # obtaining the same as it seems that the
                        # dict preserves order of insertion, which
                        # is how fromidx is filled.
    adjlist: [[(int, np.uint16)]] = []

    @staticmethod
    def updmaps(loc: str) -> None:
        if not loc in InputReader.toidx:
            InputReader.toidx[loc] = len(InputReader.toidx)
            InputReader.fromidx.append(loc)

    @staticmethod
    def updlist(loc0: str, loc1: str, cost: int) -> None:
        while InputReader.toidx[loc0] >= len(InputReader.adjlist) or \
                InputReader.toidx[loc1] >= len(InputReader.adjlist):
            InputReader.adjlist.append([])
        InputReader.adjlist[InputReader.toidx[loc0]].append((InputReader.toidx[loc1], cost))
        InputReader.adjlist[InputReader.toidx[loc1]].append((InputReader.toidx[loc0], cost))

    @staticmethod
    def transform(line: str) -> tuple[str, str, np.uint16]:
        org, rest = line.split(' to ')
        dst, cost = rest.split(' = ')
        cost = np.uint16(cost)
        #print(f'{org} -> {dst}: {cost}')
        return org, dst, cost

    @staticmethod
    def makedts() -> tuple[dict[str, int], [str], [[(int, np.uint16)]]]:
        ''' Make Data structures from lines in stdin '''
        for line in open(0).readlines():
            org, dst, cost = InputReader.transform(line)
            InputReader.updmaps(org)
            InputReader.updmaps(dst)
            InputReader.updlist(org, dst, cost)
        return InputReader.toidx, InputReader.fromidx, InputReader.adjlist


class TestInputReader(unittest.TestCase):
    def runTest(self):
        ir = InputReader()
        self.assertEqual(ir.transform('A to B = 3'), ('A', 'B', 3))
        self.assertEqual(ir.transform('edhv to ams = 200'), ('edhv', 'ams', 200))


def adjmat(adjlist) -> np.array:
    N = len(adjlist)
    A = np.zeros((N, N), dtype=np.uint16)
    for i in range(N):
        for (j, cost) in adjlist[i]:
            A[i,j] = cost
    return A


def bfs(A: np.array, fromidx: [str]) -> (np.uint16, np.uint16):
    assert(A.shape[0] == A.shape[1])
    N = A.shape[0]
    q = deque()
    for i in range(N):
        q.append((i, {i}, set(range(N))-{i},
                  np.uint16(0), [fromidx[i]]))
    mincost = np.uint16(60000)
    maxcost = np.uint16(0)
    numpaths = 0
    while len(q) > 0:
        org, visited, pending, cost, path = q.pop()
        if len(pending) == 0:
            #print(path, cost)
            mincost = min(mincost, cost)
            maxcost = max(maxcost, cost)
            numpaths += 1
        for dst in pending:
            if A[org][dst] > 0:
                q.append((dst, visited|{dst}, pending-{dst},
                          cost+A[org][dst], path+[fromidx[dst]]))
    print(N, factorial(N), numpaths)
    return mincost, maxcost


def main():
    toidx, fromidx, adjlist = InputReader.makedts()
    assert(len(toidx) == len(fromidx) and len(toidx) == len(adjlist))
    N = len(toidx)
    A = adjmat(adjlist)
    print(A)
    startbfs = time()
    mincost, maxcost = bfs(A, fromidx)
    endbfs = time()
    print(endbfs-startbfs)
    return mincost, maxcost


if __name__ == '__main__':
    shortest, longest = main()
    print('Part one:', shortest)
    print('Part two:', longest)
