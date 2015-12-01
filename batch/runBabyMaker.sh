#!/bin/sh

tar -xzf WWAnalysis.tar.gz
cd WWAnalysis/batch
python makeListOfFiles.py ${2} ${1} 
cd ..
cat batch/file.list
./main.exe batch/file.list ${2}_${1} ${3} ${4}
lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file:`pwd`/${2}_${1}_baby.root "srm://bsrm-1.t2.ucsd.edu:8443/srm/v2/server?SFN=/hadoop/cms/store/user/rcoelhol/WWbabies/${2}_${1}_baby.root"

