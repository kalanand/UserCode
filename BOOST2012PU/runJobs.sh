#!/bin/tcsh
./anal01.exe --mu=30 --flist=MinBias_7TeV_FileList.txt --nevts=100000 --output=test_7TeV_100kevents_30PU_noPUsub.root
./anal01.exe --mu=60 --flist=MinBias_7TeV_FileList.txt --nevts=100000 --output=test_7TeV_100kevents_60PU_noPUsub.root
./anal01.exe --mu=100 --flist=MinBias_7TeV_FileList.txt --nevts=100000 --output=test_7TeV_100kevents_100PU_noPUsub.root
./anal01.exe --mu=200 --flist=MinBias_7TeV_FileList.txt --nevts=100000 --output=test_7TeV_100kevents_200PU_noPUsub.root
./anal01.exe --mu=30 --flist=MinBias_8TeV_FileList.txt --nevts=100000 --output=test_8TeV_100kevents_30PU_noPUsub.root
./anal01.exe --mu=60 --flist=MinBias_8TeV_FileList.txt --nevts=100000 --output=test_8TeV_100kevents_60PU_noPUsub.root
./anal01.exe --mu=100 --flist=MinBias_8TeV_FileList.txt --nevts=100000 --output=test_8TeV_100kevents_100PU_noPUsub.root
./anal01.exe --mu=200 --flist=MinBias_8TeV_FileList.txt --nevts=100000 --output=test_8TeV_100kevents_200PU_noPUsub.root
