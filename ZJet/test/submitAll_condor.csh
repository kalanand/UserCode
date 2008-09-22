#!/bin/csh

echo "****** Submitting All PYTHIA bin jobs to condor queue ...."

./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_0_15.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_1000_1400.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_120_170.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_1400_1800.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_15_20.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_170_230.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_1800_2200.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_20_30.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_2200_2600.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_230_300.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_2600_3000.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_3000_3500.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_300_380.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_30_50.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_3500_-1.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_380_470.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_470_600.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_50_80.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_600_800.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_800_1000.cff -o /uscms_data/d1/kalanand/trash
./submitToCondor.py -c ZJetAnalysis.cfg -l CSA07-ZeeJets_Pt_80_120.cff -o /uscms_data/d1/kalanand/trash
