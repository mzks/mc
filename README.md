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
GUI mode
```
>../bin/mc

```

Batch mode
```
>../bin/mc bat.mac

```
