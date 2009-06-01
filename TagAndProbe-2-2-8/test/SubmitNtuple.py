#! /usr/bin/env python
import os

for sample in range(0,43):
    command = "condor_submit CondorJob_Georgios_ntpl_"+str(sample)+".condor"
    os.system(command)
        
        
