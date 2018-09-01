# mc - Geant4 example

This is an example of Geant4 simulation for experiment.

## Features

- Prepare materials
- Output for ROOT tree

## Usage
### Install
```
$git clone https://github.com/mzks/mc
```
for Positronium experiment, `$git checkout ps`.

### Run
```
>mkdir build
>cd build
>cmake ../source && make install
>cd ../bench
```
If you don't use macro as an argument, GUI and a prompt are opened.

GUI mode
```
>../bin/mc
```

Batch mode
```
>../bin/mc bat.mac
```

## Requirement
- Geant4.10.4
- ROOT6
- Qt (GUI mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `>source $ROOTSYS/bin/thisroot.sh` before running.


