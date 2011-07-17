import os,sys
import string, re
from time import gmtime, localtime, strftime

ConfigFile = "inputWWToLnuJJ.DAT"
parameters   = ["Delta_g1(Z)", "Delta_K(Z)", "Delta_K(gamma)", "Lambda(Z)", "Lambda(gamma)"]
parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 



def changeConfigFile(index, change):
    fin  = open(ConfigFile)
    pset_cfg      = "aTGC_" + parameters_wo_brackets[index] + change + ".DAT"
    fout = open(pset_cfg,"w")
    for line in fin.readlines():
        if  line.find(parameters[index])!=-1:
            line=line.replace("0.0d0", change )       
        fout.write(line)
    print pset_cfg + " has been written.\n"


###################
for i in range(len(parameters)):
    for j in range(len(Values)):
        changeConfigFile(i, Values[j])
