import numpy as np
import matplotlib.pyplot as plt

file = "../build/output/decay_time.txt"

data = np.loadtxt(file)

hist, bin = np.histogram(data)

bin = bin[:-1]

a, b = np.polyfit(bin[hist != 0], np.log(hist[hist != 0]), 1)

mean_time = - 1 / a
print(f'Muon lifetime = {mean_time} s')

total_time = bin[-1]

time_vec = np.arange(0, total_time, 1e-8)

plt.figure()
plt.plot(time_vec, np.exp(b) * np.exp(a * time_vec))
plt.scatter(bin, hist)
plt.show()