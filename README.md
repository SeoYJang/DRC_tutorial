# DRC generic
Repository for tutorial of GEANT4 simulation &amp; analysis of the dual-readout calorimeter.

## How-to
### Compile
After fetching the repository, do
    
    cd install
    source envset.sh
    cd ../build
    cmake -DCMAKE_INSTALL_PREFIX=<path_to_install_directory> ..
    make -j4 install

For visualization, do

    cd build
    ccmake ..
    WITH_GEANT4_UIVIS -> ON
    configure & generate

### Analysis

    ./bin/analysis <path_to_root_files> <low_edge_of_hist> <truth_E> <high_edge> <outputfile_name>

e.g.)

    ./bin/analysis /home/USER/20GeV_ele_data 0 20 25 ./20GeV_ele
