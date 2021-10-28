![Build Status](https://github.com/PfizerRD/scikit-digital-health/workflows/skdh/badge.svg)

# SciKit-Digital-Health
Python package with methods for reading, pre-processing, manipulating, and analyzing Inertial Meausurement Unit data. `scikit-digital-health` contains the following sub-modules:

### sit2stand
The `sit2stand` sub-module uses novel algorithms to first detect Sit-to-Stand transitions from lumbar-mounted accelerometer data, and then provide quantitative metrics assessing the performance of the transitions. As gyroscopes impose a significant detriment to battery life due to power consumption, `sit2stand`'s use of acceleration only allows for a single sensor to collect days worth of analyzable data.

### gait
The `gait` sub-module contains python implementations of existing algorithms from previous literature, as well as a gait classifier for detecting bouts of gait during at-home recordings of data. `gait`'s algorithms detect gait and analyze the gait for metrics such as stride time, stride length, etc from lumbar acceleration data. Adding gyroscope data will also enable several additional features.

### read
The `read` sub-module contains methods for reading data from several different sensor binary files into python. These have been implemented in low level languages (C, Fortran) to enable significant speed-ups in loading this data over pure-python implementations, or reading csv files

### features
The `features` sub-module contains a library of features that can be generated for accelerometer/gyroscope/time-series signals. These features can be used for classification problems, etc. Feature computations are also implemented in Fortran to ensure quicker computation time when dealing with very long (multiple days) recordings.

## Documentation
Full documentation is available [HERE INSERT LINK]()

## Requirements
- Python >= 3.6
- numpy >= 1.17.2
- scipy >= 1.3.1
- pandas >= 0.23.4
- lightgbm >= 2.3.0
- pywavelets

### Library Requirements
SKDH also has some compiled library requirements:

- libzip (if installing outside conda probably labeled as libzip-dev)
- gsl (if not installing with conda probably labeled as libgsl##-dev)

## Installation
```shell script
pip install git+ssh://git@github.com/PfizerRD/scikit-digital-health.git
```

If you want to install from a different branch (ie development):

```shell script
pip install git+ssh://git@github.com/PfizerRD/scikit-digital-health@development
```

## Testing
Testing has additional requirements:
- pytest
- h5py

All tests are automatically run each time a pull request is issued into the master branch. If for some reason you want to run the tests locally, first clone the repository (tests are no longer distributed with the package itself, saving on install/download size). Once the repository has been downloaded, make sure `scikit-digital-health` is installed, navigate to the local directory of `scikit-digital-health`, and simply run `pytest`:

```shell script
# clone the repository
git clone ssh+git@github.com:PfizerRD/scikit-digital-health.git
# move into the scikit-digital-health directory
cd scikit-digital-health
# make sure scikit-digital-health is installed - this is necessary if you haven't already installed it through pip (see above)
pip install .
# run tests with higher verbosity
pytest -v
```

## Usage
TODO

## Contributing
Please read the [CONTRIBUTING](CONTRIBUTING.md) document for guidelines for making contributions.
