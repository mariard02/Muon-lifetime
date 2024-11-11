import numpy as np
import matplotlib.pyplot as plt
import pmt_photons

file = "../build/output/PMT.txt"

# Open the data
data = pmt_photons.PMTSplit(file)
print(data)

# Set parameters
event_choice = 6
scintillator_choice = 0
quEff = 0.25  # Quantum efficiency (optional)
gain = 1e5  # PMT gain
qe = 1.60217663e-19  # Charge of an electron
dt = 0.1e-9  # Time interval in ns
total_time = data.time.max()

# Create a time vector and a 3D array for time bins
time_vector = np.arange(0, (total_time + dt)*1e-9, dt)
time_bins = np.zeros((data.number_of_events, data.number_of_detectors, len(time_vector)), dtype=int)
resistance = 50 # Ohms

# Populate the time bins
for i in range(len(data.time)):
    event_idx = int(data.event[i])
    detector_idx = int(data.detector[i])
    time_idx = int(data.time[i]*1e-9 / dt)
    time_bins[event_idx, detector_idx, time_idx] += 1

number_electrons_time = time_bins / dt * gain * quEff

voltage_time = number_electrons_time * qe * resistance

discriminator_threshold = 50e-3
discriminator_width_min = 20e-9

plt.figure()
plt.plot(voltage_time[8, 3, :])
plt.show()

# Plot histogram of times for separated events
"""plt.figure()
plt.title("Time in each event")
plt.xlabel("Time (ns)")
plt.ylabel("Number of photons")
for i in range(data.number_of_detectors):
    plt.hist(data.split()[event_choice][i], bins=200, label=f'Scintillator {i}', alpha=0.7)

plt.legend()
plt.show()
"""