lens_mapping = {2: {1}, 3: {7}, 4: {4}, 5: {2, 3, 5}, 6: {0, 6, 9}, 7: {8}}
segments = [{0, 1, 2, 3, 4, 5}, {1, 2}, {0, 1, 3, 4, 6}, {0, 1, 2, 3, 6},
            {1, 2, 5, 6}, {0, 2, 3, 5, 6}, {0, 2, 3, 4, 5, 6}, {0, 1, 2},
            {0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 5, 6}]
segments_to_digit = {}
for digit, segment in enumerate(segments):
    segments_to_digit[frozenset(segment)] = digit

def decode(output_values, x):
    xi = 0
    solved = set()
    while any([len(item) > 1 for item in x]):
        if len(x[xi]) == 1 and xi not in solved:
            y = x[xi].pop()
            solved.add(y)
            x[xi].add(y)
        else:
            assert len(x[xi]) > 1
            x[xi] = set.difference(x[xi], solved)
            assert len(x[xi]) >= 1
        xi  = (xi + 1) % len(x)
    # output mapping obtained with the signal patterns:
    letter_to_segment = {}
    for j, xj in enumerate(x):
        letter_to_segment[xj.pop()] = j
    # used together with the output values to obtain the answer:
    ans = 0
    for output_value in output_values:
        # build segments on with this output_value 
        segments_on = set()
        for c in output_value:
            segments_on.add(letter_to_segment[c])
        ans *= 10
        ans += segments_to_digit[frozenset(segments_on)]
    return ans

# backtracking
def decode_segments(signal_patterns, i, x, used_digits):
    if i == len(signal_patterns):
        return x
    for digit in lens_mapping[len(signal_patterns[i])]:
        if digit in used_digits:
            continue
        # FIXME try to avoid x.copy()
        y = x.copy()
        stop = False
        for segment in segments[digit]:
            if len(set(signal_patterns[i]) & set(y[segment])) == 0:
                stop = True
            y[segment] = set(signal_patterns[i]) & set(y[segment])
        if not stop:
            used_digits.add(digit)
            r = decode_segments(signal_patterns, i + 1, y, used_digits)
            if r is not None:
                return r
            used_digits.remove(digit)
    return None

if __name__ == "__main__":
    part_one = 0
    part_two = 0
    for line in open(0).readlines():
        head, tail = line.split('|')
        output_values = tail.strip().split()
        for output_value in output_values:
            n = len(output_value)
            part_one += n == 2 or n == 4 or n == 3 or n == 7
        signal_patterns = head.split()
        signal_patterns.sort(key=lambda s: len(s))
        signal_patterns = [''.join(sorted(signal_pattern)) for signal_pattern in signal_patterns]
        x = decode_segments(signal_patterns, 0, 7*['abcdefg'], set()) 
        part_two += decode(output_values, x)

    print('Part one:', part_one)
    print('Part two:', part_two)
