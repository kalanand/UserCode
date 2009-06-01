#! /usr/bin/env python
import os

#******************   definitions  **********************************
DIR_PATH =  "/uscms/home/kalanand/cms/electron/efficiency/CMSSW_2_2_8/src/PhysicsTools/TagAndProbe/test/"
EMAIL =              "kalanand@FNAL.GOV"
#*********************** creating csh files ************************
TagProbeType=[0,1,2,3]
TagProbeName=["_WenuOff_","_WenuOn_","_ZeeOff_", "_ZeeOn_"]
for sample in range(0,4):
     py_body = "py_electronTP"+TagProbeName[sample]+"et"
     py_name = DIR_PATH+py_body+ ".py"   
     file = open(py_name,'w')
     file.write("import FWCore.ParameterSet.Config as cms \n")
     file.write("process = cms.Process(\"Demo\") \n")
     file.write("process.load(\"FWCore.MessageService.MessageLogger_cfi\") \n")
     file.write("process.load(\"PhysicsTools.TagAndProbe.Electron_TagProbeEDMAnalysis_cfi\") \n")
     file.write("process.source = cms.Source(\"PoolSource\", \n")
     file.write("    fileNames = cms.untracked.vstring( \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_0.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_1.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_2.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_3.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_35.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_36.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_37.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_39.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_41.root', \n")
     file.write("'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_42.root' \n")     
     file.write(") \n")
     file.write(") \n")
     file.write("process.maxEvents = cms.untracked.PSet( \n")
     file.write("    input = cms.untracked.int32(-1) \n")
     file.write(") \n")
     file.write("process.p = cms.Path(process.demo) \n")
     file.write("process.MessageLogger.destinations = ['cout', 'cerr'] \n")
     file.write("process.MessageLogger.cerr.FwkReport.reportEvery = 1000 \n")
     tptype = "process.demo.TagProbeType = "+str(TagProbeType[sample])+" \n"
     file.write(tptype)          
     tpname = "process.demo.FitFileName = 'root_electroneff"+TagProbeName[sample]+"eta_et.root' \n"          
     file.write(tpname)
     file.close()     
     # ******************  creating the shell script  *********************     
     csh_body = "csh_electronTP"+TagProbeName[sample]+"et"
     csh_name = DIR_PATH+csh_body+".csh"
     file = open(csh_name,'w')
     file.write("#!/bin/tcsh \n")
     file.write("date \n")
     phrase = "cd "+DIR_PATH+" \n"
     file.write(phrase)
     file.write("source /uscmst1/prod/sw/cms/setup/cshrc prod \n")     
     file.write("eval `scram runtime -csh` \n")
     filetorun = "cmsRun "+"py_electronTP"+TagProbeName[sample]+"et.py \n"     
     file.write(filetorun)
     file.write("date \n")
     file.close()
     command = "chmod a+x "+csh_name
     os.system(command)
     # ******************  creating the condor job  *********************
     name = DIR_PATH+"CondorJob_electronTP"+TagProbeName[sample]+"et"
     file = open(name,'w')
     file.write("universe = vanilla \n")
     phrase = "Executable = "+csh_name+" \n"
     file.write(phrase)
     file.write("Requirements = Memory >= 199 &&OpSys == \"LINUX\"&& (Arch != \"DUMMY\" ) \n")
     file.write("Should_Transfer_Files = YES \n")
     file.write("WhenTOTransferOutput  = ON_EXIT \n")
     phrase = "Output = events"+TagProbeName[sample]+"et.stdout \n"
     file.write(phrase)
     phrase = "Error = events"+TagProbeName[sample]+"et.stderr \n"
     file.write(phrase)
     phrase = "Log = events"+TagProbeName[sample]+"et.condorlog \n"
     file.write(phrase)
     phrase = "notify_user = "+EMAIL+" \n"
     file.write(phrase)
     file.write("Queue 1 \n")
     file.close()

print "Done........" #End of sample loop          
