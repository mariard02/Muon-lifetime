# Muon_lifetime

This project simulates an experiment to determine the lifetime of the muon. The repository includes modules for both simulation and data processing.

The simulated setup consists in two scintillators (with optical guides and photomultipliers for readout), an absorber and another two scintillators. When a muon originated from cosmic rays passes through the first two scintillators, there is a certain probability that it will be stopped, and it will then decay in one electron and two neutrinos. The aim of the simulation is to observe this electron from the decay in a pair of scintillators and, by measuring the time since the muon was stopped, to obtain the lifetime of muons.

# Requirements
## Simulation
* Geant4: The simulation is developed in Geant4, which must be installed and properly configured to run this project. It can be downloaded from [here](https://geant4.web.cern.ch/download/11.3.0.html).
* CMake: Required to build the project. Install from CMake's official website.
* GCC/Clang: A C++ compiler (GCC or Clang) should be installed.
## Data Processing
* Python 3: Used for data processing and analysis.
* NumPy: The library is required for numerical computations.

To install Python dependencies for data analysis, run:
```
pip install -r requirements.txt
```

# Running the simulation
1. **Clone the repository**
```
git clone https://github.com/mariard02/Muon_lifetime
cd Muon_lifetime
```
2. **Create a build directory**
```
mkdir build
cd build
```
3. **Create an output folder** 
Inside the build directory:
```
mkdir output
```
4. **Run CMake to run the project**
```
cmake ..
```
5. **make the project**
```
make
```
6. **Run the simulation**
Run the following command in the build directory:
```
./SincilatorSim <number_of_events> <visualization>
```
Replace <number_of_events> with the desired number of events, and <visualization> with 1 if you want to see a visualization of the events. If visualization is not needed, leave this blank or enter any other value.

**REMARK:** Turning on event visualization is heavily discouraged, particularly for more than one event per run, as it consumes significant memory and noticeably increases runtime.

## Simulation output
The simulation registers the times at which photons reach the PMTs and stores the data in the file PMT.txt, located in the output folder inside the build directory.

**IMPORTANT:** The file PMT.txt is overwritten every time the simulation is run. If you want to save the data for future reference, please rename the file or move it to a different directory.

The file has three columns:
* Time: stores the time (in nanoseconds) at which a photon enters a PMT.
* Detector ID: stores the ID of the PMT detecting the photon. It ranges from 0 to 3, with 0 and 1 representing the detectors above the absorber, and 2 and 3 representing the detectors below.
* Event ID: indicates in which event of the run the photon hit the PMT.

# Analysing the results
To analyze the data, navigate to the analysis directory:
```
cd ../analysis
```
The key component for processing results is the PMTAnalysis class, whose source code is available in the pmt_photons.py file. The analysis proceeds as follows:
1. Reformats the data to obtain the time evolution of the voltage signal from each PMT.
2. Simulates the behavior of the discriminator in the physical setup by registering a signal as 1 if it is above a certain threshold, and 0 otherwise.
3. Searches for coincidences between signals from two PMTs.

For determining the lifetime of the muon, the analysis requires calculating the time elapsed from when the muon passes through the first two scintillators and stops at the absorber until the decay product (electron) is observed. To achieve this:
* Discard events where there is no delay between the observation of a signal in scintillators 0 and 1, and scintillators 2 and 3, or where no signal is observed after the muon passes through the initial scintillators.
* Once irrelevant events are discarded, calculate the time elapsed between coincidences in the two pairs of scintillators.

This can be done by running the digital_output.py file, that makes extensive use of the class metioned above:
```
python3 digital_output.py
```
This script generates a decay_time.txt file in the output folder. The file contains one column that stores the time (in nanoseconds) between the muon entering the setup and the detection of its decay products.

To obtain the muon lifetime from this, run the time_fit.py script:

```
python3 time_fit.py
```

This will fit the data in decay_time.txt to the expected distribution and calculates the error from the fit. The final calculated lifetime, as long as its uncertainty, is shown in the terminal window. A graphical representation of the data distribution is shown in a new window, and this is also saved as muon_histogram.png in the analysis folder.