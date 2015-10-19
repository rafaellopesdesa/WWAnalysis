#!/bin/bash

echo "Getting the data pileup..."
pileupCalc.py -i $1 --inputLumiJSON pileup_JSON_1.2_PRE_10-09-2015.txt --calcMode true --minBiasXsec 80000 --maxPileupBin 50 --numPileupBins 50  MyDataPileupHistogram.root
echo "Do you need to update the MC pileup [Yes/No]?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) root -q -l -b -x getPileUp.C; break;;
        No ) break;;
    esac
done
