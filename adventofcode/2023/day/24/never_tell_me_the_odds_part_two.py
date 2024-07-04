import sympy

class Hailstone:
    def __init__(self, x0, y0, z0, vx, vy, vz):
        self.x0 = x0
        self.y0 = y0
        self.z0 = z0
        self.vx = vx
        self.vy = vy
        self.vz = vz

hailstones = []
for line in open('never_tell_me_the_odds_input.txt'):
    head, tail = line.split('@')
    x0, y0, z0 = [int(x) for x in head.strip().split(', ')]
    vx, vy, vz = [int(x) for x in tail.strip().split(', ')]
    hailstones.append(Hailstone(x0, y0, z0, vx, vy, vz))

n = len(hailstones)
eqs = []
for i in range(n):
    h = hailstones[i]
    x, y, z, vx, vy, vz = sympy.symbols('x y z vx vy vz')
    eqs.append((h.vy - vy)*(x - h.x0) - (h.vx - vx)*(y - h.y0))
    eqs.append((h.vz - vz)*(y - h.y0) - (h.vy - vy)*(z - h.z0))

solution = sympy.solve(eqs)

assert(len(solution) == 1)
solution = solution[0]
print('Part Two:', solution[x] + solution[y] + solution[z])
