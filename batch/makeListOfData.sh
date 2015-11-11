#!/bin/bash

rm -rf Run2015.list
ls  /hadoop/cms/store/group/snt/run2_data/Run2015C_25ns_DoubleEG_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015C_25ns_DoubleMuon_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015C_25ns_MuonEG_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015C_25ns_SingleElectron_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015C_25ns_SingleMuon_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_DoubleEG_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_DoubleMuon_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_MuonEG_MINIAOD_05Oct2015-v2/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_SingleElectron_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_SingleMuon_MINIAOD_05Oct2015-v1/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_DoubleEG_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_DoubleMuon_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_MuonEG_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_SingleElectron_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_*root \
    /hadoop/cms/store/group/snt/run2_data/Run2015D_SingleMuon_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_*root > Run2015.list