#!/usr/bin/env bash

. ~/local/root/6.26.0/bin/thisroot.sh
. ~/local/geant4/11.0.1/bin/geant4.sh

../build/mc $1
