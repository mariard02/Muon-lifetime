import numpy as np

class PMTSplit:
    def __init__(self, file):
        # Load data from the file
        self.time, self.detector, self.event = np.loadtxt(file, usecols=[0, 1, 2], skiprows=1, unpack=True)

        # Determine the number of events and detectors
        self.number_of_events = int(np.max(self.event)) + 1
        self.number_of_detectors = int(np.max(self.detector)) + 1

    def __str__(self):
        # Return summary information about events and detectors
        output = f'NUMBER OF EVENTS = {self.number_of_events}\n\n'
        output += f'NUMBER OF SCINTILLATORS = {self.number_of_detectors}\n'
        return output

    def split(self):
        # Create a list of lists for storing times per event and detector
        time_split = [[[] for _ in range(self.number_of_detectors)] for _ in range(self.number_of_events)]
        
        # Populate time_split with times for each event and detector
        for i in range(len(self.time)):
            event_idx = int(self.event[i])
            detector_idx = int(self.detector[i])
            time_split[event_idx][detector_idx].append(self.time[i])
        
        return time_split

    def event_detector(self, event_choice, scintillator_choice):
        # Access times for a specific event and detector
        return np.array(self.split()[event_choice][scintillator_choice])
    
    def discriminator(self, event_choice, scintillator_choice):
        detector = self.event_detector(event_choice, scintillator_choice) # data from a certain pmt and event

        quEff = 0.25  # Quantum efficiency (optional)
        gain = 1e5  # PMT gain
        qe = 1.60217663e-19  # Charge of an electron
        dt = 0.1e-9  # Time interval in ns
        total_time = self.time.max()