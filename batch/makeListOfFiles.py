#!/bin/env python

import os, sys

njob = 1
ijob = int(sys.argv[2])

fileList = open('list.txt')
fileInfo = [line.split() for line in fileList.readlines() if (len(line) > 1 and line.split()[0] == sys.argv[1])]
fileList.close()


newFile = open('file.list', 'w')
for ifile in range(ijob*njob+1, (ijob+1)*njob+1):
    if os.path.exists('%smerged_ntuple_%d.root' % (fileInfo[0][1], ifile)):
        newFile.write('%smerged_ntuple_%d.root\n' % (fileInfo[0][1], ifile))
newFile.close()


