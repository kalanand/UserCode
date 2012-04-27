import os,sys
import string, re
from time import gmtime, localtime, strftime

dataset    = [
##     "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
    "/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM ",
    "/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-500_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-550_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/GluGluToHToWWToLNuQQ_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM",
##     "/WJets_TuneD6T_scaleup_7TeV-madgraph-tauola/Fall10-START38_V12-v1/AODSIM",
##     "/WJets_TuneD6T_scaledown_7TeV-madgraph-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO"
##     "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",   
##     "/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/Tbar_TuneZ2_tW-channel-DS_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
    #"/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",    
    #"/T_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/T_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/T_TuneZ2_tW-channel-DS_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM",
##     "/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM"   
]


channels   = [
##     "WJets",
    "WWtoAnything",
    "ZZtoAnything",    
##     "WZtoAnything",
##     "TTJets",
##     "HWWToLNuQQ_M-170",
##     "HWWToLNuQQ_M-180",
##     "HWWToLNuQQ_M-190",
##     "HWWToLNuQQ_M-200",
##     "HWWToLNuQQ_M-250",    
##     "HWWToLNuQQ_M-300",
##     "HWWToLNuQQ_M-350",
##     "HWWToLNuQQ_M-400",
##     "HWWToLNuQQ_M-450",
##     "HWWToLNuQQ_M-500",
##     "HWWToLNuQQ_M-550",
##     "HWWToLNuQQ_M-600",     
##     "QCD_Pt-20_MuEnrichedPt-15",
##     "WJetsToLNu_TuneD6T_Fall10",
##     "WJets_TuneD6T_scaleup",
##     "WJets_TuneD6T_scaledown"
##     "DYJetsToLL",
##     "Tbar_s-channel",
##     "Tbar_t-channel",
##     "Tbar_tW-channel-DS",
    #"Tbar_tW-channel-DR",    
    #"T_s-channel",
##     "T_t-channel",
##     "T_tW-channel-DS",
##     "T_tW-channel-DR"    
]
##condor   = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
##condor   = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
condor   = [1,1]



def changeCrabTemplateFile(index):
    fin  = open("crabTemplatePATtuple.cfg")
    pset_crab     = "crabjob_" + channels[index] + ".cfg"
    fout = open(pset_crab,"w")
    for line in fin.readlines():
        if  line.find("mydataset")!=-1:
            line=line.replace("mydataset",dataset[index])
            fout.write("\n")
        if  line.find("mypublishdataname")!=-1:
            line=line.replace("mypublishdataname", "ttbsm_v10beta_"+channels[index]+"_Fall11-PU_S6_START42_V14B")
        if  line.find("mypdirname")!=-1:
            line=line.replace("mypdirname",channels[index])                        
        if line.find("glite")!=-1 and condor[index]!=0:
            line=line.replace("glite", "condor")        
        fout.write(line)        
    if condor[index]!=0:
        fout.write("ce_white_list = cmssrm.fnal.gov")
      
    print pset_crab + " has been written.\n"



def setupFJ3(index):
    jobfile = channels[index]+"/job/CMSSW.sh"
    fin  = open(jobfile)
    fout = open(jobfile+".tmp","w")
    for line in fin.readlines():
        fout.write(line)
        if  line.find("eval `scram runtime -sh | grep -v SCRAMRT_LSB_JOBNAME`")!=-1:
            fout.write("\n")    
            fout.write("scram setup $CMS_PATH/slc5_amd64_gcc434/external/fastjet-toolfile/1.0-cms7/etc/scram.d/fastjet.xml")   
            fout.write("\n")
    child0 = os.system("mv " + jobfile+".tmp " + jobfile)
    print  "setup FJ3 has been done in file "+jobfile+"\n"

            
###################
for i in range(len(channels)):
    changeCrabTemplateFile(i)    
    submitcommand = "crab -create -cfg " + "crabjob_" + channels[i] + ".cfg"
    child   = os.system(submitcommand)
    setupFJ3(i)
    child2   = os.system("crab -submit -c "+channels[i])
