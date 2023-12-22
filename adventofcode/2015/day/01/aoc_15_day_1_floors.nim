# nim c aoc_a15_day_1_floors

import math
import sequtils

proc parenthesisToSign(p: char): int =
  if p == '(':
    return +1
  else:
    assert(p == ')')
    return -1

let input = readLine(stdin)
let signs = input.map(parenthesisToSign)

echo "Part one: ", signs.sum()

var floor = 0
for i in 0 .. len(input)-1:
  floor += parenthesisToSign(input[i])
  if floor == -1:
    echo "Part two: ", i+1
    break
