# python -m pytest aoc_2016_day_1.py -v
# mypy aoc_2016_day_1.py

import numpy as np

from typing import List

# N, E, S, W.
DIRECTIONS = [np.array(x) for x in [[0,1],[1,0],[0,-1],[-1,0]]]

def solve(line: str) -> int:
    position = np.array([0,0])
    heading  = np.array([0,1])
    direction = 0
    for item in line.split(','):
        item = item.strip()
        letter = item[0]
        digit = int(item[1:])
        adder = 1 if letter == 'R' else -1
        direction = (direction + adder) % 4
        heading = DIRECTIONS[direction]
        position += digit * heading
    return np.sum(np.abs(position))

if __name__ == '__main__':
    print(solve(open(0).readline().strip()))

import pytest

@pytest.mark.parametrize('i, o', [('R2, L3', 5), ('R2, R2, R2', 2), ('R5, L5, R5, R3', 12)])
def test_solve(i, o):
    assert solve(i) == o
