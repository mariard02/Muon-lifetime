import numpy as np
import matplotlib.pyplot as plt
import pmt_photons

file = "../build/output/PMT.txt"

# Open the data
data = pmt_photons.PMTSplit(file)
print(data)

# Set parameters
#event_choice = 0
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

discriminator_threshold = 50e-3 # minimum voltage IN VOLTS
discriminator_width_min = 20e-9 # minumum time that we need a signal to occur

plt.figure()
plt.xlabel("Time (s)")
plt.ylabel("Voltage (V)")
plt.axhline(discriminator_threshold, color = "red")
for event_choice in range(data.number_of_events):
    plt.plot(time_vector, voltage_time[event_choice, 0, :], label = f'{event_choice}')

plt.legend()


# Vector that is True when we have a signal above the threshold

# TO DO : ALL THE SCINTILLATORS AND EVENTS
filtered_signal = np.array((voltage_time[0, 0, :] - discriminator_threshold) > 0 )
time_signal = 0
start_time = 0

for index, i in enumerate(filtered_signal):
    if i == True:
        time_signal += dt
        if filtered_signal[index - 1] == False:
            start_time = index * dt
            #print(f'start = {start_time}')
    else:
        if (filtered_signal[index - 1] == True and time_signal >= discriminator_width_min ):
            print(f'start = {start_time}')
            print(f'end = {start_time + time_signal}')
            print(f'duration = {time_signal} seconds')
            time_signal = 0

plt.show()
