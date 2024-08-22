import math

from typing import Final

input_hex = 0xA0016C880162017C3686B18A3D4780

min_num_bits: Final[int] = 11

def version_and_next(s: str, i: int) -> (int, int):
    if i + min_num_bits >= len(s):
        return (0, len(s))

    # 3 first packet bits encode the version.
    version = int(s[(i + 0):(i + 3)], 2)
    # the next three the type ID.
    typeid = int(s[(i + 3):(i + 6)], 2)

    if typeid == 4:  # literal packet
        (_, j) = literal_packet(s, i + 6) # +6 from version and type bits
        return (version, j)
    else:  # operator packet
        length_type_ID = s[i + 6] 
        if length_type_ID == '0':
            subpackets_num_bits = int(s[(i + 7):(i + 7 + 15)], 2)
            subpackets_start = i + 7 + 15
        elif length_type_ID == '1':
            num_subpackets = int(s[(i + 7):(i + 7 + 11)], 2)
            subpackets_start = i + 7 + 11
        return (version, subpackets_start)

def packet(s: str, i = 0) -> (int, int): # -> (value, next_index)
    # bits 4 to 6 (the next 3 bits after the 3 first ones w/ version)
    # encode the type ID.
    typeid = int(s[(i + 3):(i + 6)], 2)
    if typeid == 4:
        return literal_packet(s, i + 6) # +6 from version and type bits
    else:  # operator packet
        return operator_packet(s, i + 6, typeid)

def operator_packet(s: str, i: int, typeid: int) -> int:
    length_type_ID = s[i] 
    if length_type_ID == '0':
        return operator_packet_length_type_ID_0(s, i, typeid) 
    elif length_type_ID == '1':
        return operator_packet_length_type_ID_1(s, i, typeid)

def literal_packet(s: str, j: int) -> (int, int):
    literal = ''
    # Blocks of 5 bits for the literal value,
    # where the first bit in each block signals whether it's the last block.
    while s[j] != '0':
        literal+= s[(j + 1):(j + 5)] # each block of 5 bits that starts with 1 isn't the last block
        j += 5
    literal += s[(j + 1):(j + 5)]
    return (int(literal, 2), j + 5)

def operator_packet_length_type_ID_0(s: str, i: int, typeid: int) -> (int, int):
    subpackets_num_bits = int(s[(i + 1):(i + 16)], 2)
    j = i + 16
    values = []
    while j - i - 16 + 1 < subpackets_num_bits:
        (value, j) = packet(s, j)
        values.append(value)
    return (operate(values, typeid), j)

def operator_packet_length_type_ID_1(s: str, i: int, typeid: int) -> (int, int):
    num_subpackets = int(s[(i + 1):(i + 12)], 2)
    j = i + 12
    values = []
    for n in range(num_subpackets):
        (value, j) = packet(s, j) 
        values.append(value)
    return (operate(values, typeid), j)

def operate(values, typeid) -> int:
    if typeid < 4:
        return [sum, math.prod, min, max][typeid](values)
    else:
        return eval(str(values[0]) + ['>', '<', '=='][typeid - 5] + str(values[1]))
        '''
        match typeid:
            case 5:
                return int(values[0] > values[1])
            case 6:
                return int(values[0] < values[1])
            case 7:
                return int(values[0] == values[1])
        '''

if __name__ == "__main__":

    s = bin(input_hex)[2:]
    while len(s) % 4 != 0:
        s = '0' + s

    (version, i) = version_and_next(s, 0)
    part_one_ans: int = version
    while i < len(s):
        (version, i) = version_and_next(s, i)
        part_one_ans += version
    print('Part one:', part_one_ans)
    print('Part two:', packet(s)[0])


def test_literal():
    assert literal_packet(bin(0xD2FE28)[2:], 6)[0], 2021

def test_operator_packet_sum():
    assert packet(bin(0xC200B40A82)[2:])[0] == 3

def test_operator_packet_prod():
    s = bin(0x04005AC33890)[2:]
    while len(s) % 4 != 0:
        s = '0' + s
    s = '0000' + s  # because the input "string" starts with a zero...
    assert packet(s)[0] == 54

def test_operator_packet_min():
    assert packet(bin(0x880086C3E88112)[2:])[0] == 7

def test_operator_packet_max():
    assert packet(bin(0xCE00C43D881120)[2:])[0] == 9

def test_operator_packet_gt():
    assert not packet(bin(0xF600BC2D8F)[2:])[0]

def test_operator_packet_lt():
    assert packet(bin(0xD8005AC2A8F0)[2:])[0]

def test_operator_packet_eq():
    assert not packet(bin(0x9C005AC2F8F0)[2:])[0]

def test_nested_operators():
    assert packet(bin(0x9C0141080250320F1802104A08)[2:])[0] == 1
