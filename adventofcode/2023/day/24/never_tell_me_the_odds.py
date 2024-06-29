import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import sys

# TODO rename to plot context.
def init():
    ax.set(xlim=[320870677764563 -40*100, 320870677764563], ylim=[335750934489987 -20*100, 335750934489987], zlim=[282502845957937, 282502845957937 + 10*100], xlabel='x', ylabel='y', zlabel='z')
    pass

# TODO rename to plot context.
def data_gen():
    p0 = np.array(positions[0])
    v = np.array(velocities[0])
    for t in range(100):
        yield p0 + v*t, p0 + v*(t + 1)

def run(data):
    p, q = data
    ax.plot([p[0], q[0]], [p[1], q[1]], [p[2], q[2]], '+-', color='black')

positions = []
velocities = []

while True:
    line = sys.stdin.readline()
    if len(line) == 0: break
    head, tail = line.split('@')
    px, py, pz = [int(x) for x in head.split(', ')]
    vx, vy, vz = [int(x) for x in tail.split(', ')]
    positions.append([px, py, pz])
    velocities.append([vx, vy, vz])

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ani = animation.FuncAnimation(fig, run, data_gen, init_func=init, cache_frame_data=False, repeat=False)
plt.show()
