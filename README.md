# mc - Geant4 example

This is an example of Geant4 simulation for experiment.

## Features

- Prepare materials
- Output for ROOT tree

## Usage
### Install & build
```
> git clone https://github.com/mzks/mc
> cd mc
> cmake -S source -B build
> cmake --build build
> cmake --install build
```
Then, `bin` directory will be generated.


GUI mode
```
>cd bench
>../bin/mc
```

Batch mode
```
>cd bench
>../bin/mc bat.mac
```

## Requirement
- Geant4.10.6
- ROOT6
- Qt (GUI mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `>source $ROOTSYS/bin/thisroot.sh` before running.


