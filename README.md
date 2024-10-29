# eVTOL Simulator

# Please refer to the document [Requirements Document: eVtol Simulator](https://docs.google.com/document/d/1PrDmock3PGhfQWBqlNfJYndWmNOpXSLpgaw_mi48pYE/edit?usp=sharing) for detailed write-up on this project


![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)


---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contact](#contact)

---

## About

eVTOL Simulator simulates a random number of each given type of vehicle and provides various statistics once the simulation is complete.

## Features

- Error handling for input JSON
- Highly modular
- Doesn't need dependencies to be installed separately; json library is included in thirdparty folder
- Easy to read 

## Installation

Currently, precompiled binaries of this project are not hosted anywhere. Please compile from to generate executable binaries;
```bash
# Prepare to build from source:
mkdir build
# Compile without unit tests
cd build
cmake ..
# To instead compile with unit tests, pass the BUILD_TESTING flag as below 
cmake -DBUILD_TESTING=ON  ..
make
```
This generates `evtol_sim_exe` (and `evtol_sim_tests` if compiled with tests)

## Usage

In a terminal, run
```bash
evtol_sim_exe <PATH_TO_JSON>
```
to run a simulation session for 3 hours of simulation time with 20 vehicles deployed. A json file which stores the vehicle types defined in the problem statement is provided in the `res` folder and can be used directly as follows:
```bash
# Assuming current directory is <project_src>/build
./evtol_sim_exe ../res/vehicle_characteristics.json
``` 

Example Output:
```bash
Echo Company
Average Time per flight (hrs): 0.862222
Average Distance traveled per flight (miles): 25.8664
Average Time charging per charge session (hrs): 2.1375
Total faults: 2
Passenger miles: 620.793

---------------------
Delta Company
Average Time per flight (hrs): 1.66694
Average Distance traveled per flight (miles): 150.025
Average Time charging per charge session (hrs): 0
Total faults: 4
Passenger miles: 1200.2

---------------------
Charlie Company
Average Time per flight (hrs): 0.625
Average Distance traveled per flight (miles): 99.998
Average Time charging per charge session (hrs): 2.37472
Total faults: 2
Passenger miles: 299.997

---------------------
Bravo Company
Average Time per flight (hrs): 0.666944
Average Distance traveled per flight (miles): 66.6959
Average Time charging per charge session (hrs): 2.33278
Total faults: 2
Passenger miles: 333.466

---------------------
Alpha Company
Average Time per flight (hrs): 1.66667
Average Distance traveled per flight (miles): 200.014
Average Time charging per charge session (hrs): 0
Total faults: 2
Passenger miles: 1600.11

---------------------
```
To run the unit tests after compiling with -DBUILD_TESTING=ON flag, run the tests executable directly:
```bash
<proj_src>/build/evtol_sim_tests
```

## Contact
npd15393@gmail.com
