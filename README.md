# mc - Geant4 example


[![Ubuntu 22.04](https://github.com/mzks/mc/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/ubuntu.yml)
[![CVMFS](https://github.com/mzks/mc/actions/workflows/cvmfs.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/cvmfs.yml)

This is an example of Geant4 simulation and data processing for physics experiment.

## Features

- Prepared physics, materials, primary generators
- Output as ROOT TTree
- Human friendly logger
- Data pocessing examples
- Track git hash
- Basic tests

## Usage

## Requirement
- Geant4 11.0
- ROOT 6
- Qt (for interactive mode)

### Install & build
```
> git clone https://github.com/mzks/mc
> cd mc
> cmake -B build
> cmake --build build
> cmake --install build --prefix .
```
or, `./make.sh` on the `mc`

### Run MC

 - GUI interactive mode
```
> ./bin/mc -i
```

 - Batch mode
```
> ./bin/mc -m run.mac
```

### Run processing
```
> ./bin/mcProcessing -i mc.root -o processed.root --clone
```


You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.

