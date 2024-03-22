# The Identity function.
def I(a):
    return a


# The Self-Applicative function.
def M(f):
    return f(f)
  # M(M) ¬ β-normal form
