{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("Events"); 
  ch->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/WWTo2L2Nu_13TeV-powheg_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_1.root");
  ScanChain(ch); 
}