#include "helper_babymaker.h"

using namespace tas;
using namespace WWAnalysis;

//Main functions
babyMaker::babyMaker(bool isDataFromFileName, bool debug){

  path = ".";
  verbose = debug;
  evt_cut = 0;

  std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3;
  std::string jetcorr_uncertainty_filename;

  jetcorr_filenames_pfL1FastJetL2L3.clear();
  if (isDataFromFileName) {
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_DATA/Summer15_25nsV6_DATA_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_DATA/Summer15_25nsV6_DATA_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_DATA/Summer15_25nsV6_DATA_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_DATA/Summer15_25nsV6_DATA_L2L3Residual_AK4PFchs.txt");

  } else {
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_MC/Summer15_25nsV6_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_MC/Summer15_25nsV6_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_pfL1FastJetL2L3.push_back("./jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_MC/Summer15_25nsV6_MC_L3Absolute_AK4PFchs.txt");

  }
  jetcorr_uncertainty_filename = "jetCorrections/JECDatabase/textFiles/Summer15_25nsV6_DATA/Summer15_25nsV6_DATA_Uncertainty_AK4PFchs.txt";

  jetCorrector = makeJetCorrector(jetcorr_filenames_pfL1FastJetL2L3);
  jetCorrector_unc = new JetCorrectionUncertainty(jetcorr_uncertainty_filename);

  resummation = new WWpTreweight("resummation/central.dat",
				 "resummation/resum_up.dat",
				 "resummation/resum_down.dat",
				 "resummation/scale_up.dat",
				 "resummation/scale_down.dat",
				 "resummation/nnlo_central.dat",
				 "resummation/powheg_2l2nu_nlo.dat",
				 "resummation/powheg_2l2nu_qup_nlo.dat",
				 "resummation/powheg_2l2nu_qdown_nlo.dat",
				 "resummation/powheg_2l2nu_sup_nlo.dat",
				 "resummation/powheg_2l2nu_sdown_nlo.dat",
				 "resummation/powheg_2l2nu_nnlo.dat");
    
  MVA_0jet = new TMVA::Reader("!Color:Silent");    
  MVA_1jet = new TMVA::Reader("!Color:Silent");    
  MVA_jets = new TMVA::Reader("!Color:Silent");    
  
  MVA_0jet_higgs = new TMVA::Reader("!Color:Silent");    
  MVA_1jet_higgs = new TMVA::Reader("!Color:Silent");    
  MVA_jets_higgs = new TMVA::Reader("!Color:Silent");    
  
  MVA_0jet->AddVariable("met", &met);
  MVA_0jet->AddVariable("min_projmet", &min_projmet);
  MVA_0jet->AddVariable("metsig", &metsig);
  MVA_0jet->AddVariable("uperp", &uperp);
  MVA_0jet->AddVariable("upara", &upara);
  MVA_0jet->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_0jet->AddVariable("dilep_pt", &dilep_pt);
  MVA_0jet->AddVariable("min_mt", &min_mt);
  MVA_0jet->AddVariable("max_mt", &max_mt);
  MVA_0jet->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_0jet->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);

  MVA_1jet->AddVariable("met", &met);
  MVA_1jet->AddVariable("min_projmet", &min_projmet);
  MVA_1jet->AddVariable("metsig", &metsig);
  MVA_1jet->AddVariable("uperp", &uperp);
  MVA_1jet->AddVariable("upara", &upara);
  MVA_1jet->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_1jet->AddVariable("dilep_pt", &dilep_pt);
  MVA_1jet->AddVariable("min_mt", &min_mt);
  MVA_1jet->AddVariable("max_mt", &max_mt);
  MVA_1jet->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_1jet->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);
  MVA_1jet->AddVariable("jet1_met_dphi", &jet1_met_dphi);
  MVA_1jet->AddVariable("jet1_pt", &jet1_pt);
  MVA_1jet->AddVariable("dilep_jet1_dphi", &dilep_jet1_dphi);

  MVA_jets->AddVariable("met", &met);
  MVA_jets->AddVariable("min_projmet", &min_projmet);
  MVA_jets->AddVariable("metsig", &metsig);
  MVA_jets->AddVariable("uperp", &uperp);
  MVA_jets->AddVariable("upara", &upara);
  MVA_jets->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_jets->AddVariable("dilep_pt", &dilep_pt);
  MVA_jets->AddVariable("min_mt", &min_mt);
  MVA_jets->AddVariable("max_mt", &max_mt);
  MVA_jets->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_jets->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);
  MVA_jets->AddVariable("jet1_met_dphi", &jet1_met_dphi);
  MVA_jets->AddVariable("jet2_met_dphi", &jet2_met_dphi);
  MVA_jets->AddVariable("jet1_pt", &jet1_pt);
  MVA_jets->AddVariable("jet2_pt", &jet2_pt);
  MVA_jets->AddVariable("dilep_jet1_dphi", &dilep_jet1_dphi);
  MVA_jets->AddVariable("dilep_jet2_dphi", &dilep_jet2_dphi);

  MVA_0jet_higgs->AddVariable("met", &met);
  MVA_0jet_higgs->AddVariable("min_projmet", &min_projmet);
  MVA_0jet_higgs->AddVariable("metsig", &metsig);
  MVA_0jet_higgs->AddVariable("uperp", &uperp);
  MVA_0jet_higgs->AddVariable("upara", &upara);
  MVA_0jet_higgs->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_0jet_higgs->AddVariable("dilep_pt", &dilep_pt);
  MVA_0jet_higgs->AddVariable("min_mt", &min_mt);
  MVA_0jet_higgs->AddVariable("max_mt", &max_mt);
  MVA_0jet_higgs->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_0jet_higgs->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);

  MVA_1jet_higgs->AddVariable("met", &met);
  MVA_1jet_higgs->AddVariable("min_projmet", &min_projmet);
  MVA_1jet_higgs->AddVariable("metsig", &metsig);
  MVA_1jet_higgs->AddVariable("uperp", &uperp);
  MVA_1jet_higgs->AddVariable("upara", &upara);
  MVA_1jet_higgs->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_1jet_higgs->AddVariable("dilep_pt", &dilep_pt);
  MVA_1jet_higgs->AddVariable("min_mt", &min_mt);
  MVA_1jet_higgs->AddVariable("max_mt", &max_mt);
  MVA_1jet_higgs->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_1jet_higgs->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);
  MVA_1jet_higgs->AddVariable("jet1_met_dphi", &jet1_met_dphi);
  MVA_1jet_higgs->AddVariable("jet1_pt", &jet1_pt);
  MVA_1jet_higgs->AddVariable("dilep_jet1_dphi", &dilep_jet1_dphi);

  MVA_jets_higgs->AddVariable("met", &met);
  MVA_jets_higgs->AddVariable("min_projmet", &min_projmet);
  MVA_jets_higgs->AddVariable("metsig", &metsig);
  MVA_jets_higgs->AddVariable("uperp", &uperp);
  MVA_jets_higgs->AddVariable("upara", &upara);
  MVA_jets_higgs->AddVariable("nGoodVertices", &nGoodVertices_float);
  MVA_jets_higgs->AddVariable("dilep_pt", &dilep_pt);
  MVA_jets_higgs->AddVariable("min_mt", &min_mt);
  MVA_jets_higgs->AddVariable("max_mt", &max_mt);
  MVA_jets_higgs->AddVariable("min_lep_met_dphi", &min_lep_met_dphi);   
  MVA_jets_higgs->AddVariable("max_lep_met_dphi", &max_lep_met_dphi);
  MVA_jets_higgs->AddVariable("jet1_met_dphi", &jet1_met_dphi);
  MVA_jets_higgs->AddVariable("jet2_met_dphi", &jet2_met_dphi);
  MVA_jets_higgs->AddVariable("jet1_pt", &jet1_pt);
  MVA_jets_higgs->AddVariable("jet2_pt", &jet2_pt);
  MVA_jets_higgs->AddVariable("dilep_jet1_dphi", &dilep_jet1_dphi);
  MVA_jets_higgs->AddVariable("dilep_jet2_dphi", &dilep_jet2_dphi);

  MVA_0jet->BookMVA("BDT", "./DYMVA/weights/dymva_0jet_BDT.weights.xml");
  MVA_1jet->BookMVA("BDT", "./DYMVA/weights/dymva_1jet_BDT.weights.xml");
  MVA_jets->BookMVA("BDT", "./DYMVA/weights/dymva_jets_BDT.weights.xml");
  MVA_0jet_higgs->BookMVA("BDT", "./DYMVA/weights/dymva_0jet_higgs_BDT.weights.xml");
  MVA_1jet_higgs->BookMVA("BDT", "./DYMVA/weights/dymva_1jet_higgs_BDT.weights.xml");
  MVA_jets_higgs->BookMVA("BDT", "./DYMVA/weights/dymva_jets_higgs_BDT.weights.xml");
  
  MVAs.push_back(MVA_0jet);
  MVAs.push_back(MVA_1jet);
  MVAs.push_back(MVA_jets);
  MVAs.push_back(MVA_0jet_higgs);
  MVAs.push_back(MVA_1jet_higgs);
  MVAs.push_back(MVA_jets_higgs);

  TFile* input = TFile::Open("pileup/MyRatioPileupHistogram.root");
  scalePileupHist = (TH1D*) ((TH1D*) input->Get("pileup"))->Clone();
  scalePileupHist->SetDirectory(0);
  //  input->Close();

}

