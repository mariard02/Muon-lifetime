import numpy as np
import matplotlib.pyplot as plt

file_path = "/Users/maria/Desktop/Máster/M1/S1/Labo IV/Muon_lifetime/analysis/2024-12-16_filtered.txt"
data = np.loadtxt(file_path, skiprows=1, usecols=(3,))
counts = np.loadtxt(file_path, skiprows=1, usecols=(4,))

frec = 50e6
time_unit = 1/frec*1e6
print(time_unit)
time, bins=  np.histogram(data * time_unit, int(np.sqrt(len(data))) + 1)
bins = bins[:-1]

x_error = 20e-9
x = bins[(time != 0) & (bins < 40.)]
delta_x = x_error * np.ones_like(x)

y = np.log(time[(time != 0) & (bins < 40.)])
delta_y = 1. / np.sqrt(time[(time != 0) & (bins < 40.)])

#a, b = np.polyfit(bins[(time != 0) & (bins < 10.)], np.log(time[(time != 0) & (bins < 10.) ]), 1)

weights = 1 / delta_y**2  

# Ajuste lineal ponderado en escala logarítmica
p, cov = np.polyfit(x, y, 1, w=weights, cov=True)
a, b = p  # Parámetros del ajuste

mean_time = - 1 / a

time_vec = np.arange(0, 40, 1e-2)

print(f'Muon lifetime = {mean_time} us')

# ----------------------------------------------------------------------------------------------------
# ERROR ANALYSIS
# ----------------------------------------------------------------------------------------------------

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

print(f"Error = {mean_time_error} us")

plt.rcParams.update({
    'font.family': 'serif',
})

plt.figure(figsize=(10, 6))
plt.plot(time_vec, np.exp(b) * np.exp(a * time_vec), label = 'Best fit', color = "lightgray")
plt.errorbar(bins[time != 0], time[time != 0], xerr=x_error, yerr=np.sqrt(time[time != 0]), fmt = '.', label = "Experimental data")
plt.xlabel('Time (us)')
plt.ylabel('dN/dt')
plt.legend()
#plt.xlim(right = 30)
plt.show()