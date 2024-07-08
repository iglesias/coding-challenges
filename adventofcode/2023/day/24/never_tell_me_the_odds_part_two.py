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

symbols_str = 'x y z vx vy vz'
num_symbols = len(symbols_str.split())
eqs = []
i = -1
for n in range(3, len(hailstones)):
    for i in range(i + 1, n):
        h = hailstones[i]
        x, y, z, vx, vy, vz = sympy.symbols(symbols_str)
        eqs.append((h.vy - vy)*(x - h.x0) - (h.vx - vx)*(y - h.y0))
        eqs.append((h.vz - vz)*(y - h.y0) - (h.vy - vy)*(z - h.z0))
    i = n - 1
    solution = sympy.solve(eqs)
    if len(solution) == 1 and len(solution[0].keys()) == num_symbols:
        break

print(solution)
solution = solution[0]
print('Part Two:', solution[x] + solution[y] + solution[z])