void babyMaker::MakeBabyNtuple(const char* output_name){

  //Create Baby
  BabyFile = new TFile(Form("%s/%s.root", path.Data(), output_name), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "WW2015 Baby Ntuple");

  BabyTree->Branch("scalePileup", &scalePileup);
  BabyTree->Branch("scaleFR", &scaleFR);
  BabyTree->Branch("scaleDY", &scaleDY);
  BabyTree->Branch("scaleTOP", &scaleTOP);
  BabyTree->Branch("scaleID", &scaleID);
  BabyTree->Branch("scaleTrigger", &scaleTrigger);
  BabyTree->Branch("scaleWWpT", &scaleWWpT);
  BabyTree->Branch("dymva", &dymva);
  BabyTree->Branch("met", &met);
  BabyTree->Branch("metPhi", &metPhi);
  BabyTree->Branch("event", &event);
  BabyTree->Branch("lumi", &lumi);
  BabyTree->Branch("run", &run);
  BabyTree->Branch("is_real_data", &is_real_data);
  BabyTree->Branch("scale1fb", &scale1fb);
  BabyTree->Branch("xsec", &xsec);
  BabyTree->Branch("kfactor", &kfactor);
  BabyTree->Branch("filename", &filename);
  BabyTree->Branch("trueNumInt", &trueNumInt);
  BabyTree->Branch("nPUvertices", &nPUvertices);
  BabyTree->Branch("bunchCrossing", &bunchCrossing);
  BabyTree->Branch("nGoodVertices", &nGoodVertices);
  BabyTree->Branch("gen_weights", &gen_weights);
  BabyTree->Branch("pass_met_filters", &pass_met_filters);
  BabyTree->Branch("gen_met", &gen_met);
  BabyTree->Branch("gen_met_phi", &gen_met_phi);
  BabyTree->Branch("met30", &met30);
  BabyTree->Branch("metPhi30", &metPhi30);
  BabyTree->Branch("set30", &set30);
  BabyTree->Branch("met_raw", &met_raw);
  BabyTree->Branch("metPhi_raw", &metPhi_raw);
  BabyTree->Branch("met_track", &met_track);
  BabyTree->Branch("metPhi_track", &metPhi_track);
  BabyTree->Branch("gen_ht", &gen_ht);
  BabyTree->Branch("genjets", &genjets);
  BabyTree->Branch("njets", &njets);
  BabyTree->Branch("njets20", &njets20);
  BabyTree->Branch("njets25", &njets25);
  BabyTree->Branch("njets30", &njets30);
  BabyTree->Branch("njets35", &njets35);
  BabyTree->Branch("njets40", &njets40);
  BabyTree->Branch("ht", &ht);
  BabyTree->Branch("ht20", &ht20);
  BabyTree->Branch("ht25", &ht25);
  BabyTree->Branch("ht30", &ht30);
  BabyTree->Branch("set", &set);
  BabyTree->Branch("pileup_jet_id", &pileup_jet_id);
  BabyTree->Branch("jet_parton_flavor", &jet_parton_flavor);
  BabyTree->Branch("jet_hadron_flavor", &jet_hadron_flavor);
  BabyTree->Branch("jets", &jets);
  BabyTree->Branch("jets_CSVv2", &jets_CSVv2);
  BabyTree->Branch("projmet", &projmet);
  BabyTree->Branch("projmet_track", &projmet_track);
  BabyTree->Branch("min_projmet", &min_projmet);
  BabyTree->Branch("metsig", &metsig);
  BabyTree->Branch("metsig30", &metsig30);
  BabyTree->Branch("dilep_pt", &dilep_pt);
  BabyTree->Branch("lep1_mt", &lep1_mt);
  BabyTree->Branch("lep2_mt", &lep2_mt);
  BabyTree->Branch("jet1_pt", &jet1_pt);
  BabyTree->Branch("jet2_pt", &jet2_pt);
  BabyTree->Branch("ut", &ut);
  BabyTree->Branch("lep1_met_dphi", &lep1_met_dphi);
  BabyTree->Branch("lep2_met_dphi", &lep2_met_dphi);
  BabyTree->Branch("dilep_jet1_dphi", &dilep_jet1_dphi);
  BabyTree->Branch("dilep_jet2_dphi", &dilep_jet2_dphi);
  BabyTree->Branch("dilep_met_dphi", &dilep_met_dphi);
  BabyTree->Branch("jet1_met_dphi", &jet1_met_dphi);
  BabyTree->Branch("jet2_met_dphi", &jet2_met_dphi);
  BabyTree->Branch("jet1_jet2_dphi", &jet1_jet2_dphi);
  BabyTree->Branch("min_jet_met_dphi", &min_jet_met_dphi);
  BabyTree->Branch("max_jet_met_dphi", &max_jet_met_dphi);
  BabyTree->Branch("min_lep_met_dphi", &min_lep_met_dphi);
  BabyTree->Branch("max_lep_met_dphi", &max_lep_met_dphi);
  BabyTree->Branch("min_mt", &min_mt);
  BabyTree->Branch("max_mt", &max_mt);
  BabyTree->Branch("upara", &upara);
  BabyTree->Branch("uperp", &uperp);
  BabyTree->Branch("mt2lblb_A", &mt2lblb_A);
  BabyTree->Branch("mt2lblb_B", &mt2lblb_B);
  BabyTree->Branch("mt2lblb_min", &mt2lblb_min);
  BabyTree->Branch("mt2bb", &mt2bb);
  BabyTree->Branch("hyp_class", &hyp_class);
  BabyTree->Branch("lep1_p4", &lep1_p4);
  BabyTree->Branch("lep2_p4", &lep2_p4);
  BabyTree->Branch("dilep_p4", &dilep_p4);
  BabyTree->Branch("lep1_id", &lep1_id);
  BabyTree->Branch("lep2_id", &lep2_id);
  BabyTree->Branch("lep1_idx", &lep1_idx);
  BabyTree->Branch("lep2_idx", &lep2_idx);
  BabyTree->Branch("hyp_type", &hyp_type);
  BabyTree->Branch("lep1_motherID", &lep1_motherID);
  BabyTree->Branch("lep2_motherID", &lep2_motherID);
  BabyTree->Branch("lep1_mc_id", &lep1_mc_id);
  BabyTree->Branch("lep2_mc_id", &lep2_mc_id);
  BabyTree->Branch("genbtags", &genbtags);
  //  BabyTree->Branch("btags", &btags);
  //  BabyTree->Branch("btags_disc", &btags_disc);
  BabyTree->Branch("nbtags", &nbtags);
  BabyTree->Branch("nbtags15", &nbtags15);
  BabyTree->Branch("nbtags20", &nbtags20);
  BabyTree->Branch("nbtags25", &nbtags25);
  BabyTree->Branch("nbtags30", &nbtags30);
  BabyTree->Branch("lep1_trigger_highpt", &lep1_trigger_highpt);
  BabyTree->Branch("lep2_trigger_highpt", &lep2_trigger_highpt);
  BabyTree->Branch("lep1_trigger_lowpt ", &lep1_trigger_lowpt );
  BabyTree->Branch("lep2_trigger_lowpt ", &lep2_trigger_lowpt );
  BabyTree->Branch("trigger_double_highpt", &trigger_double_highpt);
  BabyTree->Branch("trigger_single_highpt", &trigger_single_highpt);
  BabyTree->Branch("trigger_double_lowpt", &trigger_double_lowpt);
  BabyTree->Branch("mt", &mt);
  BabyTree->Branch("mt2", &mt2);
  BabyTree->Branch("mLSP", &mLSP);
  BabyTree->Branch("mGluino", &mGluino);
  BabyTree->Branch("mSbottom", &mSbottom);
  BabyTree->Branch("mStop", &mStop);
  BabyTree->Branch("mChargino", &mChargino);
  BabyTree->Branch("lep1_id_gen", &lep1_id_gen);
  BabyTree->Branch("lep2_id_gen", &lep2_id_gen);
  BabyTree->Branch("lep1_p4_gen", &lep1_p4_gen);
  BabyTree->Branch("lep2_p4_gen", &lep2_p4_gen);
  BabyTree->Branch("lep3_id", &lep3_id);
  BabyTree->Branch("lep3_idx", &lep3_idx);
  BabyTree->Branch("lep3_quality", &lep3_quality);
  BabyTree->Branch("lep3_p4", &lep3_p4);
  //  BabyTree->Branch("genps_p4", &genps_p4);
  //  BabyTree->Branch("genps_id", &genps_id);
  //  BabyTree->Branch("genps_id_mother", &genps_id_mother);
  //  BabyTree->Branch("genps_status", &genps_status);
  //  BabyTree->Branch("genps_id_grandma", &genps_id_grandma);
  BabyTree->Branch("genps_genjets_p4", &genps_genjets_p4);
  BabyTree->Branch("gen_bs_n", &gen_bs_n);
  BabyTree->Branch("gen_bs_p4", &gen_bs_p4);
  BabyTree->Branch("gen_bs_mother_id", &gen_bs_mother_id);
  BabyTree->Branch("gen_bs_grandma_id", &gen_bs_grandma_id);
  BabyTree->Branch("gen_bs_closest_genjet", &gen_bs_closest_genjet);
  BabyTree->Branch("gen_els_n", &gen_els_n);
  BabyTree->Branch("gen_els_p4", &gen_els_p4);
  BabyTree->Branch("gen_els_reco_p4", &gen_els_reco_p4);
  BabyTree->Branch("gen_els_mother_id", &gen_els_mother_id);
  BabyTree->Branch("gen_els_grandma_id", &gen_els_grandma_id);
  BabyTree->Branch("gen_els_reco_id", &gen_els_reco_id);
  BabyTree->Branch("gen_els_reco_iso", &gen_els_reco_iso);
  BabyTree->Branch("gen_mus_n", &gen_mus_n);
  BabyTree->Branch("gen_mus_p4", &gen_mus_p4);
  BabyTree->Branch("gen_mus_reco_p4", &gen_mus_reco_p4);
  BabyTree->Branch("gen_mus_mother_id", &gen_mus_mother_id);
  BabyTree->Branch("gen_mus_grandma_id", &gen_mus_grandma_id);
  BabyTree->Branch("gen_mus_reco_id", &gen_mus_reco_id);
  BabyTree->Branch("gen_mus_reco_iso", &gen_mus_reco_iso);
  BabyTree->Branch("gen_taus_n", &gen_taus_n);
  BabyTree->Branch("gen_taus_p4", &gen_taus_p4);
  BabyTree->Branch("gen_taus_decaymode", &gen_taus_decaymode);
  BabyTree->Branch("gen_taus_mother_id", &gen_taus_mother_id);
  BabyTree->Branch("gen_taus_grandma_id", &gen_taus_grandma_id);
  BabyTree->Branch("lep1_passes_id", &lep1_passes_id);
  BabyTree->Branch("lep2_passes_id", &lep2_passes_id);
  BabyTree->Branch("nVetoSoftMuons", &nVetoSoftMuons);
  //  BabyTree->Branch("muID_dzPV", &muID_dzPV);
  //  BabyTree->Branch("muID_dxyPV", &muID_dxyPV);
  //  BabyTree->Branch("muID_ptSig", &muID_ptSig);
  //  BabyTree->Branch("muID_type", &  muID_type);
  //  BabyTree->Branch("muID_nlayers", &  muID_nlayers);
  //  BabyTree->Branch("muID_ip3dSig", &muID_ip3dSig);
  //  BabyTree->Branch("muID_medMuonPOG", &muID_medMuonPOG);
  //  BabyTree->Branch("muID_SoftMuon", &muID_SoftMuon);
  BabyTree->Branch("muID_p4", &muID_p4);
  
  //Print warning!
  cout << "Careful!! Path is " << path << endl;


}

