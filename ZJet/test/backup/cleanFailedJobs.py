#!/usr/bin/env python
#################################################
#
# Francisco Yumiceva
# Fermilab 2007
#

import sys
import os

#________________________________________________________________
def get_list_files(directory,extension = ""):

    dir = []

    dir = os.listdir(directory)

    lfiles = []
    for f in dir:

        if f.endswith(extension):

            lfiles.append(directory+"/"+f)

    return lfiles


def main():

    if len(sys.argv) < 2:
        print "\n cleanFailedJobs.py <test=1, remove=0> <output directory >"
        print " eg. cleanFailedJobs.py 1 /uscmst1b_scratch/lpc1/cmsroc/yumiceva/TQAF/\n"
        sys.exit()

    istest = sys.argv[1]
    directory = sys.argv[2]

    logfiles = get_list_files(directory+"/logs",".log")
    stderrfiles = get_list_files(directory+"/logs",".stderr")
    stdoutfiles = get_list_files(directory+"/logs",".stdout")

    # check for bad jobs
    i_file = 0
    rm_logfiles = []
    rm_stderrfiles = []
    rm_stdoutfiles = []
    
    for f in stdoutfiles:
        #print f
        afile = open(f)

        # check file size
        if os.stat(f).st_size == 0L:
            rm_logfiles.append(logfiles[i_file])
            rm_stderrfiles.append(stderrfiles[i_file])
            rm_stdoutfiles.append(stdoutfiles[i_file])
        else:
            for line in afile:
                if line.find("Error processing File cleaning up now") == 0:
                    # a bad file
                    rm_logfiles.append(logfiles[i_file])
                    rm_stderrfiles.append(stderrfiles[i_file])
                    rm_stdoutfiles.append(stdoutfiles[i_file])
        
        i_file = i_file + 1

    # remove files
    if istest:
        print " NO files will be removed!\n"
    
    for f in rm_logfiles:
        print " rm "+f
        if not istest:
            os.remove(f)

    for f in rm_stderrfiles:
        print " rm "+f
        if not istest:
            os.remove(f)

    for f in rm_stdoutfiles:
        print " rm "+f
        if not istest:
            os.remove(f)


if __name__ =='__main__':
    sys.exit(main())


                
                
