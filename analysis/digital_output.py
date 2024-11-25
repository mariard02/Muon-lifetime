import numpy as np
import matplotlib.pyplot as plt
import pmt_photons

file = "../build/output/PMT.txt"

# Open the data
data = pmt_photons.PMTSplit(file)

time_output = np.array([])

for event in range(data.number_of_events):
    print(data.delay)

    coincidence_up = data.coincidence(event, 0, 1)
    coincidence_down = data.coincidence(event, 1, 2)

    start_time = min(coincidence_up)

    for element in coincidence_up:
        if abs(element - start_time) >= data.delay:
            time_output = np.append(time_output, element)

    for element in coincidence_down:
        if abs(element - start_time) >= data.delay:
            time_output = np.append(time_output, element)

print(time_output)