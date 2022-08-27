# mc - Geant4 example


![GitHub commit activity](https://img.shields.io/github/commit-activity/m/mzks/mc)
![GitHub last commit](https://img.shields.io/github/last-commit/mzks/mc)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/mzks/mc)
![GitHub pull requests](https://img.shields.io/github/issues-pr-raw/mzks/mc)
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
- ROOT 6 with C++17 option
- Qt (for interactive mode)

You can run easily, without installing ROOT and Geant4 using [virtual machine](https://wiki.kek.jp/display/geant4/Geant4+Virtual+Machine).
When you use this virtual machine, you have to do `source $ROOTSYS/bin/thisroot.sh` before running.

### Install & build
First of all, clone this repository,
```
> git clone https://github.com/mzks/mc
> cd mc
```
Then, execute  `./make.sh` or step-by-step build.
```
> cmake -B build
> cmake --build build
> cmake --install build --prefix .
```
The `./make.sh` script will execute build, install, and full test automatically.


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

### Output file format
The generated file with `mc` includes parameters and a TTree named mc.

Included parameters are

| Parameter name      | Example                                                       | Description                                                                                         |
|---------------------|---------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| `git_sha1`            | 46f7d136b2e52ff0dc52ba25f871ea753730a03d-dirty                | The source git SHA1 hash in build phase. If the source is modified, the hash has a suffix, `-dirty` |
| `git_date`            | Mon Aug 22 18:14:29 2022                                      | The source git date                                                                                 |
| `git_subject`         | Add tests for reconstruction                                  | The source git subject                                                                              |
| `G4Version`           | Geant4 version Name: geant4-11-00-patch-02 [MT],(25-May-2022) | Used Geant4 version                                                                                 |
| `ROOTVersion`         | 6.26/04                                                       | Used ROOT version                                                                                   |
| `physicslist_name`    | `FTFP_BERT_HP`                                                | Base physicslist name                                                                               |
| `seed`                | 1 (`TParameter<int>`)                                           | Simulation seed                                                                                     |
| `gaps_custom_physics` | True (`TParameter<bool>`)                              | Enable user (GAPS) custom physics                                                                   |
| `commands`            | /control/execute run.mac /control/verbose 1 etc.              | Applied UI command list in used Geant4 macro                                                        |

and the TTree branches are

| Branch name   | Type             | Description                                               |
|---------------|------------------|-----------------------------------------------------------|
| `nHit`        | `UInt_t`         | Number of particles detected with any "SensitiveDetector", should be match with the other `std::vector::size()`.|
| `x`           | `vector<double>` | X of touched position [mm].                               |
| `y`           | `vector<double>` | Y of touched position [mm].                               |
| `z`           | `vector<double>` | Z of touched position [mm].                               |
| `time`        | `vector<double>` | Time that particles touch the "SensitiveDetector" [ns]    |
| `eIn`         | `vector<double>` | Energy incoming particles into "SensitiveDetector" [MeV]  |
| `eDep`        | `vector<double>` | Energy deposition in "SensitiveDetector" [MeV]            |
| `TrackID`     | `vector<int>`    | Track ID of Geant4 (Initial particle starts at 1).        |
| `copyNo`      | `vector<int>`    | Copy No (user tag for physical volume in Geant4)          |
| `particle`    | `vector<int>`    | Incoming particle ID with PDG code.                       |



### Run processing
```
> ./bin/mcProcessing -i mc.root -o processed.root
> ./bin/response -i mc.root -o response.root
```
The processing (with legacy TTree reader) and response (with RDataFrame) scripts are just skeltons.
The meaningful branches should be implemented.