void babyMaker::InitBabyNtuple(){

  filename = "";
  scalePileup = -999.;
  scaleFR = -999.;
  scaleDY = -999.;
  scaleTOP = -999.;
  scaleID = -999.;
  scaleTrigger = -999.;
  met = -999.;
  metPhi = -999.;
  scale1fb = -999.;     
  xsec = -999.;         
  kfactor = -999.;      
  gen_met = -999.;      
  gen_met_phi = -999.;  
  met30 = -999.;
  metPhi30 = -999.;
  set30 = -999.;
  met_raw = -999.;
  metPhi_raw = -999.;
  met_track = -999.;
  metPhi_track = -999.;
  gen_ht = -999.;
  ht = -999.;
  ht20 = -999.;
  ht25 = -999.;
  ht30 = -999.;
  set = -999.;
  projmet = -999.;
  projmet_track = -999.;
  min_projmet = -999.;
  metsig = -999.;
  metsig30 = -999.;
  dilep_pt = -999.;
  lep1_mt = -999.;
  lep2_mt = -999.;
  jet1_pt = -999.;
  jet2_pt = -999.;
  ut = -999.;
  lep1_met_dphi = -999.;
  lep2_met_dphi = -999.;
  dilep_jet1_dphi = -999.;
  dilep_jet2_dphi = -999.;
  dilep_met_dphi = -999.;
  jet1_met_dphi = -999.;
  jet2_met_dphi = -999.;
  jet1_jet2_dphi = -999.;
  min_jet_met_dphi = -999.;
  max_jet_met_dphi = -999.;
  min_lep_met_dphi = -999.;
  max_lep_met_dphi = -999.;
  min_mt = -999.;
  max_mt = -999.;
  upara = -999.;
  uperp = -999.;
  mt2lblb_A = -999.;
  mt2lblb_B = -999.;
  mt2lblb_min = -999.;
  mt2bb = -999.;
  mt = -999.;
  mt2 = -999.;
  mLSP = -999.;
  mGluino = -999.;
  mSbottom = -999.;
  mStop = -999.;
  mChargino = -999.;
  event = -999;
  lumi = -999;
  run = -999;
  nGoodVertices = -999; 
  njets = -999;
  njets20 = -999;
  njets25 = -999;
  njets30 = -999;
  njets35 = -999;
  njets40 = -999;
  hyp_class = -999;
  lep1_id = -999;
  lep2_id = -999;
  lep1_idx = -999;
  lep2_idx = -999;
  hyp_type = -999; 
  lep1_motherID = -999;
  lep2_motherID = -999;
  lep1_mc_id = -999;
  lep2_mc_id = -999;
  nbtags = -999;
  nbtags15 = -999;
  nbtags20 = -999;
  nbtags25 = -999;
  nbtags30 = -999;
  lep1_trigger_highpt = -999;
  lep2_trigger_highpt = -999;
  lep1_trigger_lowpt = -999 ;
  lep2_trigger_lowpt = -999 ;
  lep1_id_gen = -999;
  lep2_id_gen = -999;
  lep3_id = -999;
  lep3_idx = -999;
  lep3_quality = -999;
  gen_bs_n = -999;
  gen_els_n = -999;    
  gen_mus_n = -999;
  gen_taus_n = -999;
  nVetoSoftMuons = -999;
  is_real_data = false;
  pass_met_filters = false;
  trigger_double_highpt = false;
  trigger_single_highpt = false;
  trigger_double_lowpt = false;
  lep1_passes_id = false;
  lep2_passes_id = false;
  lep1_p4 = LorentzVector(0,0,0,0);
  lep2_p4 = LorentzVector(0,0,0,0);
  lep3_p4 = LorentzVector(0,0,0,0);
  dilep_p4 = LorentzVector(0,0,0,0);
  lep1_p4_gen = LorentzVector(0,0,0,0);
  lep2_p4_gen = LorentzVector(0,0,0,0);
  trueNumInt.clear();
  nPUvertices.clear();
  bunchCrossing.clear();
  scaleWWpT.clear();
  dymva.clear();
  gen_weights.clear();
  genjets.clear();
  pileup_jet_id.clear();
  jet_parton_flavor.clear();
  jet_hadron_flavor.clear();
  jets.clear();
  jets_CSVv2.clear();
  genbtags.clear();
  btags.clear();
  btags_disc.clear();
  genps_p4.clear();
  genps_id.clear();
  genps_id_mother.clear();
  genps_status.clear();
  genps_id_grandma.clear();
  genps_genjets_p4.clear();
  gen_bs_p4.clear();
  gen_bs_mother_id.clear();
  gen_bs_grandma_id.clear();
  gen_bs_closest_genjet.clear();
  gen_els_p4.clear();
  gen_els_reco_p4.clear();
  gen_els_mother_id.clear();
  gen_els_grandma_id.clear();
  gen_els_reco_id.clear();
  gen_els_reco_iso.clear();
  gen_mus_p4.clear();
  gen_mus_reco_p4.clear();
  gen_mus_mother_id.clear();
  gen_mus_grandma_id.clear();
  gen_mus_reco_id.clear();
  gen_mus_reco_iso.clear();    
  gen_taus_p4.clear();
  gen_taus_decaymode.clear();
  gen_taus_mother_id.clear();
  gen_taus_grandma_id.clear();
  muID_dzPV.clear();      
  muID_dxyPV.clear();      
  muID_ptSig.clear();     
  muID_type.clear();
  muID_nlayers.clear();
  muID_ip3dSig.clear();   
  muID_medMuonPOG.clear();
  muID_SoftMuon.clear();
  muID_p4.clear();

    
} 

