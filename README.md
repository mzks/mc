# mc - Geant4 example


[![Ubuntu 22.04](https://github.com/mzks/mc/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/ubuntu.yml)
[![CVMFS](https://github.com/mzks/mc/actions/workflows/cvmfs.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/cvmfs.yml)

This is an example of Geant4 simulation and data processing for physics experiment.

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
> ./bin/mc -p bench -i
```

 - Batch mode
```
> ./bin/mc -p bench -m run.mac
```

## Requirement
- Geant4.11
- ROOT6
- Qt (for interactive mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.

