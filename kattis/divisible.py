import sys

T = int(raw_input())

for t in range(T):
  d, n = map(int, sys.stdin.readline().split())
  L    = map(int, sys.stdin.readline().split())
  S    = [0]*d
  ans  = 0
  s    = 0
  for i in range(n):
    s      += L[i]
    mod     = s % d
    ans    += S[mod]
    ans    += not(mod)
    S[mod] += 1

  print ans
