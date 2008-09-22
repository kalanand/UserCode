#!/bin/csh

echo "Configuring CMS environment...."

source /uscmst1/prod/sw/cms/cshrc uaf

cd {PATHTOBASE}
eval `scramv1 runtime -csh`

if (${?_CONDOR_SCRATCH_DIR}) then 
   echo " change to Condor Batch system scratch disk"
   cd ${_CONDOR_SCRATCH_DIR}
else 
   echo " no Condor scratch directory! "
endif

if (-e {OUTFILE}) then
  echo " {OUTFILE} already exists exiting"
  echo " to rerun delete or rename  first "
  exit 1 
endif 

cmsRun -p {CONFIGFILE}
if ( $? == 0 ) then 
    echo 'Congratulations !!!! processing File went fine ! '
    exit 0
else
      echo 'Error processing File cleaning up now '  
      /bin/rm -f {OUTFILE}
      exit 1
endif

