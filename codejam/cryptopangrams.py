import sys

def gcd(a, b):
   if b == 0:
      return a
   else:
      return gcd(b, a%b)

T = int(input())

for t in range(T):
   N, L = map(int, sys.stdin.readline().split())
   cipher = list(map(int, sys.stdin.readline().split()))

   primes = set()
   for l in range(L-1):
      if cipher[l] != cipher[l+1]:
         gcd_v = gcd(cipher[l], cipher[l+1])
         primes.add(gcd_v)
         primes.add(cipher[l] // gcd_v)
         primes.add(cipher[l+1] // gcd_v)

   primes = list(primes)
   primes.sort()

   letter_to_prime = {}
   prime_to_letter = {}
   for i, prime in enumerate(primes):
      letter_to_prime[chr(ord('A')+i)] = prime
      prime_to_letter[prime] = chr(ord('A')+i)

   l = 0
   while cipher[l] == cipher[l+1]:
      l += 1

   gcd_v = gcd(cipher[l], cipher[l+1])
   pangram = ['x']*(L+1)

   # First different.
   pangram[l+1] = prime_to_letter[gcd_v]

   # Propagate left.
   for i in range(l, -1, -1):
      val = cipher[i] // letter_to_prime[pangram[i+1]]
      pangram[i] = prime_to_letter[val]

   # Propagate right.
   for i in range(2, L+1):
      pangram[i] = prime_to_letter[cipher[i-1] // letter_to_prime[pangram[i-1]]]

   print("Case #%d: %s" % (t+1, str().join(pangram)))
