# mc - Geant4 example


[![Ubuntu 22.04](https://github.com/mzks/mc/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/ubuntu.yml)
[![Mac with M1](https://github.com/mzks/mc/actions/workflows/macos.yml/badge.svg)](https://github.com/mzks/mc/actions/workflows/macos.yml)

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
Command line options are 
```
-h --help          	shows help message and exits [default: false]
-v --version       	prints version information and exits [default: false]
-m --macro         	set macro filename for batch mode [default: "run.mac"]
-o --output        	output file name with .root [default: "mc.root"]
-i --interactive   	run as interactive mode, not batch [default: false]
-s --seed          	run as interactive mode, not batch [default: 0]
-p --path-to-macro 	set macro search path with colon-separated list [default: ".:bench"]
-a --ascii         	Output with ascii file, not root [default: false]
-k --keep-history  	Make a file to keep command list [default: false]
```
With `-k` option, the used macro list will be generated as like `mc_macro.txt`.
This is a perfect macro, thus you can run again with the file with `./bin/mc -m mc_macro.txt`.


### Run processing
```
> ./bin/mcProcessing -i mc.root -o processed.root --clone
```


You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.

