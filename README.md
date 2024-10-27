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

Describe the purpose of the project. What problem does it solve? Provide some background and motivation here.

## Features

- List the key features of the project.
- Explain any unique functionality or highlight how the project stands out.
- Bullet-point format works well here.

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
to run a simulation session for 3 hours of simulation time with 20 vehicles deployed.

## Contact
npd15393@gmail.com
