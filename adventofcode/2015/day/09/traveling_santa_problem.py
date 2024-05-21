#!/bin/python
#
# mypy
# python -m unittest

import numpy as np
import unittest

from collections import deque
from math import factorial
from time import time
from typing import Any, NewType

Cost = NewType('Cost', np.uint16)
Node = NewType('Node', np.uint8)
AdjList = NewType('AdjList', list[list[tuple[Node, Cost]]])

''' Over-engineered input reading '''


class InputReader:
    toidx: dict[str, Node] = {}
    fromidx: list[str] = []
    adjlist: Any = []

    @staticmethod
    def updmaps(loc: str) -> None:
        if loc not in InputReader.toidx:
            assert (len(InputReader.toidx) < 2**8-1)
            InputReader.toidx[loc] = Node(np.uint8(len(InputReader.toidx)))
            InputReader.fromidx.append(loc)

    @staticmethod
    def updlist(loc0: str, loc1: str, cost: Cost) -> None:
        while InputReader.toidx[loc0] >= len(InputReader.adjlist) or \
                InputReader.toidx[loc1] >= len(InputReader.adjlist):
            InputReader.adjlist.append([])
        InputReader.adjlist[InputReader.toidx[loc0]].append(
            (InputReader.toidx[loc1], cost))
        InputReader.adjlist[InputReader.toidx[loc1]].append(
            (InputReader.toidx[loc0], cost))

    @staticmethod
    def transform(line: str) -> tuple[str, str, Cost]:
        org, rest = line.split(' to ')
        dst, cost = rest.split(' = ')
        # print(f'{org} -> {dst}: {cost}')
        return org, dst, Cost(np.uint16(cost))

    @staticmethod
    def makedts() -> tuple[dict[str, Node], list[str], AdjList]:
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
        self.assertEqual(ir.transform('edhv to ams = 200'),
                         ('edhv', 'ams', 200))


def adjmat(adjlist: AdjList) -> np.ndarray:
    N = len(adjlist)
    A = np.zeros((N, N), dtype=np.uint16)
    for i in range(N):
        for (j, cost) in adjlist[i]:
            A[i, j] = cost
    return A


def bfs(A: np.ndarray, fromidx: list[str]) -> tuple[Cost, Cost]:
    N = A.shape[0]
    q: deque = deque()
    for i in range(N):
        q.append((i, set(range(N))-{i}, np.uint16(0), [fromidx[i]]))
    mincost = np.uint16(60000)
    maxcost = np.uint16(0)
    numpaths = 0
    while len(q) > 0:
        org, pending, cost, path = q.pop()
        if len(pending) == 0:
            # print(path, cost)
            mincost = min(mincost, cost)
            maxcost = max(maxcost, cost)
            numpaths += 1
        for dst in pending:
            if A[org][dst] > 0:
                q.append((dst, pending-{dst}, cost+A[org][dst],
                          path+[fromidx[dst]]))
    print(N, factorial(N), numpaths)
    return Cost(mincost), Cost(maxcost)


def dfs(A: np.ndarray, fromidx: list[str], org=-1, pending={}, cost=np.uint16(0), path=[]) -> tuple[Cost, Cost]:
    N = A.shape[0]
    mincost = np.uint16(60000)
    maxcost = np.uint16(0)
    if org == -1:
        # numpaths = 0
        for i in range(N):
            trymin, trymax = dfs(A, fromidx, i, set(range(N))-{i},
                                 np.uint16(0), [fromidx[i]])
            mincost = min(mincost, trymin)
            maxcost = max(maxcost, trymax)
        return Cost(mincost), Cost(maxcost)
    if len(pending) == 0:
        return cost, cost
    for dst in pending:
        if A[org][dst] > 0:
            trymin, trymax = dfs(A, fromidx, dst, pending-{dst}, cost+A[org][dst],
                                 path+[fromidx[dst]])
            mincost = min(mincost, trymin)
            maxcost = max(maxcost, trymax)
    return Cost(mincost), Cost(maxcost)


def main():
    _, fromidx, adjlist = InputReader.makedts()
    A = adjmat(adjlist)
    print(A)
    startbfs = time()
    mincost, maxcost = bfs(A, fromidx)
    endbfs = time()
    print('bfs:', endbfs-startbfs, mincost, maxcost)
    startdfs = time()
    mincost, maxcost = dfs(A, fromidx)
    enddfs = time()
    print('dfs:', enddfs-startdfs, mincost, maxcost)
    return mincost, maxcost


if __name__ == '__main__':
    shortest, longest = main()
    print('Part one:', shortest)
    print('Part two:', longest)
