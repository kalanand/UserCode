#!/bin/csh

echo "****** Submitting All MCFM bin jobs to condor queue ...."

./submitToCondor.py -c aTGC_Lambda_Z0.25d0.DAT -o Lambda_Z0.25d0
