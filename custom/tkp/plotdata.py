import matplotlib.pyplot as plt
import numpy as np

stream = open(0)

# TODO put both in a function
D, P = stream.readline().strip().split()
D = int(D)
P = int(P)
dx = np.zeros(D, dtype=int)
dy = np.zeros(D, dtype=int)
for i in range(D):
    left, right = stream.readline().strip().split(',')
    dx[i] = int(left[1:])
    dy[i] = int(right[:-1])

px = np.zeros(P, dtype=int)
py = np.zeros(P, dtype=int)
for i in range(P):
    left, right = stream.readline().strip().split(',')
    px[i] = int(left[1:])
    py[i] = int(right[:-1])

plt.scatter(dx, dy, marker='v')
plt.scatter(px, py, marker='^')

plt.show()