//Main function
int babyMaker::ProcessBaby(string filename_in, double fudge, int JECvar){

  //Initialize variables
  InitBabyNtuple();

  //Local variables
  bool isData = evt_isRealData();

  // Protection against known bugs
  if (tas::hyp_type().size() < 1) return -1;
  if (mus_dxyPV().size() != mus_dzPV().size()) return -1;

  //Number of good vertices
  nGoodVertices = 0;
  for (unsigned int i = 0; i < tas::vtxs_ndof().size(); i++){
    if (!isGoodVertex(i)) continue;
    nGoodVertices++;
  }
  if (nGoodVertices < 1) return -1;
  
  // Events information
  filename = filename_in;
  event = evt_event();
  lumi = evt_lumiBlock();
  run = evt_run();
  kfactor = 1;
  is_real_data = evt_isRealData();
  pass_met_filters = is_real_data ? passesMETfilterv2() : 1;
  scale1fb = is_real_data ? 1 : evt_scale1fb() * fudge;
  scalePileup = 1.;
  if (!is_real_data) {
    if (scalePileupHist) {
      if (puInfo_trueNumInteractions().size() > 1) {
	for (int ibc=0; ibc < puInfo_bunchCrossing().size(); ibc++) {
	  if (puInfo_bunchCrossing().at(ibc) == 0) {
	    scalePileup = scalePileupHist->GetBinContent(scalePileupHist->FindBin(puInfo_trueNumInteractions().at(ibc)));
	  }
	}
      }
    }
  }
  for (int iwgt = 0; iwgt<6; iwgt++) {
    if (filename.Contains("WWTo2L2Nu")) 
      scaleWWpT.push_back(resummation->reweight(getWWpT(),iwgt));
    else 
      scaleWWpT.push_back(1.0);
  }

  //Fill lepton variables
  FillLepInformation();
  if (lep1_p4.pt() < 12) return -1;
  if (lep2_p4.pt() < 12) return -1;
  if (dilep_p4.pt() < 20) return -1;

  //Fill Jet/MET variables
  FillJetInformation(JECvar);
  if (met < 15) return -1;

  //Fill gen variables
  if (!isData){
    FillGenInformation();
  }

  //Fill DY/Top variables
  FillMVAInformation();
  if (min_projmet < 15) return -1;
  FillSMuInformation();

  // Some high-level precalculation
  mt    = MT((lep1_p4+lep2_p4).pt(), (lep1_p4+lep2_p4).phi(), met, metPhi);
  mt2   = MT2(met, metPhi, lep1_p4, lep2_p4);
  if (nbtags > 1) {
    mt2lblb_A = MT2(met, metPhi, lep1_p4 + btags.at(0), lep2_p4 + btags.at(1));
    mt2lblb_B = MT2(met, metPhi, lep1_p4 + btags.at(1), lep2_p4 + btags.at(0));
    mt2lblb_min = TMath::Min(mt2lblb_A, mt2lblb_B);
    mt2bb = MT2(met, metPhi, btags.at(0), btags.at(1));    
  }
  
  //Fill Baby
  BabyTree->Fill();
  
  return 0;  

}
  
