#!/usr/bin/python

from itertools import permutations

O = int(input())
Z = int(input())

s: str = '0'*Z + '1'*O

for t in permutations(s):
    last_char_count: int = 1
    last_char: str = t[0]
    found_invalid = False
    i = 1
    while i < len(t) and not found_invalid:
        if t[i] == last_char: last_char_count += 1
        else:
            last_char_count = 1
            last_char = t[i]
        if last_char_count >= 3: found_invalid = True
        i += 1
    if found_invalid: continue
    print(''.join(t))
    break
