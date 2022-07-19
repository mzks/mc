# mc - Geant4 example


[![Test on Ubuntu](https://github.com/mzks/mc/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/ubuntu.yml)
[![Test on cvmfs](https://github.com/mzks/mc/actions/workflows/cvmfs.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/cvmfs.yml)

This is an example of Geant4 simulation for experiment.

## Features

- Prepare materials
- Output for ROOT tree

## Usage
### Install & build
```
> git clone https://github.com/mzks/mc
> cmake -S mc -B build
> cmake --build build
> cmake --install build --prefix .
```

 - GUI mode
```
> cd mc/bench
> ../../bin/mc
```

 - Batch mode
```
> cd mc/bench
> ../../bin/mc bat.mac
```

## Requirement
- Geant4.11
- ROOT6
- Qt (for GUI mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.


