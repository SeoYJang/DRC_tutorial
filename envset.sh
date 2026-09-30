#!/bin/sh

#export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.14.2/Linux-x86_64/bin:$PATH
#source /cvmfs/sft.cern.ch/lcg/contrib/gcc/11/x86_64-el9/setup.sh
#
#source /cvmfs/sft.cern.ch/lcg/releases/LCG_107/ROOT/6.34.02/x86_64-el9-gcc11-opt/ROOT-env.sh
##source /cvmfs/sft.cern.ch/lcg/releases/LCG_104/clhep/2.4.6.4/x86_64-el9-gcc11-opt/clhep-env.sh
#source /cvmfs/sft.cern.ch/lcg/releases/LCG_107/clhep/2.4.7.1/x86_64-el9-gcc11-opt/clhep-env.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_107/x86_64-el9-gcc11-opt/setup.sh
#source /cvmfs/geant4.cern.ch/geant4/11.3.p02/x86_64-el9-gcc11-optdeb-MT/CMake-setup.sh
source /cvmfs/geant4.cern.ch/geant4/11.4/x86_64-el9-gcc11-optdeb-MT/CMake-setup.sh

SIPM_INSTALL=/u/user/syjang/DRC_tutorial/SimSiPM/install
export LD_LIBRARY_PATH=$SIPM_INSTALL/lib64:$LD_LIBRARY_PATH
export CPATH=$SIPM_INSTALL/include:$CPATH
export CMAKE_PREFIX_PATH=$SIPM_INSTALL:${CMAKE_PREFIX_PATH:-}

export TERM=xterm
