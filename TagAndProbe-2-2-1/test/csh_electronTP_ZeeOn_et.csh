#!/bin/tcsh 
date 
cd /uscms/home/kalanand/cms/electron/efficiency/CMSSW_2_2_1/src/PhysicsTools/TagAndProbe/test/ 
source /uscmst1/prod/sw/cms/setup/cshrc prod 
eval `scram runtime -csh` 
cmsRun py_electronTP_ZeeOn_et.py 
date 
