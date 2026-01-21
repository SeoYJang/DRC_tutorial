# DRC generic
Repository for tutorial of GEANT4 simulation &amp; analysis of the dual-readout calorimeter.

## How-to
### Fetching
On your terminal (in Alma9 Linux environment // VMWare),

    git clone -b https://github.com/SeoYJang/DRC_tutorial

### Compile
After fetching the repository, do

    cd DRC_tutorial
    cd build
    ccmake ..
        # configure(c), and turn WITH_GEANT4_UIVIS to ON (enter)
        # configure(c), generate(g)
    make -j4

when you update your code after initial compile, only run make again in build folder

    make -j4

### Visualization
After compiling, inside /build folder, run

    cd DRsim
    ./DRsim

### Run
To generate an event (execute a run),

    ./DRsim <macro_file> <seed> <outputfile_name>

e.g.)

    ./DRsim run_ele.mac 1 output

### Analysis
To analyze output root file, inside /build folder, run

    cd analysis
    ./analysis <path_to_root_files> <low_edge_of_hist> <truth_E> <high_edge> <outputfile_name>

e.g.)

    ./bin/analysis /home/USER/20GeV_ele_data 0 20 25 ./20GeV_ele
