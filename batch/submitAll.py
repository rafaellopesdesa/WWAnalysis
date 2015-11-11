#!/bin/env python

import os, re, sys, ROOT, time, math, filecmp, shutil


def main():

    if len(sys.argv) != 2 and len(sys.argv) != 3:
        print 'Usage: '
        print '   submitAll.py list.txt [sample]'
        sys.exit(0)

    samples = readList(sys.argv[1])
    filesPerJob = getFilesPerJob()
    filesToSubmit = []        
    
    for sampleName, sampleInfo in samples.iteritems():
        try:
            if sampleName.strip() != sys.argv[2].strip():
                continue
        except IndexError:
            pass

        print 20*'-'
        print 'Doing', sampleName
        fileCount = 0
        try:
            for file in os.listdir(sampleInfo[0]):
                match = re.search('merged_ntuple_(\d+).root', file)
                if match is not None:
                    fileCount = fileCount + 1
        except OSError:
            print 'The directory %s for sample %s does not exist, I will skip it' % (sampleInfo[0], sampleName)
            continue

        print 'Found', fileCount, 'files in hadoop'
        numJobs = int(math.ceil(float(fileCount)/float(filesPerJob)))
        print 'Will do in', numJobs, 'jobs'
        jobFailures = []
        jobSuccess = []
        
        for job in xrange(numJobs):
            if not os.path.exists('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job)):
                jobFailures.append(job)
                continue

            rootFile = ROOT.TFile.Open('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job))

            if rootFile == None:
                os.unlink('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job))
                jobFailures.append(job)
                continue

            if rootFile.IsZombie():
                os.unlink('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job))
                jobFailures.append(job)
                continue

            rootTree = rootFile.Get('t')
            if not rootTree:
                os.unlink('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job))
                jobFailures.append(job)
                continue
            
            rootFile.Close()
            jobSuccess.append('/hadoop/cms/store/user/rcoelhol/WWbabies/%s_%d_baby.root' % (sampleName, job))
            
        print 'Found', len(jobFailures), 'files missing'
        if len(jobFailures) == numJobs:
            filesToSubmit.append('submit.%s' % sampleName)
            writeCondor('submit.%s' % sampleName, sampleName, numJobs, sampleInfo[1])
        elif len(jobFailures) > 0:
            localtime   = time.localtime()
            timeString  = time.strftime("%Y%m%d%H%M%S", localtime)
            filesToSubmit.append('submit.%s.recover.%s' % (sampleName, timeString))
            writeRecovery('submit.%s.recover.%s' % (sampleName, timeString), sampleName, sampleInfo[1], jobFailures)
        else:
            mergeFiles(jobSuccess, sampleName)
            
    print 'Making tar file and submitting'
    makeTar()
    submitCondor(filesToSubmit)


def mergeFiles(listOfFiles, sampleName):

    doMerge = False
    if not os.path.exists('/hadoop/cms/store/user/rcoelhol/WWbabies/merged/%s.root' % sampleName):
        doMerge = True
    else:
        for files in listOfFiles:
            if os.stat(files).st_mtime - os.stat('/hadoop/cms/store/user/rcoelhol/WWbabies/merged/%s.root' % sampleName).st_mtime > 0:
                doMerge = True
                break
    if not doMerge:
        return 
    
    os.system('hadd /nfs-5/users/rclsa/WW_2015/CMSSW_7_4_12/src/WWAnalysis/batch/merged/%s.root %s' % (sampleName, ' '.join(listOfFiles)))
    hasCopied = False
    while not hasCopied:
        try:
            os.unlink('/hadoop/cms/store/user/rcoelhol/WWbabies/merged/%s.root' % sampleName)
        except:
            pass
        shutil.copy('/nfs-5/users/rclsa/WW_2015/CMSSW_7_4_12/src/WWAnalysis/batch/merged/%s.root' % sampleName, '/hadoop/cms/store/user/rcoelhol/WWbabies/merged/%s.root' % sampleName)
        hasCopied = filecmp.cmp('/nfs-5/users/rclsa/WW_2015/CMSSW_7_4_12/src/WWAnalysis/batch/merged/%s.root' % sampleName, '/hadoop/cms/store/user/rcoelhol/WWbabies/merged/%s.root' % sampleName)
    os.unlink('/nfs-5/users/rclsa/WW_2015/CMSSW_7_4_12/src/WWAnalysis/batch/merged/%s.root' % sampleName)
    
def getFilesPerJob():

    retval = -1
    makeFile = open('makeListOfFiles.py')
    for line in makeFile:
        match = re.search('njob = (\d+)', line)
        if match is not None:
            retval = int(match.group(1))
    if retval < 0:
        print 'Events per file not found, I will use 1'
        retval = 1
    return retval
                
def readList(fileName):

    retval = {}
    listFile = open(fileName)

    for sample in listFile:
        if '--' in sample:
            continue
        if 'TODO' in sample:
            continue
        info = sample.strip().split()
        if len(info) == 2:
            retval[info[0]] = [info[1], 1.0]
        elif len(info) == 3:
            retval[info[0]] = [info[1], float(info[2])]
    listFile.close()

    return retval

def writeCondor(fileName, sampleName, numJobs, fudgeFactor):

    configFile = open(fileName, 'w')

    configFile.write('universe=grid\n')
    configFile.write('Grid_Resource=condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu\n')
    configFile.write('+DESIRED_Sites="T2_US_UCSD"\n')
    configFile.write('Should_Transfer_Files = YES\n')
    configFile.write('WhenToTransferOutput = ON_EXIT\n')
    configFile.write('Transfer_Input_Files = ../../WWAnalysis.tar.gz\n')
    configFile.write('Output = logs/condorLog.%s.$(Process).stdout\n' % sampleName)
    configFile.write('Error = logs/condorLog.%s.$(Process).stderr\n' % sampleName)
    configFile.write('Log = logs/condorLog.%s.$(Process).log\n' % sampleName)
    configFile.write('+Owner = undefined\n')
    configFile.write('notify_user = rclsa@fnal.gov\n')
    configFile.write('x509userproxy = /tmp/x509up_u31150\n')
    configFile.write('Executable = runBabyMaker.sh\n')
    configFile.write('Transfer_executable = True\n')
    configFile.write('getenv = True\n')

    configFile.write('arguments = $(Process) %s %f\n' % (sampleName, fudgeFactor))
    configFile.write('Queue %d\n' % numJobs)

    configFile.close()

def writeRecovery(fileName, sampleName, fudgeFactor, listOfFailures):

    configFile = open(fileName, 'w')

    configFile.write('universe=grid\n')
    configFile.write('Grid_Resource=condor cmssubmit-r1.t2.ucsd.edu glidein-collector.t2.ucsd.edu\n')
    configFile.write('+DESIRED_Sites="T2_US_UCSD"\n')
    configFile.write('Should_Transfer_Files = YES\n')
    configFile.write('WhenToTransferOutput = ON_EXIT\n')
    configFile.write('Transfer_Input_Files = ../../WWAnalysis.tar.gz\n')
    configFile.write('Output = logs/condorLog.%s.$(Process).recover.stdout\n' % sampleName)
    configFile.write('Error = logs/condorLog.%s.$(Process).recover.stderr\n' % sampleName)
    configFile.write('Log = logs/condorLog.%s.$(Process).recover.log\n' % sampleName)
    configFile.write('+Owner = undefined\n')
    configFile.write('notify_user = rclsa@fnal.gov\n')
    configFile.write('x509userproxy = /tmp/x509up_u31150\n')
    configFile.write('Executable = runBabyMaker.sh\n')
    configFile.write('Transfer_executable = True\n')
    configFile.write('getenv = True\n')

    for jobNumber in listOfFailures:
        configFile.write('arguments = %d %s %f\n' % (jobNumber, sampleName, fudgeFactor))
        configFile.write('Queue 1\n')

    configFile.close()

def submitCondor(filesToSubmit):

#    os.system('voms-proxy-init --voms cms -hours 120')
    for fileName in filesToSubmit:
        os.system('condor_submit %s' % fileName)
#    print 'submit yourself'

def makeTar():

    diff_time = os.stat('../../WWAnalysis.tar.gz').st_mtime - os.stat('../main.exe').st_mtime
    if diff_time < 0:        
        if os.path.exists('../../WWAnalysis.tar.gz'):
            os.unlink('../../WWAnalysis.tar.gz')
        os.system('tar -czpf ../../WWAnalysis.tar.gz ../../WWAnalysis')
        
if __name__ == "__main__":
    main()
