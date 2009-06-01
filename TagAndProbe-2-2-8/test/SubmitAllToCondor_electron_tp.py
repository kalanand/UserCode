#! /usr/bin/env python
import os

TagProbeName=["_WenuOff_","_WenuOn_","_ZeeOff_", "_ZeeOn_"]
varstring="et"


for sample in range(0,4):
    command = "condor_submit CondorJob_electronTP"+TagProbeName[sample]+varstring
    os.system(command)
        
        
