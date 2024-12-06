import numpy as np
import matplotlib.pyplot as plt

file = "../build/output/decay_time_al_2000.txt"

data = np.loadtxt(file)

nbins = int(np.sqrt(len(data)))

hist, bin = np.histogram(data, nbins)

bin = bin[:-1]
# hist = hist/(bin[1] - bin[0]) ASK THIS

# We expect an exponential. Perform the fit using the least squares method (log -> lin).
a, b = np.polyfit(bin[(hist != 0) & (bin < 10e-6)], np.log(hist[(hist != 0) & (bin < 10e-6)]), 1)

mean_time = - 1 / a
print(f'Muon lifetime = {mean_time} s')

total_time = bin[-1]

time_vec = np.arange(0, total_time, 1e-8)


# ----------------------------------------------------------------------------------------------------
# ERROR ANALYSIS
# ----------------------------------------------------------------------------------------------------

# We are going to calculate the error of the fit
# x axis: time. The uncertainty is 20 ns (ASK THIS)
x_error = 20e-9
# y axis: logarithm of number of counts

x = bin[hist != 0]
delta_x = x_error * np.ones_like(x)

y = np.log(hist[hist != 0])
delta_y = 1 / np.sqrt( len(y) )

# ERROR DUE TO UNCERTAINTIES IN MEASUREMENT
n = len(x)
sigma_N_squared = np.sum(((y - y.mean()) * (1 - 1 / len(y)))**2 * delta_x**2)
sigma_D_squared = np.sum((2 * (x - x.mean()) - (2 / len(x)) * np.sum(x - x.mean()))**2 * delta_x**2)

numerator = np.sum(x - x.mean() * (y - y.mean()))
denominator = np.sum( (x - x.mean())**2 )

sigma_m_exp_x = np.sqrt( np.sum( ((n * x - np.sum(x)) / (n * sum(x**2) - sum(x)**2))**2 * delta_y**2 ) )
sigma_m_exp = np.sqrt((sigma_N_squared / denominator**2) + (numerator**2 * sigma_D_squared / denominator**4) + sigma_m_exp_x**2)

# ERROR DUE TO STATISTICAL UNCERTAINTIES
sigma_x = np.sqrt( 1/len(x) * np.sum( (x - x.mean())**2 ) )
sigma_y = np.sqrt( 1/len(y) * np.sum(( y - y.mean() )**2) )
sigma_xy = 1/len(x) * np.sum( x * y - x.mean()*y.mean() )

r = sigma_xy / (sigma_x * sigma_y)
R2 = r**2

sigma_m_est = abs(a) * np.sqrt( (r**(-2) - 1)/(len(x) - 2) )

sigma_a_total = np.sqrt(sigma_m_est**2 + sigma_m_exp**2)

mean_time_error = abs( mean_time / a * sigma_a_total )

print(f"Error = {mean_time_error} s")

plt.figure()
plt.plot(time_vec*1e6, np.exp(b) * np.exp(a * time_vec), label = 'Best fit', color = "lightgray")
plt.errorbar(bin[hist != 0]*1e6, hist[hist != 0], xerr=x_error*1e6, yerr=delta_y, fmt = '.', label = "Simulation data")
plt.xlabel("t (us)")
plt.ylabel("dN/dt")
plt.legend()

plt.show()


