import sys

data = []
part_one = 0

while True:
    levels = [int(x) for x in sys.stdin.readline().strip().split()]
    if len(levels) == 0: break
    data.append(levels)
    deltas = [b - a for a, b in zip(levels, levels[1:])]
    part_one += [x for x in deltas if 0 < abs(x) < 4] == deltas and all(x * deltas[0] >= 0 for x in deltas)

del deltas, levels

print('Part one:', part_one)

part_two = 0

def is_decreasing_type_of_safe(levels, switch=1):
    for i in range(len(levels) - 1):
        if not 1 <= levels[i] - levels[i + 1] <= 3:
            return switch and any(fun(levels[j-1:j] + levels[j+1:], 0) for j in (i, i + 1))
    return True

fun = is_decreasing_type_of_safe
for levels in data:
    part_two += fun(levels) or fun(levels[::-1])

print('Part two:', part_two)
