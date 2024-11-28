import numpy as np

class PMTAnalysis:
    def __init__(self, file):
        # Load data from the file
        self.time, self.detector, self.event = np.loadtxt(file, usecols=[0, 1, 2], skiprows=1, unpack=True)

        # Determine the number of events and detectors
        self.number_of_events = int(np.max(self.event)) + 1
        self.number_of_detectors = 4 #int(np.max(self.detector)) + 1
        self.number_of_measurements = len(self.time) + 1

        self.event_list = set(self.event)

        self.quEff = 0.25  # Quantum efficiency (optional)
        self.gain = 1e5  # PMT gain
        self.qe = 1.60217663e-19  # Charge of an electron
        self.dt = 0.1e-9  # Time interval in ns
        self.total_time = self.time.max()
        self.resistance = 50 # Ohms

        self.overlap = 5e-9
        self.discriminator_threshold = 50e-3 # minimum voltage IN VOLTS
        self.discriminator_width_min = 20e-9 # minumum time that we need a signal to occur

        self.delay = 100e-9 # Dead time after the start during which decays cannot be detected

        self.time_vector = np.arange(0, (self.total_time + self.dt)*1e-9, self.dt)

    def __str__(self):
        # Return summary information about events and detectors
        output = f'NUMBER OF EVENTS = {self.number_of_events}\n\n'
        output += f'NUMBER OF SCINTILLATORS = {self.number_of_detectors}\n'
        return output

    def split(self):
        """
        Returns an array that stores in each row the detector number and the time of detection of each photon
        """
        # Create a list of lists for storing times per event and detector
        time_split = [[[] for _ in range(self.number_of_detectors)] for _ in range(self.number_of_events)]
        
        # Populate time_split with times for each event and detector
        for i in range(len(self.time)):
            event_idx = int(self.event[i])
            detector_idx = int(self.detector[i])
            time_split[event_idx][detector_idx].append(self.time[i])
        
        return np.array(time_split)

    def event_detector(self, event_choice, scintillator_choice):
        # Access times for a specific event and detector
        return np.array(self.split()[event_choice][scintillator_choice])
    
    def voltage(self):
        """
        Returns the voltage emitted by the PMT at each time for every detector. Counts the number of photons 
        received in 1 ns and uses the quantum efficiency to decide how many are detected. Then, using 
        proportionality factors, the voltage is given.
        """
        
        time_bins = np.zeros((self.number_of_events, self.number_of_detectors, len(self.time_vector)), dtype=int)
        for i in range(self.number_of_measurements - 1): # Go through all the measurements
            event_idx = int(self.event[i])
            detector_idx = int(self.detector[i])
            time_idx = int(self.time[i]*1e-9 / self.dt)
            time_bins[event_idx, detector_idx, time_idx] += 1 # Do a histogram

        number_electrons_time = time_bins / self.dt * self.gain * self.quEff

        voltage_time = number_electrons_time * self.qe * self.resistance

        return voltage_time

    def PMT_filter(self, event, detector):
        """
        Returns the start and end time of the signals that are above the threshold
        """
        voltage_time = self.voltage()
        output = np.array([])

        filtered_signal = np.array((voltage_time[event, detector, :] - self.discriminator_threshold) > 0 )
        time_signal = 0
        start_time = 0

        for index, i in enumerate(filtered_signal):
            if i == True:
                time_signal += self.dt
                if filtered_signal[index - 1] == False:
                    start_time = index * self.dt
            else:
                if (filtered_signal[index - 1] == True and time_signal >= self.discriminator_width_min ):
                    # We have to add the self.discrimination_width_min because it is the length of the signal by the discriminator
                    output = np.array(output.tolist() + [[start_time, start_time + time_signal + self.discriminator_width_min]])

                    time_signal = 0
        
        return output
    
    def coincidence(self, event, detector1, detector2):
        """
        Given two detectors, filters the signal to obtain the start and end time of signals and checks if there
        are any coincidences: the start of a signal in one detector must fall within the duration of a signal in the
        other detector, and the overlap must be longer than self.overlap.
        """

        sci0 = np.atleast_2d(self.PMT_filter(event, detector1))
        sci1 = np.atleast_2d(self.PMT_filter(event, detector2))

        if sci0.size == 0 or sci1.size == 0:
            return np.array([])  # No signals in one or both detectors.

        start_0, end_0 = sci0[:, 0], sci0[:, 1]
        start_1, end_1 = sci1[:, 0], sci1[:, 1]

        # Check for overlaps where start_0 is within the interval [start_1, end_1]
        cond_0_within_1 = (start_0[:, np.newaxis] >= start_1) & (start_0[:, np.newaxis] <= end_1)
        cond_1_within_0 = (start_1 >= start_0[:, np.newaxis]) & (start_1 <= end_0[:, np.newaxis])

        # Combine conditions to find valid overlaps
        valid_pairs = cond_0_within_1 | cond_1_within_0

        if not valid_pairs.any():
            return np.array([])  # No coincidence

        idx_0, idx_1 = np.where(valid_pairs)

        # Calculate overlap intervals
        start_coincidence = np.maximum(start_0[idx_0], start_1[idx_1])
        end_coincidence = np.minimum(end_0[idx_0], end_1[idx_1])

        # Calculate the length of the coincidence
        len_coincidence = end_coincidence - start_coincidence

        # Filter for coincidences with sufficient overlap
        valid_coincidences = len_coincidence > self.overlap

        return start_coincidence[valid_coincidences]