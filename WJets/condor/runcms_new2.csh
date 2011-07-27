#!/bin/csh

if (${?_CONDOR_SCRATCH_DIR}) then 
   echo " change to Condor Batch system scratch disk"
   cd ${_CONDOR_SCRATCH_DIR}
else 
   echo " no Condor scratch directory! "
endif


cp -r /uscms_data/d2/kalanand/software/MCFM .
cd MCFM/Bin
./mcfm {OUTDIR}/{CONFIGFILE}
mv *.rz  /uscms_data/d2/kalanand/software/MCFM/Bin/{OUTDIR}/


if ( $? == 0 ) then 
    echo 'Congratulations !!!! processing File went fine ! '
    exit 0
else
      echo 'Error processing File cleaning up now '  
      exit 1
endif

