import numpy as np

class PMTSplit:
    def __init__(self, file):
        # Load data from the file
        self.time, self.detector, self.event = np.loadtxt(file, usecols=[0, 1, 2], skiprows=1, unpack=True)

        # Determine the number of events and detectors
        self.number_of_events = int(np.max(self.event)) + 1
        self.number_of_detectors = 4 #int(np.max(self.detector)) + 1

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
    
    def voltage(self):
        
        time_bins = np.zeros((self.number_of_events, self.number_of_detectors, len(self.time_vector)), dtype=int)
        for i in range(len(self.time)):
            event_idx = int(self.event[i])
            detector_idx = int(self.detector[i])
            time_idx = int(self.time[i]*1e-9 / self.dt)
            time_bins[event_idx, detector_idx, time_idx] += 1

        number_electrons_time = time_bins / self.dt * self.gain * self.quEff

        voltage_time = number_electrons_time * self.qe * self.resistance

        return voltage_time

    def PMT_filter(self, event, detector):
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
                    output = np.append(output, np.array([start_time, start_time + time_signal]))
                    time_signal = 0
        
        return output
    
    def coincidence(self, event, detector1, detector2):
        output = np.array([])
        sci0 = np.atleast_2d(self.PMT_filter(event, detector1))
        sci1 = np.atleast_2d(self.PMT_filter(event, detector2))

        # No coincidence if there is no signal in one of the detectors
        if sci0.size == 0 or sci1.size == 0:       
            return output

        start_0 = np.atleast_1d(sci0[:, 0]) # column of start times of the first detector
        start_1 = np.atleast_1d(sci1[:, 0]) # column of start times of the first detector

        end_0 = np.atleast_1d(sci0[:, 1]) # column of end times of the first detector
        end_1 = np.atleast_1d(sci1[:, 1]) # column of end times of the first detector

        for count0, start_detection0 in enumerate(start_0):
            for count1, start_detection1 in enumerate(start_1):
                if abs(start_detection0 - start_detection1) < self.discriminator_width_min:
                    
                    end_detection0 = end_0[count0]
                    end_detection1 = end_1[count1]

                    start_coincidence = max(start_detection0, start_detection1)
                    end_coincidence = min(end_detection0, end_detection1)

                    len_coincidence = end_coincidence - start_coincidence

                    if len_coincidence > self.overlap:
                        output = np.append(output, start_coincidence)
        return output