void babyMaker::FillGenInformation() {

  gen_ht = getGenHT(true);
  trueNumInt = puInfo_trueNumInteractions();
  nPUvertices = puInfo_nPUvertices();
  bunchCrossing = puInfo_bunchCrossing();
  xsec = evt_xsec_incl();
  kfactor = evt_kfactor();
  gen_met = tas::gen_met();
  gen_met_phi = tas::gen_metPhi();
  
  for (unsigned int iwgt = 0; iwgt < tas::genweights().size(); ++iwgt) {
    gen_weights.push_back(genweights().at(iwgt));
  }
  
  Lep lep1 = Lep(lep1_id, lep1_idx);
  Lep lep2 = Lep(lep2_id, lep2_idx);

  lep1_motherID = lepMotherID(lep1);
  lep2_motherID = lepMotherID(lep2);
  lep1_mc_id = lep1.mc_id();
  lep2_mc_id = lep2.mc_id();

  vector <particle_t> genPair = getGenPair(verbose);
  if (genPair.size() == 2){
    lep1_id_gen = genPair.at(0).id;
    lep2_id_gen = genPair.at(1).id;
    lep1_p4_gen = genPair.at(0).p4;
    lep2_p4_gen = genPair.at(1).p4;
  }

  genps_p4 = tas::genps_p4();
  genps_id = tas::genps_id();
  genps_id_mother = tas::genps_id_mother();
  genps_status = tas::genps_status(); 
  genps_id_grandma = tas::genps_id_simplegrandma(); 
  genps_genjets_p4 = tas::genjets_p4NoMuNoNu();

  for (int i=0; i<tas::genps_id().size(); i++) {
      
    if (tas::genps_isLastCopy().at(i) && TMath::Abs(tas::genps_id().at(i)) == 5) {
      gen_bs_p4.push_back(tas::genps_p4().at(i));
      gen_bs_mother_id.push_back(tas::genps_id_simplemother().at(i));
      gen_bs_grandma_id.push_back(tas::genps_id_simplegrandma().at(i));

      int genjet_idx = -1;
      float min_dr = 9999999.;
      for (uint j=0; j<tas::genjets_p4NoMuNoNu().size(); j++) {
	float DR = ROOT::Math::VectorUtil::DeltaR(tas::genjets_p4NoMuNoNu().at(j), tas::genps_p4().at(i));
	if (DR < min_dr) {
	  min_dr = DR;
	  genjet_idx = j;
	}
      }
      gen_bs_closest_genjet.push_back(genjet_idx);
      gen_bs_n = gen_bs_p4.size();
    }
	
      
    if (tas::genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(tas::genps_id().at(i)) == 11) {
      gen_els_p4.push_back(tas::genps_p4().at(i));
      gen_els_mother_id.push_back(tas::genps_id_simplemother().at(i));
      gen_els_grandma_id.push_back(tas::genps_id_simplegrandma().at(i));
	
      int reco_idx = -1;
      float min_dr = 9999999.;
      for (uint j=0; j<tas::els_p4().size(); j++) {
	float DR = ROOT::Math::VectorUtil::DeltaR(tas::els_p4().at(j), tas::genps_p4().at(i));
	if (DR < min_dr) {
	  min_dr = DR;
	  reco_idx = j;
	}
      }
      if (min_dr > 0.1) reco_idx = -1;
	
      if (reco_idx > -1) {
	gen_els_reco_p4.push_back(tas::els_p4().at(reco_idx));
	gen_els_reco_id.push_back(isTightElectronPOGspring15noIso_v1(reco_idx));
	gen_els_reco_iso.push_back(isTightElectronPOGspring15_v1(reco_idx));
      } else {
	gen_els_reco_p4.push_back(LorentzVector(0,0,0,0));
	gen_els_reco_id.push_back(false);
	gen_els_reco_iso.push_back(false);
      }	  
      gen_els_n = gen_els_p4.size();
    }
      
    else if (tas::genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(tas::genps_id().at(i)) == 13) {
      gen_mus_p4.push_back(tas::genps_p4().at(i));
      gen_mus_mother_id.push_back(tas::genps_id_simplemother().at(i));
      gen_mus_grandma_id.push_back(tas::genps_id_simplegrandma().at(i));

      int reco_idx = -1;
      float min_dr = 9999999.;
      for (uint j=0; j<tas::mus_p4().size(); j++) {
	float DR = ROOT::Math::VectorUtil::DeltaR(tas::mus_p4().at(j), tas::genps_p4().at(i));
	if (DR < min_dr) {
	  min_dr = DR;
	  reco_idx = j;
	}
      }
      if (min_dr > 0.1) reco_idx = -1;
	
      if (reco_idx > -1) {
	gen_mus_reco_p4.push_back(tas::mus_p4().at(reco_idx));
	gen_mus_reco_id.push_back(isTightMuonPOG(reco_idx) && TMath::Abs(mus_dxyPV().at(reco_idx)) < 0.05 && TMath::Abs(mus_dzPV().at(reco_idx)) < 0.1);
	gen_mus_reco_iso.push_back(isTightMuonPOG(reco_idx) && TMath::Abs(mus_dxyPV().at(reco_idx)) < 0.05 && TMath::Abs(mus_dzPV().at(reco_idx)) < 0.1 && muRelIso04DB(reco_idx) > 0.12);
      } else {
	gen_mus_reco_p4.push_back(LorentzVector(0,0,0,0));
	gen_mus_reco_id.push_back(false);
	gen_mus_reco_iso.push_back(false);
      }	  
      gen_mus_n = gen_mus_p4.size();
    }
      
    else if (tas::genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(tas::genps_id().at(i)) == 15) {
	
      gen_taus_p4.push_back(tas::genps_p4().at(i));
      gen_taus_mother_id.push_back(tas::genps_id_simplemother().at(i));
      gen_taus_grandma_id.push_back(tas::genps_id_simplegrandma().at(i));
	
      int lastcopy_idx = i;
      bool isLastCopy = tas::genps_isLastCopy().at(i);
      while (!isLastCopy) {
	for (int j=0; j<tas::genps_id().size(); j++) {
	  if (tas::genps_id().at(j) == tas::genps_id().at(lastcopy_idx) && tas::genps_idx_simplemother().at(j) == lastcopy_idx) {
	    lastcopy_idx = j;
	    isLastCopy = tas::genps_isLastCopy().at(j);
	    break;
	  }
	}
      }
	
      int thisDecay = 0;
      for (int j=0; j<tas::genps_id().size(); j++) {
	if (!tas::genps_isDirectHardProcessTauDecayProductFinalState().at(j)) continue;
	if (tas::genps_idx_simplemother().at(j) != lastcopy_idx) continue;
	if (tas::genps_charge().at(j) == 0) continue;
	if (TMath::Abs(tas::genps_id().at(j)) == 11 || TMath::Abs(tas::genps_id().at(j)) == 13) {
	  thisDecay = tas::genps_id().at(j);
	  break;
	} else {
	  thisDecay++;
	}
      }
      gen_taus_decaymode.push_back(thisDecay);
      gen_taus_n = gen_taus_p4.size();
    }
  }
  return;
}

