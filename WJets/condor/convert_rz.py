import os,sys
import string, re
from time import gmtime, localtime, strftime

parameters   = ["Delta_g1(Z)", "Delta_K(Z)", "Delta_K(gamma)", "Lambda(Z)", "Lambda(gamma)"]
parameters_wo_brackets  = ["Delta_g1_Z", "Delta_K_Z", "Delta_K_gamma", "Lambda_Z", "Lambda_gamma"]
Values = ["-0.25d0", "-0.2d0", "-0.15d0", "-0.1d0", "-0.05d0", "0.05d0", "0.1d0", "0.15d0", "0.2d0", "0.25d0"] 
output_string = "wwqqbr_tota_cteq6l1_1___1___test"
hist_value = ["0__.rz", "1__.rz", "2__.rz", "3__.rz", "4__.rz", "5__.rz"]


def clean_up_output(index, change):
    dirName = parameters_wo_brackets[index] + change 
    child = os.system("cd " + dirName)
    child2 = os.system("../h2root " + output_string + hist_value[index] )
    print "The .rz files have been converted.\n"

###################
    for i in range(len(parameters)):
        for j in range(len(Values)):
                    clean_up_output(i, Values[j])
                    
