import numpy as np
import matplotlib.pyplot as plt

file = "../build/output/PMT.txt"

energy, time, event, photon_ID = np.loadtxt(file, usecols=[0, 1, 2, 3], skiprows=1, unpack=True)

number_of_events = int(max(event)) + 1

time_split = [[] for i in range(number_of_events) ]
energy_split = [[] for i in range(number_of_events) ]

# Separate the data from different events

for i in range(len(time)):
	time_split[int(event[i])].append(time[i])
	energy_split[int(event[i])].append(energy[i])


# Histogram of energy for separated events

plt.figure()
plt.title("Energy in each event")
plt.xlabel("Energy (eV)")
plt.ylabel("Number of photons")
for i in range(number_of_events):
	plt.hist(energy_split[i], 200)

# Histogram of time for separated events
plt.figure()
plt.title("Time in each event")
plt.xlabel("Time (ns)")
plt.ylabel("Number of photons")
for i in range(number_of_events):
	plt.hist(time_split[i], 200)

# Histogram of energy for all events
plt.figure()
plt.title("Energy in all events")
plt.xlabel("Energy (eV)")
plt.ylabel("Number of photons")
plt.hist(energy, 200)


# Histogram of time for all events
plt.figure()
plt.title("Time in all events")
plt.xlabel("Time (ns)")
plt.ylabel("Number of photons")
plt.hist(time, 200)

plt.show()