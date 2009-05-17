#!/bin/csh
source /uscmst1/prod/sw/cms/cshrc uaf
cd /uscms/home/kalanand/cms/electron/efficiency/CMSSW_2_2_1/src/PhysicsTools/TagAndProbe/test
eval `scram runtime -csh`
cmsRun py_Georgios_ntpl_summer08_1.py