void babyMaker::FillJetInformation(int JECvar) {

  vector<LorentzVector> corrJets;
  for (size_t iJet=0; iJet < pfjets_p4().size(); iJet++) {
    
    LorentzVector pfjet_p4_uncor = pfjets_p4().at(iJet) * tas::pfjets_undoJEC().at(iJet);

    jetCorrector->setRho   ( evt_fixgridfastjet_all_rho() );
    jetCorrector->setJetA  ( pfjets_area().at(iJet)       );
    jetCorrector->setJetPt ( pfjet_p4_uncor.pt()               );
    jetCorrector->setJetEta( pfjet_p4_uncor.eta()              );
    double corr = jetCorrector->getCorrection();

    // check for negative correction
    if (corr < 0. && fabs(pfjet_p4_uncor.eta()) < 4.7) {
      std::cout << "BabyMaker: WARNING: negative jet correction: " << corr
		<< ", raw jet pt: " << pfjet_p4_uncor.pt() << ", eta: " << pfjet_p4_uncor.eta() << std::endl;
    }

    double unc = 0;
    if ((JECvar != 0) && pfjet_p4_uncor.pt()*corr > 0. && fabs(pfjet_p4_uncor.eta()) < 5.4) {
      jetCorrector_unc->setJetEta(pfjet_p4_uncor.eta());
      jetCorrector_unc->setJetPt(pfjet_p4_uncor.pt() * corr);
      unc = jetCorrector_unc->getUncertainty(true);
    }
    double var = (1. + JECvar * unc);
    corrJets.push_back(pfjet_p4_uncor*corr*var);

  }
  
  pair<float, float> metCorrected;
  if (JECvar)
    metCorrected = getT1CHSMET_fromMINIAOD(jetCorrector, jetCorrector_unc, ((bool)(JECvar==1)));
  else 
    metCorrected = getT1CHSMET_fromMINIAOD(jetCorrector);
  met = metCorrected.first;
  metPhi = metCorrected.second;
  
  met_raw    = tas::evt_pfmet_raw();
  metPhi_raw = tas::evt_pfmetPhi_raw();

  metStruct trackerMET_ = trackerMET(0.1);
  met_track = trackerMET_.met;
  metPhi_track = trackerMET_.metphi;

  set = evt_pfsumet() - lep1_p4.pt() - lep2_p4.pt();  

  pair<float, float> met3p0 = MET3p0();
  met30 = met3p0.first;
  metPhi30 = met3p0.second;

  std::pair <vector <Jet>, vector <Jet> > jet_results = WWJetsCalculator(corrJets, false);

  ht = 0;
  ht20 = 0.;
  ht25 = 0.;
  ht30 = 0.;
  njets20 = 0;
  njets25 = 0;
  njets30 = 0;
  njets35 = 0;
  njets40 = 0;
  nbtags15 = 0; 
  nbtags20 = 0;
  nbtags25 = 0;
  nbtags30 = 0;

  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_CSVv2.push_back(jet_results.first.at(i).CSVv2());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) pileup_jet_id.push_back(jet_results.first.at(i).pileup_jet_id());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc.push_back(jet_results.second.at(i).CSVv2());  

  for (unsigned int i = 0; i < jets.size(); i++) ht += jets.at(i).pt(); 
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 20) ht20 += jet_results.first.at(i).p4().pt();
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 25) ht25 += jet_results.first.at(i).p4().pt();
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 30) ht30 += jet_results.first.at(i).p4().pt();

  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 40.) njets40++;
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 35.) njets35++;
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 30.) njets30++;
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 25.) njets25++;
  for (unsigned int i = 0; i < jet_results.first.size(); i++) if (jet_results.first.at(i).p4().pt() > 20.) njets20++;

  for (unsigned int i = 0; i < jet_results.second.size(); i++) if (jet_results.second.at(i).p4().pt() > 30.) nbtags30++;
  for (unsigned int i = 0; i < jet_results.second.size(); i++) if (jet_results.second.at(i).p4().pt() > 25.) nbtags25++;
  for (unsigned int i = 0; i < jet_results.second.size(); i++) if (jet_results.second.at(i).p4().pt() > 20.) nbtags20++;
  for (unsigned int i = 0; i < jet_results.second.size(); i++) if (jet_results.second.at(i).p4().pt() > 15.) nbtags15++;

  njets = jets.size();
  nbtags = btags.size();

  if (!is_real_data) {
    for (unsigned int i = 0; i < jet_results.first.size(); i++) jet_parton_flavor.push_back(jet_results.first.at(i).parton_flavor());
    for (unsigned int i = 0; i < jet_results.first.size(); i++) jet_hadron_flavor.push_back(jet_results.first.at(i).hadron_flavor());
    for (unsigned int i = 0; i < jet_results.first.size(); i++) genjets.push_back(jet_results.first.at(i).genjet_p4());
    for (unsigned int i = 0; i < jet_results.second.size(); i++) genbtags.push_back(jet_results.second.at(i).genjet_p4());
  }

  return;
}

