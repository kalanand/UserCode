#!/bin/tcsh 
date 
source /uscmst1/prod/sw/cms/setup/cshrc prod 
cd /uscms/home/kalanand/cms/CMSSW_1_6_9/src/JetMETCorrections/ZJet/test/
eval `scramv1 runtime -csh` 
cmsRun ZJetAnalysis.cfg >&! /uscms_data/d1/kalanand/trash/logfile_ZJetAnalysis 
date 
