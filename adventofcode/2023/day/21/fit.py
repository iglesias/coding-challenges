import numpy as np
import sys

num_garden_plots = np.loadtxt(sys.argv[1], dtype=int)
num_steps_taken = [i for i in range(1, len(num_garden_plots) + 1)]
#print([item for item in zip(num_steps_taken, num_garden_plots)])
assert(max(num_steps_taken) == 500)

import matplotlib.pyplot as plt

#plt.plot(num_steps_taken, num_garden_plots, '+')
#plt.plot(num_steps_taken[0:-1], np.diff(num_garden_plots))
#plt.show()



from scipy.optimize import curve_fit

# Assuming
# - x: independent variable (e.g., data point index)
# - y: dependent variable (sequence values)
x = np.array(num_steps_taken[0:-1])
y = np.diff(num_garden_plots)

#print(x[0:5], y[0:5])

from numpy.polynomial import Polynomial

p = Polynomial.fit(x, y, deg=2)  # Try different degrees
trend = p(x)

#plt.plot(x, y - trend)
#plt.show()

from scipy.fft import fft, fftfreq

residuals = y - trend
N = len(residuals)
T = 1.0  # Assuming time step of 1
yf = fft(residuals)
xf = fftfreq(N, T)[:N//2]

#plt.plot(xf, 2.0/N * np.abs(yf[:N//2]))
#plt.grid()
#plt.show()

from scipy.optimize import curve_fit

def harmonic_model(x, *params):
    result = np.zeros_like(x, dtype=np.float64)
    num_harmonics = len(params) // 2
    for i in range(num_harmonics):
        amplitude = params[2 * i]
        frequency = params[2 * i + 1]
        result += amplitude * np.sin(2 * np.pi * frequency * x)
    return result

initial_guess = []
initial_guess.extend([218, 0.5])
initial_guess.extend([40, 0.48])
#initial_guess.extend([15, 0.46])
params, params_covariance = curve_fit(harmonic_model, x, residuals, p0=initial_guess)
harmonics = harmonic_model(x, *params)

final_model = trend + harmonics
plt.plot(x, final_model)
plt.show()

sys.exit(0)

def func(x, A, C, w, phi):
  return A * x + C * np.cos(w * x + phi)

# Define initial guess for the parameters (adjust as needed)
p0 = [1, 1000, 2*np.pi, np.pi/2]  # A, C, omega, phi

# Perform the curve fitting
params, cov = curve_fit(func, x, y, p0=p0)

# Print the fitted parameters
print("Fitted parameters:")
print("A:", params[0])
print("C:", params[1])
print("omega:", params[2])
print("phi:", params[3])

# Generate fitted curve using the fitted parameters
fitted_curve = func(np.array(x), *params)

#plt.plot(x, fitted_curve)
#plt.show()
