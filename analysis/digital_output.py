import numpy as np
import matplotlib.pyplot as plt
import pmt_photons

file = "../build/output/PMT_al_200.txt"

output_file = "../build/output/decay_time.txt"

# Open the data
data = pmt_photons.PMTAnalysis(file)

time_output = []

for event in range(data.number_of_events):
    coincidence_up = data.coincidence(event, 0, 1)
    coincidence_down = data.coincidence(event, 2, 3)

    if coincidence_up.size > 0:
        start_time = coincidence_up.min()
        valid_up = coincidence_up[coincidence_up - start_time >= data.delay]
        valid_down = coincidence_down[coincidence_down - start_time >= data.delay]

        time_output.extend(valid_up - start_time)
        time_output.extend(valid_down - start_time)

np.savetxt(output_file, time_output, delimiter=' ')
