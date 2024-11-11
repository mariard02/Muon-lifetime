import numpy as np
import matplotlib.pyplot as plt

class scintillator_split:
    def __init__(self, file):
        self.scintillator, self.counts, self.event = np.loadtxt(file, usecols=[0, 1, 2], skiprows=1, unpack=True)
        self.number_of_events = int(max(self.event)) + 1

    def __str__(self):
        output  = f'Number of events = {self.number_of_events}\n'

        output += f'Number of scintillators = {int(max(self.scintillator)) + 1}'

        return output

    def split_counts(self):
        counts_split = [ [] for _ in range(self.number_of_events) ]

        for i in range(len(self.scintillator)):
            counts_split[int(self.event[i])].append( [int(self.scintillator[i]), int(self.counts[i])]  )

        return counts_split
    
    def event_scintillator(self, num, scin):
        for i, j in self.split_counts()[num]:
            if i == scin:
                return j
        return 0

file = "../build/output/time.txt"

prueba = scintillator_split(file)

print(prueba)

print(prueba.event_scintillator(2, 2))