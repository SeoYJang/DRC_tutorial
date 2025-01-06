#run the bellow command

#for i in "e-" "pi-" "pi0" "proton"
for i in "e-"
# particle names(e-, pi-)
do
    run_beamOn="1"

    tower_material="3by3"


    for j in 20
    # for j in 5 10 20 30 50 70 90 110
    # energy with the unit of GeV.
    do
        #------------------------------------------------------------#
        # events number = beamon*queue. so change "/run/beamOn 10" when you simulate different events number.
        #------------------------------------------------------------#

        macroname="${tower_material}_${i:0:-1}_${j}"
                
        gun_particle=$i
        gun_energy="$j GeV"
        root_name="${macroname}"

        ########################################################
        results="/path/to/your/result/directory/$root_name/"
        ########################################################

        echo "starting submit for $gun_energy $gun_particle and the output will be $root_name.root"   
        
        # ctrl+shift+L >> select same words    

        # Length from beam generator to tower front : 1500 mm
        # x = 1500*tan(theta)
        # 0.0 : 0 || 0.5 : -1.309 || 1.0 : -2.618 || 1.5 : -3.928 || 2.0 : -5.238 || 2.5 : -6.549 || 3.0 : -7.861

        # ---------------------------
        # for Center of Module (theta 0)
        # theta : 0
        # phi   : 1.0
        # x(cm) : 0
        # y(cm) : 2.618
        # ---------------------------
        # ---------------------------
        # for Center of Module (theta 1.5)
        # theta : 1.5
        # phi   : 1.0
        # x(cm) : -3.928
        # y(cm) : 2.618
        # ---------------------------

        echo "/DRsim/action/useHepMC False" >> 1run_$macroname.mac
        echo "/DRsim/action/useCalib True" >> 1run_$macroname.mac
        echo "/vis/disable" >> 1run_$macroname.mac
        echo "/run/numberOfThreads 1" >> 1run_$macroname.mac
        echo "/run/initialize" >>1run_$macroname.mac
        echo "/run/verbose 1" >> 1run_$macroname.mac
        echo "/DRsim/generator/randx 10" >> 1run_$macroname.mac
        echo "/DRsim/generator/randy 10" >> 1run_$macroname.mac
        echo "/DRsim/generator/theta 1.5" >> 1run_$macroname.mac
        echo "/DRsim/generator/phi 1.0" >> 1run_$macroname.mac
        echo "/DRsim/generator/x0 -3.928" >> 1run_$macroname.mac
        echo "/DRsim/generator/y0 2.618" >> 1run_$macroname.mac
        echo "/gun/particle $i" >> 1run_$macroname.mac
        echo "/gun/energy $j GeV" >> 1run_$macroname.mac
        echo "/run/beamOn $run_beamOn" >> 1run_$macroname.mac

        echo "#! /bin/sh" > 1run_$macroname.sh
        echo "export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.14.2/Linux-x86_64/bin:\$PATH" >> 1run_$macroname.sh
        echo "source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-centos7/setup.sh" >> 1run_$macroname.sh
        echo "source /cvmfs/sft.cern.ch/lcg/releases/LCG_96b/ROOT/6.18.04/x86_64-centos7-gcc8-opt/ROOT-env.sh" >> 1run_$macroname.sh
        echo "source /cvmfs/geant4.cern.ch/geant4/10.5.p01/x86_64-centos7-gcc8-opt-MT/CMake-setup.sh" >> 1run_$macroname.sh
        echo "export HEPMC_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/hepmc3/3.1.2/x86_64-centos7-gcc8-opt" >> 1run_$macroname.sh
        echo "export FASTJET_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/fastjet/3.3.2/x86_64-centos7-gcc8-opt" >> 1run_$macroname.sh
        echo "export PYTHIA_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/MCGenerators/pythia8/240/x86_64-centos7-gcc8-opt" >> 1run_$macroname.sh
        echo "export PYTHIA8=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/MCGenerators/pythia8/240/x86_64-centos7-gcc8-opt" >> 1run_$macroname.sh
        echo "export PYTHIA8DATA=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/MCGenerators/pythia8/240/x86_64-centos7-gcc8-opt/share/Pythia8/xmldoc" >> 1run_$macroname.sh
        echo "export ROOT_INCLUDE_PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_96b/hepmc3/3.1.2/x86_64-centos7-gcc8-opt/include:\$ROOT_INCLUDE_PATH" >> 1run_$macroname.sh
        echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$HEPMC_DIR/lib64:\$FASTJET_DIR/lib:\$PYTHIA_DIR/lib:\$PWD/lib" >> 1run_$macroname.sh
        echo "./bin/DRsim_${tower_material} 1run_$macroname.mac \$1 ${results}root/$root_name" >> 1run_$macroname.sh

        echo "universe = vanilla" > 1run_$macroname.sub
        echo "executable = 1run_$macroname.sh" >> 1run_$macroname.sub
        echo "arguments = \$(ProcId)" >> 1run_$macroname.sub
        echo "core_size = 0" >> 1run_$macroname.sub
        echo "output = $results/log/\$(ProcId).out" >> 1run_$macroname.sub
        echo "error = $results/log/\$(ProcId).err" >> 1run_$macroname.sub
        echo "log = $results/log/\$(ProcId).log" >> 1run_$macroname.sub
        echo "request_memory = 1.0 GB" >> 1run_$macroname.sub
        echo "should_transfer_files = YES" >> 1run_$macroname.sub
        echo "when_to_transfer_output = ON_EXIT" >> 1run_$macroname.sub
        echo "transfer_input_files =./bin, ./lib, ./init.mac, ./1run_$macroname.mac" >> 1run_$macroname.sub
        echo "queue 3000" >> 1run_$macroname.sub

        #cat 0_SimulationShFile.sh
        mkdir "$results"
        mkdir "$results/log"
        mkdir "$results/root"
        condor_submit 1run_$macroname.sub

        echo "Simulation Uploaded"
        echo "$i $j GeV $tower_material"

        cp 1run_$macroname.* $results
    done
    
done

