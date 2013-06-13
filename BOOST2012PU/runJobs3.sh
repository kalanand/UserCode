#!/bin/tcsh

##./anal02.exe --mu=-30 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=20 --PUsub=0 --output=signalWithPU_8TeV_20events_noPUsub.root

##./anal02.exe --mu=-30 --sigflist=signalFileList.txt --puflist=testFileList.txt --nevts=20 --PUsub=0 --output=signalWithPU_8TeV_20events_noPUsub.root

##./anal01.exe --mu=-1 --flist=signalFileList.txt --nevts=2000 --PUsub=0 --output=signal_8TeV_2kevents_noPUsub.root
##./anal01.exe --mu=-1 --flist=signalFileList.txt --nevts=2000 --PUsub=1 --output=signal_8TeV_2kevents_PUsub.root



./anal02.exe --mu=30 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=0 --output=signal_8TeV_100kevents_30PU_noPUsub.root
./anal02.exe --mu=60 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=0 --output=signal_8TeV_100kevents_60PU_noPUsub.root
./anal02.exe --mu=100 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=0 --output=signal_8TeV_100kevents_100PU_noPUsub.root
./anal02.exe --mu=200 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=0 --output=signal_8TeV_100kevents_200PU_noPUsub.root

./anal02.exe --mu=30 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=1 --output=signal_8TeV_100kevents_30PU_PUsub.root
./anal02.exe --mu=60 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=1 --output=signal_8TeV_100kevents_60PU_PUsub.root
./anal02.exe --mu=100 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=1 --output=signal_8TeV_100kevents_100PU_PUsub.root
./anal02.exe --mu=200 --sigflist=signalFileList.txt --puflist=test_8TeV_FileList.txt --nevts=100000 --PUsub=1 --output=signal_8TeV_100kevents_200PU_PUsub.root
