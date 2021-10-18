#!/usr/bin/env bash

. /opt/root/6.22.6/bin/thisroot.sh
. /opt/geant4/10.6.3/bin/geant4.sh

../build/mc $1
