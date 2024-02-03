def flippingBits(n):
    print('[INFO] n =', n)
    print('[INFO] Python\'s builtin binary:', bin(n))
    # Write your code here
    nbin = 0
    powerOfTen = 1
    numberOfBits = 0
    while n > 0:
        nbin += (n%2)*powerOfTen
        powerOfTen *= 10
        numberOfBits += 1
        n //= 2
    print('[INFO] My binary:', nbin, numberOfBits, 'bit(s)')
    prefixbin = 0
    for i in range(32-numberOfBits): # 32-bit unsigned integers
        prefixbin += 1
        if i < 32-numberOfBits-1: prefixbin *= 10
    print('[INFO] Left-most bits set to 1 to pad to 32 bits:', prefixbin)
    suffixbin = []
    while nbin > 0:
        if nbin%2 == 0: suffixbin.append('1')
        else:           suffixbin.append('0')
        nbin //= 10
    suffixbin = ''.join([bit for bit in reversed(suffixbin)])
    print('[INFO] My binary representation with bits flipped:', suffixbin)
    powerOfTwo = 1
    ans = 0
    print('[INFO] Answer in binary: ', str(prefixbin) + suffixbin)
    for bit in reversed(str(prefixbin) + suffixbin):
        if bit == '1':  ans += powerOfTwo
        powerOfTwo *= 2
    return ans

print(flippingBits(2147483647))
print(flippingBits(1))
print(flippingBits(0))
print(flippingBits(2))
print(flippingBits(4))
print(flippingBits(123456))
