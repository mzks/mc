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

### Requirement
- Geant4 11.0
- ROOT 6
- Qt (for interactive mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.

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
-h --help               	shows help message and exits [default: false]
-v --version            	prints version information and exits [default: false]
-m --macro              	set macro filename for batch mode [default: "run.mac"]
-o --output             	output file name with .root [default: "mc.root"]
-i --interactive        	run as interactive mode, not batch [default: false]
-s --seed               	run as interactive mode, not batch [default: 0]
-p --path-to-macro      	set macro search path with colon-separated list [default: ".:bench:../bench"]
-a --ascii              	Output with ascii file, not root [default: false]
-k --keep-history       	Make a file to keep command list [default: false]
-r --root-include-macro 	Store command list to root file [default: false]
```
With `-k` option, the used macro list will be generated as like `mc_macro.txt`.
This is a macro which can be used as a input, thus you can run again with the file with `./bin/mc -m mc_macro.txt`.

The `bench/run.mac` is an example of the input macro for the mc.
As a primary particle generator, three options are provided with `/mcgun/selectGunAction`
The first option (pg) is a mcParticleGun. 
This is a G4ParticleGun with some user customed command.
If you want to custom your primary particle, it is a good option when you generates 1 particle per each event.
The second option is a G4GeneralParticleSource (gps).
The third option is fully-user customed primary particle generator (custom).
The primary particle is defined at mcPrimaryGeneratorAction.


### Run processing
```
> ./bin/mcProcessing -i mc.root -o processed.root --clone
```



