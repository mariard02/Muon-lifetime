import numpy as np
import matplotlib.pyplot as plt
import pmt_photons

file = "../build/output/PMT.txt"

# Open the data
data = pmt_photons.PMTSplit(file)

data.coincidence(0, 0, 1)

plt.show()