void babyMaker::FillLepInformation() {

  hyp_result_t best_hyp_info = chooseBestHyp(true, verbose);
  hyp_class = best_hyp_info.hyp_class;

  int best_hyp = best_hyp_info.best_hyp;
  if (hyp_class == 0 || hyp_class == -1) return;

  lep1_p4 = (hyp_ll_p4().at(best_hyp).pt() > hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_p4().at(best_hyp) : hyp_lt_p4().at(best_hyp);
  lep2_p4 = (hyp_ll_p4().at(best_hyp).pt() <= hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_p4().at(best_hyp) : hyp_lt_p4().at(best_hyp);
  dilep_p4 = lep1_p4 + lep2_p4; 

  lep1_id = (hyp_ll_p4().at(best_hyp).pt() > hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_id().at(best_hyp) : hyp_lt_id().at(best_hyp);
  lep2_id = (hyp_ll_p4().at(best_hyp).pt() <= hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_id().at(best_hyp) : hyp_lt_id().at(best_hyp);
  lep1_idx = (hyp_ll_p4().at(best_hyp).pt() > hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_index().at(best_hyp) : hyp_lt_index().at(best_hyp);
  lep2_idx = (hyp_ll_p4().at(best_hyp).pt() <= hyp_lt_p4().at(best_hyp).pt()) ? hyp_ll_index().at(best_hyp) : hyp_lt_index().at(best_hyp);

  hyp_type = tas::hyp_type().at(best_hyp);
  if (hyp_type == 2 && abs(lep1_id) == 13) hyp_type = 1;
  pair <particle_t, int> thirdLepton = getThirdLepton(best_hyp);
  lep3_id = thirdLepton.first.id;
  lep3_idx = thirdLepton.first.idx;
  lep3_p4 = thirdLepton.first.p4;
  lep3_quality = thirdLepton.second;
  lep1_passes_id = isGoodLepton(lep1_id, lep1_idx);
  lep2_passes_id = isGoodLepton(lep2_id, lep2_idx);

  //Trigger
  trigger_double_highpt = ( passUnprescaledHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||    
			    passUnprescaledHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") );
  trigger_single_highpt = ( passUnprescaledHLTTriggerPattern("HLT_IsoMu20_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_IsoTkMu20_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_IsoMu27_v") ||
			    passUnprescaledHLTTriggerPattern("HLT_Ele23_WPLoose_Gsf_v") || 
			    passUnprescaledHLTTriggerPattern("HLT_Ele27_WPLoose_Gsf_v") || 
			    passUnprescaledHLTTriggerPattern("HLT_Ele22_eta2p1_WP75_Gsf_v") );
  
  trigger_double_lowpt = ( passUnprescaledHLTTriggerPattern("HLT_DoubleMu8_Mass8_PFHT300_v") ||
			   passUnprescaledHLTTriggerPattern("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v") ||
			   passUnprescaledHLTTriggerPattern("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v") );

  lep1_trigger_highpt = 1.0;//getHighPtTriggerPrescale(lep1_p4, lep1_idx, lep1_id);
  lep2_trigger_highpt = 1.0;//getHighPtTriggerPrescale(lep2_p4, lep2_idx, lep2_id);
  lep1_trigger_lowpt  = 1.0;//getLowPtTriggerPrescale(lep1_p4, lep1_idx, lep1_id);
  lep2_trigger_lowpt  = 1.0;//getLowPtTriggerPrescale(lep2_p4, lep2_idx, lep2_id);
}


void babyMaker::FillMVAInformation() {

  nGoodVertices_float = ((float) nGoodVertices);
  lep1_met_dphi = fabs(lep1_p4.phi() - metPhi);
  lep1_met_dphi = lep1_met_dphi > TMath::Pi() ? TMath::TwoPi() - lep1_met_dphi : lep1_met_dphi;
  lep2_met_dphi = fabs(lep2_p4.phi() - metPhi);
  lep2_met_dphi = lep2_met_dphi > TMath::Pi() ? TMath::TwoPi() - lep2_met_dphi : lep2_met_dphi;
  dilep_met_dphi = fabs(dilep_p4.phi() - metPhi);
  dilep_met_dphi = dilep_met_dphi > TMath::Pi() ? TMath::TwoPi() - dilep_met_dphi : dilep_met_dphi;
  dilep_jet1_dphi = jets.size() > 0 ? fabs(dilep_p4.phi() - jets[0].phi()) : -999.;
  dilep_jet1_dphi = dilep_jet1_dphi > TMath::Pi() ? TMath::TwoPi() - dilep_jet1_dphi : dilep_jet1_dphi;
  dilep_jet2_dphi = jets.size() > 1 ? fabs(dilep_p4.phi() - jets[1].phi()) : -999.;
  dilep_jet2_dphi = dilep_jet2_dphi > TMath::Pi() ? TMath::TwoPi() - dilep_jet2_dphi : dilep_jet2_dphi;
  jet1_met_dphi = jets.size() > 0 ? fabs(jets[0].phi() - metPhi) : -999.;
  jet1_met_dphi = jet1_met_dphi > TMath::Pi() ? TMath::TwoPi() - jet1_met_dphi : jet1_met_dphi;
  jet2_met_dphi = jets.size() > 1 ? fabs(jets[1].phi() - metPhi) : -999.;
  jet2_met_dphi = jet2_met_dphi > TMath::Pi() ? TMath::TwoPi() - jet2_met_dphi : jet2_met_dphi;
  jet1_jet2_dphi = jets.size() > 1 ? fabs(jets[1].phi() - jets[0].phi()) : -999.;
  jet1_jet2_dphi = jet1_jet2_dphi > TMath::Pi() ? TMath::TwoPi() - jet1_jet2_dphi : jet1_jet2_dphi;
  min_jet_met_dphi = jets.size() > 1 ? TMath::Min(jet1_met_dphi, jet2_met_dphi) : -999.;
  max_jet_met_dphi = jets.size() > 1 ? TMath::Max(jet1_met_dphi, jet2_met_dphi) : -999.;
  min_lep_met_dphi = TMath::Min(lep1_met_dphi, lep2_met_dphi);
  max_lep_met_dphi = TMath::Max(lep1_met_dphi, lep2_met_dphi);
  projmet = min(lep1_met_dphi,lep2_met_dphi) < TMath::Pi()/2. ?
    met * sin(min(lep1_met_dphi,lep2_met_dphi)) : met;
  projmet_track = min(lep1_met_dphi,lep2_met_dphi) < TMath::Pi()/2. ?
    met_track * sin(min(lep1_met_dphi,lep2_met_dphi)) : met_track;
  min_projmet = min(projmet_track,projmet);
  metsig = met/sqrt(set);
  if (ht30 > 0) {
    metsig30 = met/sqrt(ht30);  
  } else {
    metsig30 = 0.;
  }
  dilep_pt = dilep_p4.pt();
  lep1_mt = sqrt(2*lep1_p4.pt()*met*(1-cos(lep1_met_dphi)));
  lep2_mt = sqrt(2*lep2_p4.pt()*met*(1-cos(lep2_met_dphi)));
  jet1_pt = jets.size() > 0 ? jets[0].pt() : -999.;
  jet2_pt = jets.size() > 1 ? jets[1].pt() : -999.;
  ut = sqrt((met*cos(metPhi)+dilep_p4.px())*(met*cos(metPhi)+dilep_p4.px())
	    +(met*sin(metPhi)+dilep_p4.py())*(met*sin(metPhi)+dilep_p4.py()));
  min_mt = TMath::Min(lep1_mt, lep2_mt);
  max_mt = TMath::Max(lep1_mt, lep2_mt);

  // upara and uperp
  TVector2 metv(met*cos(metPhi), met*sin(metPhi));
  TVector2 dilv(dilep_p4.px(), dilep_p4.py());
  TVector2 utv = -1.*(metv+dilv);
  double phi = utv.DeltaPhi(dilv);
  upara = ut*TMath::Cos(phi);
  uperp = ut*TMath::Sin(phi);


  for(int i=0; i<6; i++) {
    dymva.push_back(MVAs[i]->EvaluateMVA("BDT"));
  }

  return;
}

void babyMaker::FillSMuInformation() {

  nVetoSoftMuons = 0;
  for (unsigned int muidx = 0; muidx < mus_p4().size(); muidx++){    
    if (abs(lep1_id) == 13 && ((uint) lep1_idx) == muidx) continue;
    if (abs(lep2_id) == 13 && ((uint) lep2_idx) == muidx) continue;
    if (PassSoftMuonCut(muidx)) nVetoSoftMuons++;
  }

  for (unsigned int index = 0; index < mus_p4().size(); index++){
    if (mus_p4().at(index).pt() < 3) continue;
    if (!PassSoftMuonCut(index)) continue;
    if (abs(lep1_id) == 13 && ((uint) lep1_idx) == index) continue;
    if (abs(lep2_id) == 13 && ((uint) lep2_idx) == index) continue;
    muID_dxyPV     .push_back(fabs(mus_dxyPV().at(index)));
    muID_dzPV      .push_back(fabs(mus_dzPV().at(index)));
    muID_type      .push_back(mus_type().at(index));
    muID_nlayers   .push_back(mus_nlayers().at(index));
    muID_ptSig     .push_back(mus_ptErr().at(index)/mus_trk_p4().at(index).pt());
    muID_ip3dSig   .push_back(fabs(mus_ip3d().at(index))/mus_ip3derr().at(index));
    muID_medMuonPOG.push_back(isMediumMuonPOG(index));
    muID_SoftMuon  .push_back(PassSoftMuonCut(index));
    muID_p4        .push_back(mus_p4().at(index));
  }
  return;
}
