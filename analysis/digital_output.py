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
def PMT_filter(event, detector, voltage_time):
    output = np.array([])
    #print(f'DETECTOR {detector}')

    filtered_signal = np.array((voltage_time[event, detector, :] - discriminator_threshold) > 0 )
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
                output = np.append(output, np.array([start_time, start_time + time_signal]))
                #print(f'start = {start_time}')
                #print(f'end = {start_time + time_signal}')
                #print(f'duration = {time_signal} seconds')
                time_signal = 0
    
    return output

sci0 = PMT_filter(0, 0, voltage_time)
sci1 = PMT_filter(0, 1, voltage_time)
#PMT_filter(0, 2, voltage_time)
#PMT_filter(0, 3, voltage_time)

print(sci0)
print(sci1)

overlap = 5e-9 # Minimum duration of the overlap to be detected

def coincidence(event, detector1, detector2, voltage_time):
    sci0 = np.atleast_2d(PMT_filter(event, detector1, voltage_time))
    sci1 = np.atleast_2d(PMT_filter(event, detector2, voltage_time))

    start_0 = np.atleast_1d(sci0[:, 0]) # column of start times of the first detector
    start_1 = np.atleast_1d(sci1[:, 0]) # column of start times of the first detector

    end_0 = np.atleast_1d(sci0[:, 1]) # column of end times of the first detector
    end_1 = np.atleast_1d(sci1[:, 1]) # column of end times of the first detector

    for count0, start_detection0 in enumerate(start_0):
        for count1, start_detection1 in enumerate(start_1):
            if abs(start_detection0 - start_detection1) < 20e-9:
                
                end_detection0 = end_0[count0]
                end_detection1 = end_1[count1]

                start_coincidence = max(start_detection0, start_detection1)
                end_coincidence = min(end_detection0, end_detection1)

                len_coincidence = end_coincidence - start_coincidence

                if len_coincidence > overlap:
                    print(f'COINCIDENCE at time = {start_coincidence} s')

coincidence(0, 0, 1, voltage_time)

plt.show()
