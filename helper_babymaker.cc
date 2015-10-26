#include "helper_babymaker.h"

using namespace tas;
using namespace WWAnalysis;

//Main functions
babyMaker::babyMaker(FactorizedJetCorrector* jetCorrector_pf_, FactorizedJetCorrector* jetCorrector_puppi_, bool debug) {
  jetCorrector = jetCorrector_pf_;
  jetCorrector_puppi = jetCorrector_puppi_;
  path = ".";
  verbose = debug;
  scalePileupHist = nullptr;
  evt_cut = 0;
}

void babyMaker::SetPileupHist(const char* pileup_file) {
  TFile* input = TFile::Open(pileup_file);
  scalePileupHist = (TH1D*) ((TH1D*) input->Get("pileup"))->Clone();
  scalePileupHist->SetDirectory(0);
  input->Close();
}

void babyMaker::MakeBabyNtuple(const char* output_name){
  
  //Create Baby
  BabyFile = new TFile(Form("%s/%s.root", path.Data(), output_name), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "WW2015 Baby Ntuple");

  //Define Branches
  BabyTree->Branch("met"                   , &met                   );
  BabyTree->Branch("metPhi"                , &metPhi                );
  BabyTree->Branch("met30"                 , &met30                 );
  BabyTree->Branch("metPhi30"              , &metPhi30              );
  BabyTree->Branch("met_raw"               , &met_raw               );
  BabyTree->Branch("metPhi_raw"            , &metPhi_raw            );
  BabyTree->Branch("met_jettoolbox"        , &met_jettoolbox        );
  BabyTree->Branch("metPhi_jettoolbox"     , &metPhi_jettoolbox     );
  BabyTree->Branch("met_track"             , &met_track             );
  BabyTree->Branch("metPhi_track"          , &metPhi_track          );
  BabyTree->Branch("met_track_puppi"       , &met_track_puppi       );
  BabyTree->Branch("metPhi_track_puppi"    , &metPhi_track_puppi    );
  BabyTree->Branch("met_puppi"             , &met_puppi             );
  BabyTree->Branch("metPhi_puppi"          , &metPhi_puppi          );
  BabyTree->Branch("set"                   , &set                   );
  BabyTree->Branch("set_puppi"             , &set_puppi             );
  BabyTree->Branch("set30"                 , &set30                 );

  BabyTree->Branch("event"                 , &event                 );
  BabyTree->Branch("lumi"                  , &lumi                  );
  BabyTree->Branch("run"                   , &run                   );

  BabyTree->Branch("filt_csc"              , &filt_csc              );
  BabyTree->Branch("filt_hbhe"             , &filt_hbhe             );
  BabyTree->Branch("filt_hcallaser"        , &filt_hcallaser        );
  BabyTree->Branch("filt_ecaltp"           , &filt_ecaltp           );
  BabyTree->Branch("filt_trkfail"          , &filt_trkfail          );
  BabyTree->Branch("filt_eebadsc"          , &filt_eebadsc          );

  BabyTree->Branch("is_real_data"          , &is_real_data          );
  BabyTree->Branch("scale1fb"              , &scale1fb              );
  BabyTree->Branch("scalePileup"           , &scalePileup           );
  BabyTree->Branch("xsec"                  , &xsec                  );
  BabyTree->Branch("kfactor"               , &kfactor               );

  BabyTree->Branch("gen_met"               , &gen_met               );
  BabyTree->Branch("gen_met_phi"           , &gen_met_phi           );
  BabyTree->Branch("gen_ht"                , &gen_ht                );

  BabyTree->Branch("genjets"               , &genjets               );
  BabyTree->Branch("genbtags"              , &genbtags              );
  BabyTree->Branch("pileup_jet_id"         , &pileup_jet_id         );   
  BabyTree->Branch("jet_parton_flavor"     , &jet_parton_flavor     );   
  BabyTree->Branch("jet_hadron_flavor"     , &jet_hadron_flavor     );   
  BabyTree->Branch("njets"                 , &njets                 );
  BabyTree->Branch("njets20"               , &njets20               );
  BabyTree->Branch("njets25"               , &njets25               );
  BabyTree->Branch("njets30"               , &njets30               );
  BabyTree->Branch("njets35"               , &njets35               );
  BabyTree->Branch("njets40"               , &njets40               );
  BabyTree->Branch("ht"                    , &ht                    );
  BabyTree->Branch("ht20"                  , &ht20                  );
  BabyTree->Branch("ht25"                  , &ht25                  );
  BabyTree->Branch("ht30"                  , &ht30                  );
  BabyTree->Branch("jets"                  , &jets                  );
  BabyTree->Branch("jets_CSVv2"            , &jets_CSVv2            );
  BabyTree->Branch("jets_CSVsm"            , &jets_CSVsm            );
  BabyTree->Branch("jets_CSVse"            , &jets_CSVse            );
  BabyTree->Branch("jets_CSVtche"          , &jets_CSVtche          );
  BabyTree->Branch("btags_disc"            , &btags_disc            );
  BabyTree->Branch("jets_disc"             , &jets_disc             );
  BabyTree->Branch("btags"                 , &btags                 );
  BabyTree->Branch("nbtags"                , &nbtags                );
  BabyTree->Branch("nbtags15"              , &nbtags15              );
  BabyTree->Branch("nbtags20"              , &nbtags20              );
  BabyTree->Branch("nbtags25"              , &nbtags25              );
  BabyTree->Branch("nbtags30"              , &nbtags30              );

  BabyTree->Branch("genjets_puppi"         , &genjets_puppi         );
  BabyTree->Branch("genbtags_puppi"        , &genbtags_puppi        );
  BabyTree->Branch("pileup_jet_id_puppi"   , &pileup_jet_id_puppi   );   
  BabyTree->Branch("jet_parton_flavor_puppi", &jet_parton_flavor_puppi);   
  BabyTree->Branch("jet_hadron_flavor_puppi", &jet_hadron_flavor_puppi);   
  BabyTree->Branch("njets_puppi"           , &njets_puppi           );
  BabyTree->Branch("njets20_puppi"         , &njets20_puppi         );
  BabyTree->Branch("njets25_puppi"         , &njets25_puppi         );
  BabyTree->Branch("njets30_puppi"         , &njets30_puppi         );
  BabyTree->Branch("njets35_puppi"         , &njets35_puppi         );
  BabyTree->Branch("njets40_puppi"         , &njets40_puppi         );
  BabyTree->Branch("ht_puppi"              , &ht_puppi              );
  BabyTree->Branch("ht20_puppi"            , &ht20_puppi            );
  BabyTree->Branch("ht25_puppi"            , &ht25_puppi            );
  BabyTree->Branch("ht30_puppi"            , &ht30_puppi            );
  BabyTree->Branch("jets_puppi"            , &jets_puppi            );
  BabyTree->Branch("jets_CSVv2_puppi"      , &jets_CSVv2_puppi      );
  BabyTree->Branch("jets_CSVsm_puppi"      , &jets_CSVsm_puppi      );
  BabyTree->Branch("jets_CSVse_puppi"      , &jets_CSVse_puppi      );
  BabyTree->Branch("jets_CSVtche_puppi"    , &jets_CSVtche_puppi    );
  BabyTree->Branch("btags_disc_puppi"      , &btags_disc_puppi      );
  BabyTree->Branch("jets_disc_puppi"       , &jets_disc_puppi       );
  BabyTree->Branch("btags_puppi"           , &btags_puppi           );
  BabyTree->Branch("nbtags_puppi"          , &nbtags_puppi          );
  BabyTree->Branch("nbtags15_puppi"        , &nbtags15_puppi        );
  BabyTree->Branch("nbtags20_puppi"        , &nbtags20_puppi        );
  BabyTree->Branch("nbtags25_puppi"        , &nbtags25_puppi        );
  BabyTree->Branch("nbtags30_puppi"        , &nbtags30_puppi        );

  BabyTree->Branch("hyp_class"             , &hyp_class             );
  BabyTree->Branch("hyp_type"              , &hyp_type              );

  BabyTree->Branch("lep1_p4"               , &lep1_p4               );
  BabyTree->Branch("lep2_p4"               , &lep2_p4               );
  BabyTree->Branch("lep1_motherID"         , &lep1_motherID         );
  BabyTree->Branch("lep2_motherID"         , &lep2_motherID         );
  BabyTree->Branch("lep1_mc_id"            , &lep1_mc_id            );
  BabyTree->Branch("lep2_mc_id"            , &lep2_mc_id            );
  BabyTree->Branch("lep1_id"               , &lep1_id               );
  BabyTree->Branch("lep2_id"               , &lep2_id               );
  BabyTree->Branch("lep1_coneCorrPt"       , &lep1_coneCorrPt       );
  BabyTree->Branch("lep2_coneCorrPt"       , &lep2_coneCorrPt       );
  BabyTree->Branch("lep1_idx"              , &lep1_idx              );
  BabyTree->Branch("lep2_idx"              , &lep2_idx              );
  BabyTree->Branch("lep1_MVA"              , &lep1_MVA              );
  BabyTree->Branch("lep2_MVA"              , &lep2_MVA              );  
  BabyTree->Branch("lep1_trigger_highpt"   , &lep1_trigger_highpt   );
  BabyTree->Branch("lep2_trigger_highpt"   , &lep2_trigger_highpt   );
  BabyTree->Branch("lep1_trigger_lowpt"    , &lep1_trigger_lowpt    );
  BabyTree->Branch("lep2_trigger_lowpt"    , &lep2_trigger_lowpt    );
  BabyTree->Branch("trigger_double_highpt" , &trigger_double_highpt );
  BabyTree->Branch("trigger_single_highpt" , &trigger_single_highpt );
  BabyTree->Branch("trigger_double_lowpt"  , &trigger_double_lowpt  );  

  BabyTree->Branch("sf_dilep_eff"          , &sf_dilep_eff          );

  BabyTree->Branch("mt"                    , &mt                    );
  BabyTree->Branch("mt_l2"                 , &mt_l2                 );
  BabyTree->Branch("mt2"                   , &mt2                   );
  BabyTree->Branch("mt2lblb_A"             , &mt2lblb_A             );
  BabyTree->Branch("mt2lblb_B"             , &mt2lblb_B             );
  BabyTree->Branch("mt2lblb_min"           , &mt2lblb_min           );
  BabyTree->Branch("mt2bb"                 , &mt2bb                 );
  BabyTree->Branch("mtmin"                 , &mtmin                 );

  BabyTree->Branch("mStop"                 , &mStop                 );
  BabyTree->Branch("mGluino"               , &mGluino               );
  BabyTree->Branch("mLSP"                  , &mLSP                  );
  BabyTree->Branch("mSbottom"              , &mSbottom              );
  BabyTree->Branch("mChargino"             , &mChargino             );

  BabyTree->Branch("lep1_id_gen"           , &lep1_id_gen           );
  BabyTree->Branch("lep2_id_gen"           , &lep2_id_gen           );
  BabyTree->Branch("lep1_p4_gen"           , &lep1_p4_gen           );
  BabyTree->Branch("lep2_p4_gen"           , &lep2_p4_gen           );
  BabyTree->Branch("lep3_id"               , &lep3_id               );
  BabyTree->Branch("lep3_idx"              , &lep3_idx              );
  BabyTree->Branch("lep3_p4"               , &lep3_p4               );
  BabyTree->Branch("lep3_quality"          , &lep3_quality          );
  BabyTree->Branch("lep1_iso"              , &lep1_iso              );
  BabyTree->Branch("lep2_iso"              , &lep2_iso              );
  BabyTree->Branch("dilep_p4"              , &dilep_p4              );
  BabyTree->Branch("genps_genjets_p4"      , &genps_genjets_p4      );
  BabyTree->Branch("genps_recojets_p4"     , &genps_recojets_p4     );
  BabyTree->Branch("genps_recodisc"        , &genps_recodisc        );
  BabyTree->Branch("genps_p4"              , &genps_p4              );
  BabyTree->Branch("genps_id"              , &genps_id              );
  BabyTree->Branch("genps_id_mother"       , &genps_id_mother       );
  BabyTree->Branch("genps_status"          , &genps_status          );
  BabyTree->Branch("genps_id_grandma"      , &genps_id_grandma      );
  BabyTree->Branch("lep1_passes_id"        , &lep1_passes_id        );
  BabyTree->Branch("lep2_passes_id"        , &lep2_passes_id        );
  BabyTree->Branch("lep1_dxyPV"            , &lep1_dxyPV            );
  BabyTree->Branch("lep2_dxyPV"            , &lep2_dxyPV            );
  BabyTree->Branch("lep1_dZ"               , &lep1_dZ               );
  BabyTree->Branch("lep2_dZ"               , &lep2_dZ               );
  BabyTree->Branch("lep1_d0_err"           , &lep1_d0_err           );
  BabyTree->Branch("lep2_d0_err"           , &lep2_d0_err           );
  BabyTree->Branch("lep1_ip3d"             , &lep1_ip3d             );
  BabyTree->Branch("lep2_ip3d"             , &lep2_ip3d             );
  BabyTree->Branch("lep1_ip3d_err"         , &lep1_ip3d_err         );
  BabyTree->Branch("lep2_ip3d_err"         , &lep2_ip3d_err         );

  BabyTree->Branch("nVetoElectrons7"       , &nVetoElectrons7       );
  BabyTree->Branch("nVetoElectrons10"      , &nVetoElectrons10      );
  BabyTree->Branch("nVetoElectrons25"      , &nVetoElectrons25      );

  BabyTree->Branch("nVetoSoftMuons"        , &nVetoSoftMuons        );
  BabyTree->Branch("nVetoMuons5"           , &nVetoMuons5           );
  BabyTree->Branch("nVetoMuons10"          , &nVetoMuons10          );
  BabyTree->Branch("nVetoMuons25"          , &nVetoMuons25          );

  //  BabyTree->Branch("filename"              , &filename              );
  //  BabyTree->Branch("lep1_ptrel_v0"         , &lep1_ptrel_v0         );
  //  BabyTree->Branch("lep1_ptrel_v1"         , &lep1_ptrel_v1         );
  //  BabyTree->Branch("lep2_ptrel_v0"         , &lep2_ptrel_v0         );
  //  BabyTree->Branch("lep2_ptrel_v1"         , &lep2_ptrel_v1         );

  BabyTree->Branch("lep1_miniIso"          , &lep1_miniIso          );
  BabyTree->Branch("lep2_miniIso"          , &lep2_miniIso          );
  BabyTree->Branch("jet_close_lep1"        , &jet_close_lep1        );
  BabyTree->Branch("jet_close_lep2"        , &jet_close_lep2        );
  
  //  BabyTree->Branch("eleID_MVA"             , &eleID_MVA             );
  //  BabyTree->Branch("eleID_kfhits"          , &eleID_kfhits          );
  //  BabyTree->Branch("eleID_oldsigmaietaieta", &eleID_oldsigmaietaieta);
  //  BabyTree->Branch("eleID_oldsigmaiphiiphi", &eleID_oldsigmaiphiiphi);
  //  BabyTree->Branch("eleID_oldcircularity"  , &eleID_oldcircularity  );
  //  BabyTree->Branch("eleID_oldr9"           , &eleID_oldr9           );
  //  BabyTree->Branch("eleID_scletawidth"     , &eleID_scletawidth     );
  //  BabyTree->Branch("eleID_sclphiwidth"     , &eleID_sclphiwidth     );
  //  BabyTree->Branch("eleID_he"              , &eleID_he              );
  //  BabyTree->Branch("eleID_psEoverEraw"     , &eleID_psEoverEraw     );
  //  BabyTree->Branch("eleID_kfchi2"          , &eleID_kfchi2          );
  //  BabyTree->Branch("eleID_chi2_hits"       , &eleID_chi2_hits       );
  //  BabyTree->Branch("eleID_fbrem"           , &eleID_fbrem           );
  //  BabyTree->Branch("eleID_ep"              , &eleID_ep              );
  //  BabyTree->Branch("eleID_eelepout"        , &eleID_eelepout        );
  //  BabyTree->Branch("eleID_IoEmIop"         , &eleID_IoEmIop         );
  //  BabyTree->Branch("eleID_deltaetain"      , &eleID_deltaetain      );
  //  BabyTree->Branch("eleID_deltaphiin"      , &eleID_deltaphiin      );
  //  BabyTree->Branch("eleID_deltaetaseed"    , &eleID_deltaetaseed    );
  //  BabyTree->Branch("eleID_pT"              , &eleID_pT              );
  //  BabyTree->Branch("eleID_isbarrel"        , &eleID_isbarrel        );
  //  BabyTree->Branch("eleID_isendcap"        , &eleID_isendcap        );
  //  BabyTree->Branch("eleID_scl_eta"         , &eleID_scl_eta         );
  //  BabyTree->Branch("eleID_gsfhits"         , &eleID_gsfhits         );
  //  BabyTree->Branch("eleID_expectedMissingInnerHits", &eleID_expectedMissingInnerHits);
  //  BabyTree->Branch("eleID_convVtxFitProbability"   , &eleID_convVtxFitProbability   );

  BabyTree->Branch("muID_dxyPV"            , &muID_dxyPV            );
  BabyTree->Branch("muID_dzPV"             , &muID_dzPV             );
  BabyTree->Branch("muID_type"             , &muID_type             );
  BabyTree->Branch("muID_nlayers"          , &muID_nlayers          );
  BabyTree->Branch("muID_ptSig"            , &muID_ptSig            );
  BabyTree->Branch("muID_ip3dSig"          , &muID_ip3dSig          );
  BabyTree->Branch("muID_medMuonPOG"       , &muID_medMuonPOG       );
  BabyTree->Branch("muID_SoftMuon"         , &muID_SoftMuon         );
  BabyTree->Branch("muID_pt"               , &muID_pt               );
  BabyTree->Branch("muID_eta"              , &muID_eta              );
  //  
  BabyTree->Branch("trueNumInt"            , &trueNumInt            );
  BabyTree->Branch("nPUvertices"           , &nPUvertices           ); 
  BabyTree->Branch("nGoodVertices"         , &nGoodVertices         ); 
  
  //InSituFR
  BabyTree->Branch("lep1_isGoodLeg"         , &lep1_isGoodLeg         );
  BabyTree->Branch("lep2_isGoodLeg"         , &lep2_isGoodLeg         );
  BabyTree->Branch("lep1_isFakeLeg"         , &lep1_isFakeLeg         );
  BabyTree->Branch("lep2_isFakeLeg"         , &lep2_isFakeLeg         );
  BabyTree->Branch("truth_inSituFR"         , &truth_inSituFR         );
  BabyTree->Branch("lep1_multiIso"          , &lep1_multiIso          );
  BabyTree->Branch("lep2_multiIso"          , &lep2_multiIso          );
  BabyTree->Branch("lep1_sip"               , &lep1_sip               );
  BabyTree->Branch("lep2_sip"               , &lep2_sip               );
  BabyTree->Branch("lep1_closeJet"          , &lep1_closeJet          );
  BabyTree->Branch("lep2_closeJet"          , &lep2_closeJet          );
  BabyTree->Branch("passed_id_inSituFR_lep1", &passed_id_inSituFR_lep1); 
  BabyTree->Branch("passed_id_inSituFR_lep2", &passed_id_inSituFR_lep2); 

  //To study the DY background
  BabyTree->Branch("projmet"               , &projmet               );
  BabyTree->Branch("projmet_track"         , &projmet_track         );
  BabyTree->Branch("min_projmet"           , &min_projmet           );
  BabyTree->Branch("projmet_puppi"         , &projmet_puppi         );
  BabyTree->Branch("projmet_track_puppi"   , &projmet_track_puppi   );
  BabyTree->Branch("min_projmet_puppi"     , &min_projmet_puppi     );
  BabyTree->Branch("metsig"                , &metsig                );
  BabyTree->Branch("metsig_puppi"          , &metsig_puppi          );
  BabyTree->Branch("metsig30"              , &metsig30              );
  BabyTree->Branch("dilep_pt"              , &dilep_pt              );
  BabyTree->Branch("lep1_mt"               , &lep1_mt               );
  BabyTree->Branch("lep2_mt"               , &lep2_mt               );
  BabyTree->Branch("min_mt"                , &min_mt                );
  BabyTree->Branch("max_mt"                , &max_mt                );
  BabyTree->Branch("jet1_pt"               , &jet1_pt               );
  BabyTree->Branch("jet2_pt"               , &jet2_pt               );
  BabyTree->Branch("ut"                    , &ut                    );
  BabyTree->Branch("upara"                 , &upara                 );
  BabyTree->Branch("uperp"                 , &uperp                 );
  BabyTree->Branch("lep1_met_dphi"         , &lep1_met_dphi         );
  BabyTree->Branch("lep2_met_dphi"         , &lep2_met_dphi         );
  BabyTree->Branch("min_lep_met_dphi"      , &min_lep_met_dphi      );
  BabyTree->Branch("max_lep_met_dphi"      , &max_lep_met_dphi      );
  BabyTree->Branch("dilep_jet1_dphi"       , &dilep_jet1_dphi       );
  BabyTree->Branch("dilep_jet2_dphi"       , &dilep_jet2_dphi       );
  BabyTree->Branch("dilep_met_dphi"        , &dilep_met_dphi        );
  BabyTree->Branch("jet1_met_dphi"         , &jet1_met_dphi         );
  BabyTree->Branch("jet2_met_dphi"         , &jet2_met_dphi         );
  BabyTree->Branch("min_jet_met_dphi"      , &min_jet_met_dphi      );
  BabyTree->Branch("max_jet_met_dphi"      , &max_jet_met_dphi      );
  BabyTree->Branch("jet1_jet2_dphi"        , &jet1_jet2_dphi        );
  
  // The Melia variables 1110.6185
  BabyTree->Branch("costhetaV"             , &costhetaV             );
  BabyTree->Branch("x1V"                   , &x1V                   );
  BabyTree->Branch("x2V"                   , &x2V                   );

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
  
  //Print warning!
  cout << "Careful!! Path is " << path << endl;


}

void babyMaker::InitBabyNtuple(){

    met = -1;
    metPhi = -1;
    met30 = -1;
    metPhi30 = -1;
    met_raw = -1.;
    metPhi_raw = -1.;
    met_jettoolbox = -1.;
    metPhi_jettoolbox = -1.;
    met_track = -1.;
    metPhi_track = -1.;
    met_track_puppi = -1.;
    metPhi_track_puppi = -1.;
    met_puppi = -1.;
    metPhi_puppi = -1.;
    projmet = -1.;
    projmet_track = -1.;
    min_projmet = -1.;
    projmet_puppi = -1.;
    projmet_track_puppi = -1.;
    min_projmet_puppi = -1.;
    event = -1;
    lumi = -1;
    run = -1;
    filt_csc = 0;
    filt_hbhe = 0;
    filt_hcallaser = 0;
    filt_ecaltp = 0;
    filt_trkfail = 0;
    filt_eebadsc = 0;
    is_real_data = 0;
    scale1fb = -1;
    xsec = -1;
    kfactor = -1;
    gen_met = -1;
    gen_met_phi = -1;
    gen_ht = -1;
    hyp_class = -1;
    set = -1;
    set_puppi = -1;
    set30 = -1;

    ht = -1;
    ht20 = -1;
    ht25 = -1;
    ht30 = -1;
    njets = -1;
    njets20 = -1;
    njets25 = -1;
    njets30 = -1;
    njets35 = -1;
    njets40 = -1;
    nbtags = -1;
    nbtags15 = -1;
    nbtags20 = -1;
    nbtags25 = -1;
    nbtags30 = -1;
    jets.clear();
    jets_CSVv2.clear();
    jets_CSVsm.clear();
    jets_CSVse.clear();
    jets_CSVtche.clear();
    genjets.clear();
    btags_disc.clear();
    jets_disc.clear();
    btags.clear();
    genbtags.clear();
    pileup_jet_id.clear();
    jet_parton_flavor.clear();
    jet_hadron_flavor.clear();

    ht_puppi = -1;
    ht20_puppi = -1;
    ht25_puppi = -1;
    ht30_puppi = -1;
    njets_puppi = -1;
    njets20_puppi = -1;
    njets25_puppi = -1;
    njets30_puppi = -1;
    njets35_puppi = -1;
    njets40_puppi = -1;
    nbtags_puppi = -1;
    nbtags15_puppi = -1;
    nbtags20_puppi = -1;
    nbtags25_puppi = -1;
    nbtags30_puppi = -1;
    jets_puppi.clear();
    jets_CSVv2_puppi.clear();
    jets_CSVsm_puppi.clear();
    jets_CSVse_puppi.clear();
    jets_CSVtche_puppi.clear();
    genjets_puppi.clear();
    btags_disc_puppi.clear();
    jets_disc_puppi.clear();
    btags_puppi.clear();
    genbtags_puppi.clear();
    pileup_jet_id_puppi.clear();
    jet_parton_flavor_puppi.clear();
    jet_hadron_flavor_puppi.clear();

    lep1_motherID = 0;
    lep2_motherID = 0;
    lep1_mc_id = -1;
    lep2_mc_id = -1; 
    lep1_id = -1;
    lep2_id = -1;
    lep1_coneCorrPt = -1;
    lep2_coneCorrPt = -1;
    lep1_idx = -1;
    lep2_idx = -1;
    lep1_MVA = -1.;
    lep2_MVA = -1.;
    trigger_double_highpt = -1;
    trigger_single_highpt = -1;
    trigger_double_lowpt = -1;
    lep1_trigger_highpt = -1;
    lep2_trigger_highpt = -1;
    lep1_trigger_lowpt  = -1;
    lep2_trigger_lowpt  = -1;
    hyp_type = -1;
    sf_dilep_eff = -1;
    mt = -1;
    mt_l2 = -1;
    mt2 = -1;
    mtmin = -1;
    mGluino = -1;
    mLSP = -1;
    mStop = -1;
    mSbottom = -1;
    mChargino = -1;
    lep1_id_gen = -1;
    lep2_id_gen = -1;
    lep3_id = -1;
    lep3_idx = -1;
    lep3_quality = -1;
    lep1_iso = -1;
    lep2_iso = -1;
    genps_genjets_p4.clear();
    genps_recojets_p4.clear();
    genps_recodisc.clear();
    genps_p4.clear();
    genps_id.clear();
    genps_id_mother.clear();
    genps_status.clear();
    genps_id_grandma.clear();
    lep1_passes_id = false;
    lep2_passes_id = false;
    lep1_dxyPV = -999998;
    lep2_dxyPV = -999998;
    lep1_dZ = -999998;
    lep2_dZ = -999998;
    lep1_d0_err = -999998;
    lep2_d0_err = -999998;
    lep1_ip3d = -999998;
    lep2_ip3d = -999998;
    lep1_ip3d_err = -999998;
    lep2_ip3d_err = -999998;
    nVetoElectrons7 = 0;
    nVetoElectrons10 = 0;
    nVetoElectrons25 = 0;
    nVetoSoftMuons = 0;
    nVetoMuons5 = 0;
    nVetoMuons10 = 0;
    nVetoMuons25 = 0;
    filename = "";
    lep1_ptrel_v0 = -1;
    lep1_ptrel_v1 = -1;
    lep2_ptrel_v0 = -1;
    lep2_ptrel_v1 = -1;
    lep1_miniIso = -1;
    lep2_miniIso = -1;
    jet_close_lep1 = LorentzVector(0,0,0,0);
    jet_close_lep2 = LorentzVector(0,0,0,0);
    lep1_p4 = LorentzVector(0,0,0,0);
    lep2_p4 = LorentzVector(0,0,0,0);
    lep3_p4 = LorentzVector(0,0,0,0);
    lep1_p4_gen = LorentzVector(0,0,0,0);
    lep2_p4_gen = LorentzVector(0,0,0,0);
    lep1_closeJet = LorentzVector(0,0,0,0);
    lep2_closeJet = LorentzVector(0,0,0,0);
    dilep_p4 = LorentzVector(0,0,0,0);
    /*
    eleID_MVA.clear();
    eleID_kfhits.clear();
    eleID_oldsigmaietaieta.clear();
    eleID_oldsigmaiphiiphi.clear();
    eleID_oldcircularity.clear();
    eleID_oldr9.clear();
    eleID_scletawidth.clear();
    eleID_sclphiwidth.clear();
    eleID_he.clear();
    eleID_psEoverEraw.clear();
    eleID_kfchi2.clear();
    eleID_chi2_hits.clear();
    eleID_fbrem.clear();
    eleID_ep.clear();
    eleID_eelepout.clear();
    eleID_IoEmIop.clear();
    eleID_deltaetain.clear();
    eleID_deltaphiin.clear();
    eleID_deltaetaseed.clear();
    eleID_pT.clear();
    eleID_isbarrel.clear();
    eleID_isendcap.clear();
    eleID_scl_eta.clear();
    eleID_gsfhits.clear();
    eleID_expectedMissingInnerHits.clear();
    eleID_convVtxFitProbability.clear();
    */
    muID_dxyPV.clear();      
    muID_dzPV.clear();      
    muID_type.clear();     
    muID_nlayers.clear();     
    muID_ptSig.clear();     
    muID_ip3dSig.clear();
    muID_medMuonPOG.clear();
    muID_SoftMuon.clear();
    muID_pt.clear();        
    muID_eta.clear();
    
    lep1_isGoodLeg = 0; 
    lep2_isGoodLeg = 0; 
    lep1_isFakeLeg = 0; 
    lep2_isFakeLeg = 0; 
    truth_inSituFR = false;
    lep1_multiIso          = 0;
    lep2_multiIso          = 0;
    lep1_sip = -1;
    lep2_sip = -1;
    passed_id_inSituFR_lep1 = 0;
    passed_id_inSituFR_lep2 = 0;
    trueNumInt.clear();
    nPUvertices.clear(); 
    nGoodVertices = 0; 

    gen_els_n = -1.;
    gen_mus_n = -1.;
    gen_taus_n = -1.;
    
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
    
} 

//Main function
int babyMaker::ProcessBaby(string filename_in, double fudge){

  //Manually set expt (FO2 + FO4)
  bool expt = true;

  //Initialize variables
  InitBabyNtuple();
  
  //Local variables
  bool isData = tas::evt_isRealData();

  //Preliminary stuff
  if (tas::hyp_type().size() < 1) return -1;
  if (tas::mus_dxyPV().size() != tas::mus_dzPV().size()) return -1;

  //Number of good vertices
  for (unsigned int i = 0; i < tas::vtxs_ndof().size(); i++){
    if (!isGoodVertex(i)) continue;
    nGoodVertices++;
  }
  if (nGoodVertices < 1) return -1;
  
  //Fill Easy Variables
  filename = filename_in;
  event = tas::evt_event();
  lumi = tas::evt_lumiBlock();
  run = tas::evt_run();
  kfactor = 1;
  is_real_data = tas::evt_isRealData();
  if (!isData) {
    gen_ht = getGenHT(true);
    trueNumInt = tas::puInfo_trueNumInteractions();
    nPUvertices = puInfo_nPUvertices();
    xsec = tas::evt_xsec_incl();
    kfactor = tas::evt_kfactor();
    gen_met = tas::gen_met();
    gen_met_phi = tas::gen_metPhi();
  }

  //Fill data vs. mc variables
  filt_csc = is_real_data ? tas::evt_cscTightHaloId() : 1;
  filt_hbhe = is_real_data ? hbheNoiseFilter_25ns() : 1;
  filt_ecaltp = is_real_data ? tas::filt_ecalTP() : 1;
  filt_eebadsc = is_real_data ? tas::filt_eeBadSc() : 1;

  filt_trkfail = 1;
  filt_hcallaser = 1;

  scale1fb = is_real_data ? 1 : tas::evt_scale1fb() * fudge;

  // Pileup reweighting
  scalePileup = 1.;
  if (!is_real_data) {
    if (scalePileupHist) {
      if (trueNumInt.size() > 1) {
	scalePileup = scalePileupHist->GetBinContent(scalePileupHist->FindBin(trueNumInt[0]));
      }
    }
  }

  //Fill lepton variables
  hyp_result_t best_hyp_info = chooseBestHyp(expt, verbose);
  hyp_class = best_hyp_info.hyp_class;
  int best_hyp = best_hyp_info.best_hyp;
  if (verbose) cout << "chose hyp: " << best_hyp << " of class" << hyp_class << endl;
  if (hyp_class == 0 || hyp_class == -1) return -1;
  lep1_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep2_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep1_id = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep2_id = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep1_idx = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep2_idx = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep1_MVA = abs(lep1_id) == 11 ? getMVAoutput(lep1_idx) : -9999;
  lep2_MVA = abs(lep2_id) == 11 ? getMVAoutput(lep2_idx) : -9999;
  lep1_coneCorrPt = coneCorrPt(lep1_id, lep1_idx);
  lep2_coneCorrPt = coneCorrPt(lep2_id, lep2_idx);
  Lep lep1 = Lep(lep1_id, lep1_idx);
  Lep lep2 = Lep(lep2_id, lep2_idx);
  lep1_dxyPV = lep1.dxyPV();
  lep2_dxyPV = lep2.dxyPV();
  lep1_dZ = lep1.dzPV();
  lep2_dZ = lep2.dzPV();
  lep1_d0_err = lep1.d0Err();
  lep2_d0_err = lep2.d0Err();
  lep1_ip3d = lep1.ip3d();
  lep2_ip3d = lep2.ip3d();
  lep1_ip3d_err = lep1.ip3dErr();
  lep2_ip3d_err = lep2.ip3dErr();
  if (!isData) {
    lep1_motherID = lepMotherID(lep1);
    lep2_motherID = lepMotherID(lep2);
    lep1_mc_id = lep1.mc_id();
    lep2_mc_id = lep2.mc_id();
  }
  hyp_type = tas::hyp_type().at(best_hyp);
  if (hyp_type == 2 && abs(lep1_id) == 13) hyp_type = 1;
  pair <particle_t, int> thirdLepton = getThirdLepton(best_hyp);
  lep3_id = thirdLepton.first.id;
  lep3_idx = thirdLepton.first.idx;
  lep3_p4 = thirdLepton.first.p4;
  lep3_quality = thirdLepton.second;
  lep1_iso = abs(lep1_id) == 11 ? eleRelIso03(lep1_idx, WW) :  muRelIso03(lep1_idx, WW);
  lep2_iso = abs(lep2_id) == 11 ? eleRelIso03(lep2_idx, WW) :  muRelIso03(lep2_idx, WW);
  lep1_multiIso = abs(lep1_id) == 11 ? passMultiIso(11, lep1_idx, 0.10, 0.7, 7.0, 1, 1) : passMultiIso(13, lep1_idx, 0.14, 0.68, 6.7, 1, 1);
  lep2_multiIso = abs(lep2_id) == 11 ? passMultiIso(11, lep2_idx, 0.10, 0.7, 7.0, 1, 1) : passMultiIso(13, lep2_idx, 0.14, 0.68, 6.7, 1, 1);
  lep1_sip = abs(lep1_id) == 11 ? fabs(els_ip3d().at(lep1_idx))/els_ip3derr().at(lep1_idx) : fabs(mus_ip3d().at(lep1_idx))/mus_ip3derr().at(lep1_idx); 
  lep2_sip = abs(lep2_id) == 11 ? fabs(els_ip3d().at(lep2_idx))/els_ip3derr().at(lep2_idx) : fabs(mus_ip3d().at(lep2_idx))/mus_ip3derr().at(lep2_idx); 
  dilep_p4 = lep1_p4 + lep2_p4; 
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

  if (isData) {
    lep1_trigger_highpt = getHighPtTriggerPrescale(lep1_p4, lep1_idx, lep1_id);
    lep2_trigger_highpt = getHighPtTriggerPrescale(lep2_p4, lep2_idx, lep2_id);
    lep1_trigger_lowpt  = getLowPtTriggerPrescale(lep1_p4, lep1_idx, lep1_id);
    lep2_trigger_lowpt  = getLowPtTriggerPrescale(lep2_p4, lep2_idx, lep2_id);
  }
  
  //PtRel for both leptons

  lep1_ptrel_v0 = getPtRel(lep1_id, lep1_idx, false, 2);
  lep2_ptrel_v0 = getPtRel(lep2_id, lep2_idx, false, 2);
  lep1_ptrel_v1 = getPtRel(lep1_id, lep1_idx, true, 2);
  lep2_ptrel_v1 = getPtRel(lep2_id, lep2_idx, true, 2);
  //MiniIso
  lep1_miniIso = abs(lep1_id)==11 ? elMiniRelIsoCMS3_EA(lep1_idx) : muMiniRelIsoCMS3_EA(lep1_idx);
  lep2_miniIso = abs(lep2_id)==11 ? elMiniRelIsoCMS3_EA(lep2_idx) : muMiniRelIsoCMS3_EA(lep2_idx);

  //For inSituFR, both must pass looser ID (easier than selection ID)
  passed_id_inSituFR_lep1 = isInSituFRLepton(lep1_id, lep1_idx, expt); 
  passed_id_inSituFR_lep2 = isInSituFRLepton(lep2_id, lep2_idx, expt); 
  if (passed_id_inSituFR_lep1 && passed_id_inSituFR_lep2){
    int truth_lep1 = lepMotherID_inSituFR( Lep(lep1_id, lep1_idx) ); 
    int truth_lep2 = lepMotherID_inSituFR( Lep(lep2_id, lep2_idx) ); 

    //Need one good leg and one fake leg
    if (truth_lep1 > 0) lep1_isGoodLeg = true;
    else lep1_isGoodLeg = false;
    if (truth_lep1 < 0) lep1_isFakeLeg = true;
    else lep1_isFakeLeg = false;
    if (truth_lep2 > 0) lep2_isGoodLeg = true;
    else lep2_isGoodLeg = false;
    if (truth_lep2 < 0) lep2_isFakeLeg = true;
    else lep2_isFakeLeg = false;

    //Now require one good leg and one fake leg
    if ((lep1_isGoodLeg && lep2_isFakeLeg) || (lep1_isFakeLeg && lep2_isGoodLeg)) truth_inSituFR = true;
    else truth_inSituFR = false; 
 }

  //Closest jet for both leptons
  lep1_closeJet = closestJet(lep1_p4, 0.3, 4.7, 1);
  lep2_closeJet = closestJet(lep2_p4, 0.3, 4.7, 1);
  
  //Fill generated lepton variables, ignoring reco (matching to reco done above)
  if (!isData) {
    vector <particle_t> genPair = getGenPair(verbose);
    if (genPair.size() == 2){
      lep1_id_gen = genPair.at(0).id;
      lep2_id_gen = genPair.at(1).id;
      lep1_p4_gen = genPair.at(0).p4;
      lep2_p4_gen = genPair.at(1).p4;
    }
  }
  
  //Fill all generated particles
  if (!isData){
    genps_p4 = tas::genps_p4();
    genps_id = tas::genps_id();
    genps_id_mother = tas::genps_id_mother();
    genps_status = tas::genps_status(); 
    genps_id_grandma = tas::genps_id_simplegrandma(); 

    genps_genjets_p4 = cms3.genjets_p4NoMuNoNu();

    for (int i=0; i<cms3.genps_id().size(); i++) {
      
      
      if (cms3.genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(cms3.genps_id().at(i)) == 11) {
	gen_els_p4.push_back(cms3.genps_p4().at(i));
	gen_els_mother_id.push_back(cms3.genps_id_simplemother().at(i));
	gen_els_grandma_id.push_back(cms3.genps_id_simplegrandma().at(i));
	
	int reco_idx = -1;
	float min_dr = 9999999.;
	for (uint j=0; j<cms3.els_p4().size(); j++) {
	  float DR = ROOT::Math::VectorUtil::DeltaR(cms3.els_p4().at(j), cms3.genps_p4().at(i));
	  if (DR < min_dr) {
	    min_dr = DR;
	    reco_idx = j;
	  }
	}
	if (min_dr > 0.1) reco_idx = -1;
	
	if (reco_idx > -1) {
	  gen_els_reco_p4.push_back(cms3.els_p4().at(reco_idx));
	  gen_els_reco_id.push_back(isTightElectronPOGspring15noIso_v1(reco_idx));
	  gen_els_reco_iso.push_back(isTightElectronPOGspring15_v1(reco_idx));
	} else {
	  gen_els_reco_p4.push_back(LorentzVector(0,0,0,0));
	  gen_els_reco_id.push_back(false);
	  gen_els_reco_iso.push_back(false);
	}	  
	gen_els_n = gen_els_p4.size();
      }
      
      else if (cms3.genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(cms3.genps_id().at(i)) == 13) {
	gen_mus_p4.push_back(cms3.genps_p4().at(i));
	gen_mus_mother_id.push_back(cms3.genps_id_simplemother().at(i));
	gen_mus_grandma_id.push_back(cms3.genps_id_simplegrandma().at(i));

	int reco_idx = -1;
	float min_dr = 9999999.;
	for (uint j=0; j<cms3.mus_p4().size(); j++) {
	  float DR = ROOT::Math::VectorUtil::DeltaR(cms3.mus_p4().at(j), cms3.genps_p4().at(i));
	    if (DR < min_dr) {
	      min_dr = DR;
	      reco_idx = j;
	    }
	}
	if (min_dr > 0.1) reco_idx = -1;
	
	if (reco_idx > -1) {
	    gen_mus_reco_p4.push_back(cms3.mus_p4().at(reco_idx));
	    gen_mus_reco_id.push_back(isTightMuonPOG(reco_idx) && TMath::Abs(mus_dxyPV().at(reco_idx)) < 0.05 && TMath::Abs(mus_dzPV().at(reco_idx)) < 0.1);
	    gen_mus_reco_iso.push_back(isTightMuonPOG(reco_idx) && TMath::Abs(mus_dxyPV().at(reco_idx)) < 0.05 && TMath::Abs(mus_dzPV().at(reco_idx)) < 0.1 && muRelIso04DB(reco_idx) > 0.12);
	} else {
	  gen_mus_reco_p4.push_back(LorentzVector(0,0,0,0));
	  gen_mus_reco_id.push_back(false);
	  gen_mus_reco_iso.push_back(false);
	}	  
	gen_mus_n = gen_mus_p4.size();
      }
      
      else if (cms3.genps_fromHardProcessBeforeFSR().at(i) && TMath::Abs(cms3.genps_id().at(i)) == 15) {
	
	gen_taus_p4.push_back(cms3.genps_p4().at(i));
	gen_taus_mother_id.push_back(cms3.genps_id_simplemother().at(i));
	gen_taus_grandma_id.push_back(cms3.genps_id_simplegrandma().at(i));
	
	int lastcopy_idx = i;
	bool isLastCopy = cms3.genps_isLastCopy().at(i);
	while (!isLastCopy) {
	  for (int j=0; j<cms3.genps_id().size(); j++) {
	    if (cms3.genps_id().at(j) == cms3.genps_id().at(lastcopy_idx) && cms3.genps_idx_simplemother().at(j) == lastcopy_idx) {
	      lastcopy_idx = j;
	      isLastCopy = cms3.genps_isLastCopy().at(j);
	      break;
	    }
	  }
	}
	
	int thisDecay = 0;
	for (int j=0; j<cms3.genps_id().size(); j++) {
	  if (!cms3.genps_isDirectHardProcessTauDecayProductFinalState().at(j)) continue;
	  if (cms3.genps_idx_simplemother().at(j) != lastcopy_idx) continue;
	  if (cms3.genps_charge().at(j) == 0) continue;
	  if (TMath::Abs(cms3.genps_id().at(j)) == 11 || TMath::Abs(cms3.genps_id().at(j)) == 13) {
	    thisDecay = cms3.genps_id().at(j);
	    break;
	  } else {
	    thisDecay++;
	  }
	}
	gen_taus_decaymode.push_back(thisDecay);
	gen_taus_n = gen_taus_p4.size();
      }
    }
  }
  
  //Determine and save jet and b-tag variables
  vector<LorentzVector> corrJets;
  for (size_t iJet=0; iJet < tas::pfjets_p4().size(); iJet++) {
    
    LorentzVector pfjet_p4_uncor = tas::pfjets_p4().at(iJet) * cms3.pfjets_undoJEC().at(iJet);

    //    if (pfjet_p4_uncor.pt() < 10) continue;
    //    if (pfjet_p4_uncor.eta() > 9.9) continue;

    jetCorrector->setRho   ( tas::evt_fixgridfastjet_all_rho() );
    jetCorrector->setJetA  ( tas::pfjets_area().at(iJet)       );
    jetCorrector->setJetPt ( pfjet_p4_uncor.pt()               );
    jetCorrector->setJetEta( pfjet_p4_uncor.eta()              );
    double corr = jetCorrector->getCorrection();

    // check for negative correction
    if (corr < 0. && fabs(pfjet_p4_uncor.eta()) < 4.7) {
      std::cout << "BabyMaker: WARNING: negative jet correction: " << corr
		<< ", raw jet pt: " << pfjet_p4_uncor.pt() << ", eta: " << pfjet_p4_uncor.eta() << std::endl;
    }

    // apply new JEC to p4
    corrJets.push_back(pfjet_p4_uncor*corr);

  }

  vector<LorentzVector> corrJets_puppi;
  for (size_t iJet=0; iJet < tas::pfjets_puppi_p4().size(); iJet++) {
    
    LorentzVector pfjet_p4_uncor = tas::pfjets_puppi_p4().at(iJet) * cms3.pfjets_puppi_undoJEC().at(iJet);

    //    if (pfjet_p4_uncor.pt() < 10) continue;
    //    if (pfjet_p4_uncor.eta() > 9.9) continue;

    jetCorrector_puppi->setRho   ( tas::evt_fixgridfastjet_all_rho() );
    jetCorrector_puppi->setJetA  ( tas::pfjets_puppi_area().at(iJet) );
    jetCorrector_puppi->setJetPt ( pfjet_p4_uncor.pt()               );
    jetCorrector_puppi->setJetEta( pfjet_p4_uncor.eta()              );
    double corr = jetCorrector_puppi->getCorrection();

    // check for negative correction
    if (corr < 0. && fabs(pfjet_p4_uncor.eta()) < 4.7) {
      std::cout << "BabyMaker: WARNING: negative jet correction: " << corr
		<< ", raw jet pt: " << pfjet_p4_uncor.pt() << ", eta: " << pfjet_p4_uncor.eta() << std::endl;
    }

    // apply new JEC to p4
    //    cout << pfjet_p4_uncor.pt()*corr << endl;
    corrJets_puppi.push_back(pfjet_p4_uncor*corr);

  }
  genps_recojets_p4 = corrJets;
  genps_recodisc    = cms3.pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag();
  
  pair<float, float> metCorrected_puppi = getT1CHSMET_fromMINIAOD(jetCorrector_puppi);
  met_puppi = metCorrected_puppi.first;
  metPhi_puppi = metCorrected_puppi.second;

  pair<float, float> metCorrected = getT1CHSMET_fromMINIAOD(jetCorrector);
  met = metCorrected.first;
  metPhi = metCorrected.second;

  pair<float, float> metCorrected_jettoolbox = getT1CHSMET(jetCorrector);
  met_jettoolbox = metCorrected_jettoolbox.first;
  metPhi_jettoolbox = metCorrected_jettoolbox.second;
  
  met_raw    = cms3.evt_pfmet_raw();
  metPhi_raw = cms3.evt_pfmetPhi_raw();

  // track MET
  metStruct trackerMET_ = trackerMET(0.1);
  met_track = trackerMET_.met;
  metPhi_track = trackerMET_.metphi;

  metStruct trackerMET_puppi_ = trackerMETpuppi(0.1);
  met_track_puppi = trackerMET_puppi_.met;
  metPhi_track_puppi = trackerMET_puppi_.metphi;

  // SET is uncorrect, and will continue.
  set = evt_pfsumet() - lep1_p4.pt() - lep2_p4.pt();  
  set_puppi = METpuppi().sumet - lep1_p4.pt() - lep2_p4.pt();

  // The bullshit MET30
  pair<float, float> met3p0 = MET3p0();
  met30 = met3p0.first;
  metPhi30 = met3p0.second;


  std::pair <vector <Jet>, vector <Jet> > jet_results = WWJetsCalculator(corrJets, false);
  std::pair <vector <Jet>, vector <Jet> > jet_results_puppi = WWJetsCalculator(corrJets_puppi, true);

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
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_disc.push_back(jet_results.first.at(i).CSVv2());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_CSVv2.push_back(jet_results.first.at(i).CSVv2());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_CSVsm.push_back(jet_results.first.at(i).CSVsm());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_CSVse.push_back(jet_results.first.at(i).CSVse());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_CSVtche.push_back(jet_results.first.at(i).CSVtche());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) pileup_jet_id.push_back(jet_results.first.at(i).pileup_jet_id());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc.push_back(jet_results.second.at(i).CSVv2());  

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

  for (unsigned int i = 0; i < jets.size(); i++) ht += jets.at(i).pt(); 
  njets = jets.size();
  nbtags = btags.size();

  if (!is_real_data) {
    for (unsigned int i = 0; i < jet_results.first.size(); i++) jet_parton_flavor.push_back(jet_results.first.at(i).parton_flavor());
    for (unsigned int i = 0; i < jet_results.first.size(); i++) jet_hadron_flavor.push_back(jet_results.first.at(i).hadron_flavor());
    for (unsigned int i = 0; i < jet_results.first.size(); i++) genjets.push_back(jet_results.first.at(i).genjet_p4());
    for (unsigned int i = 0; i < jet_results.second.size(); i++) genbtags.push_back(jet_results.second.at(i).genjet_p4());
  }

  ht_puppi = 0;
  ht20_puppi = 0.;
  ht25_puppi = 0.;
  ht30_puppi = 0.;
  njets20_puppi = 0;
  njets25_puppi = 0;
  njets30_puppi = 0;
  njets35_puppi = 0;
  njets40_puppi = 0;
  nbtags15_puppi = 0; 
  nbtags20_puppi = 0;
  nbtags25_puppi = 0;
  nbtags30_puppi = 0;

  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_puppi.push_back(jet_results_puppi.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_disc_puppi.push_back(jet_results_puppi.first.at(i).CSVv2());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_CSVv2_puppi.push_back(jet_results_puppi.first.at(i).CSVv2());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_CSVsm_puppi.push_back(jet_results_puppi.first.at(i).CSVsm());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_CSVse_puppi.push_back(jet_results_puppi.first.at(i).CSVse());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jets_CSVtche_puppi.push_back(jet_results_puppi.first.at(i).CSVtche());
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) pileup_jet_id_puppi.push_back(jet_results_puppi.first.at(i).pileup_jet_id());
  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) btags_puppi.push_back(jet_results_puppi.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) btags_disc_puppi.push_back(jet_results_puppi.second.at(i).CSVv2());  

  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 20) ht20 += jet_results_puppi.first.at(i).p4().pt();
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 25) ht25 += jet_results_puppi.first.at(i).p4().pt();
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 30) ht30 += jet_results_puppi.first.at(i).p4().pt();

  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 40.) njets40_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 35.) njets35_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 30.) njets30_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 25.) njets25_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) if (jet_results_puppi.first.at(i).p4().pt() > 20.) njets20_puppi++;

  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) if (jet_results_puppi.second.at(i).p4().pt() > 30.) nbtags30_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) if (jet_results_puppi.second.at(i).p4().pt() > 25.) nbtags25_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) if (jet_results_puppi.second.at(i).p4().pt() > 20.) nbtags20_puppi++;
  for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) if (jet_results_puppi.second.at(i).p4().pt() > 15.) nbtags15_puppi++;

  for (unsigned int i = 0; i < jets_puppi.size(); i++) ht_puppi += jets_puppi.at(i).pt(); 
  njets_puppi = jets_puppi.size();
  nbtags_puppi = btags_puppi.size();

  if (!is_real_data) {
    for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jet_parton_flavor_puppi.push_back(jet_results_puppi.first.at(i).parton_flavor());
    for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) jet_hadron_flavor_puppi.push_back(jet_results_puppi.first.at(i).hadron_flavor());
    for (unsigned int i = 0; i < jet_results_puppi.first.size(); i++) genjets_puppi.push_back(jet_results_puppi.first.at(i).genjet_p4());
    for (unsigned int i = 0; i < jet_results_puppi.second.size(); i++) genbtags_puppi.push_back(jet_results_puppi.second.at(i).genjet_p4());
  }



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
  projmet_puppi = min(lep1_met_dphi,lep2_met_dphi) < TMath::Pi()/2. ?
    met_puppi * sin(min(lep1_met_dphi,lep2_met_dphi)) : met_puppi;
  projmet_track_puppi = min(lep1_met_dphi,lep2_met_dphi) < TMath::Pi()/2. ?
    met_track_puppi * sin(min(lep1_met_dphi,lep2_met_dphi)) : met_track_puppi;
  min_projmet_puppi = min(projmet_track_puppi,projmet_puppi);
  metsig = met/sqrt(set);
  metsig_puppi = met_puppi/sqrt(set_puppi);
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

  // Melia variables
  Boost dilep_beta(-dilep_p4.px()/dilep_p4.e(), -dilep_p4.py()/dilep_p4.e(), -dilep_p4.pz()/dilep_p4.e());
  auto cm_p4 = lep1_id > 0 ? dilep_beta(lep1_p4) : dilep_beta(lep2_p4);
  costhetaV = TMath::Cos(cm_p4.theta());
  x1V = (lep1_p4.e() + lep2_p4.e() + lep1_p4.pz() + lep1_p4.pz())/13000.;
  x2V = (lep1_p4.e() + lep2_p4.e() - lep1_p4.pz() - lep1_p4.pz())/13000.;

  //Closest Jet
  jet_close_lep1 = closestJet(lep1_p4, 0.3, 4.7, 1);
  jet_close_lep2 = closestJet(lep2_p4, 0.3, 4.7, 1);

  //nVeto Leptons
  for (unsigned int eidx = 0; eidx < tas::els_p4().size(); eidx++){

    if (abs(lep1_id) == 11 && ((uint) lep1_idx) == eidx) continue;
    if (abs(lep2_id) == 11 && ((uint) lep2_idx) == eidx) continue;

    if (!isFakableElectron(eidx)) continue;
    if (tas::els_p4().at(eidx).pt() < 7) continue;
    nVetoElectrons7++;
    if (tas::els_p4().at(eidx).pt() < 10) continue;
    nVetoElectrons10++;
    if (tas::els_p4().at(eidx).pt() < 25) continue;
    nVetoElectrons25++;
  }
  for (unsigned int muidx = 0; muidx < tas::mus_p4().size(); muidx++){
    
    if (abs(lep1_id) == 13 && ((uint) lep1_idx) == muidx) continue;
    if (abs(lep2_id) == 13 && ((uint) lep2_idx) == muidx) continue;

    if (PassSoftMuonCut(muidx)) nVetoSoftMuons++;
    if (!isFakableMuon(muidx)) continue;
    if (tas::mus_p4().at(muidx).pt() < 5) continue;
    nVetoMuons5++;
    if (tas::mus_p4().at(muidx).pt() < 10) continue;
    nVetoMuons10++;
    if (tas::mus_p4().at(muidx).pt() < 25) continue;
    nVetoMuons25++;
  }
  
  //MT variables
  mt    = MT(lep1_p4.pt(), lep1_p4.phi(), met, metPhi);
  mt_l2 = MT(lep2_p4.pt(), lep2_p4.phi(), met, metPhi);
  mt2   = MT2(met, metPhi, lep1_p4, lep2_p4);
  mtmin = mt > mt_l2 ? mt_l2 : mt; 
  if (nbtags > 1) {
    mt2lblb_A = MT2(met, metPhi, lep1_p4 + btags.at(0), lep2_p4 + btags.at(1));
    mt2lblb_B = MT2(met, metPhi, lep1_p4 + btags.at(1), lep2_p4 + btags.at(0));
    mt2lblb_min = TMath::Min(mt2lblb_A, mt2lblb_B);
    mt2bb = MT2(met, metPhi, btags.at(0), btags.at(1));    
  }


  //Electron ID variables
  /*
  for (unsigned int index = 0; index < els_fbrem().size(); index++){
    if (!isGoodVetoElectron(index)) continue;
    if (tas::els_p4().at(index).pt() < 7) continue;
    if ((tas::els_p4().at(index).eta()) > 2.5) continue;
    eleID_MVA             .push_back(getMVAoutput(index));
    eleID_kfhits          .push_back(tas::els_ckf_laywithmeas().at(index));
    eleID_oldsigmaietaieta.push_back(tas::els_sigmaIEtaIEta_full5x5().at(index)); 
    eleID_oldsigmaiphiiphi.push_back(tas::els_sigmaIPhiIPhi_full5x5().at(index));
    eleID_oldcircularity  .push_back(1.0 - tas::els_e1x5_full5x5().at(index)/tas::els_e5x5_full5x5().at(index)); 
    eleID_oldr9           .push_back(tas::els_r9_full5x5().at(index)); 
    eleID_scletawidth     .push_back(tas::els_etaSCwidth().at(index));
    eleID_sclphiwidth     .push_back(tas::els_phiSCwidth().at(index));
    eleID_he              .push_back(tas::els_hOverE().at(index));
    eleID_psEoverEraw     .push_back(tas::els_eSCPresh().at(index)/tas::els_eSCRaw().at(index));
    eleID_kfchi2          .push_back(tas::els_ckf_chi2().at(index)/tas::els_ckf_ndof().at(index));
    eleID_chi2_hits       .push_back(tas::els_chi2().at(index)/tas::els_ndof().at(index));
    eleID_fbrem           .push_back(tas::els_fbrem().at(index));
    eleID_ep              .push_back(tas::els_eOverPIn().at(index));
    eleID_eelepout        .push_back(tas::els_eOverPOut().at(index));
    eleID_IoEmIop         .push_back(tas::els_ecalEnergy().at(index) != 0 ? 1.0/tas::els_ecalEnergy().at(index) - tas::els_eOverPIn().at(index)/tas::els_ecalEnergy().at(index) : 999999);
    eleID_deltaetain      .push_back(tas::els_dEtaIn().at(index));
    eleID_deltaphiin      .push_back(tas::els_dPhiIn().at(index));
    eleID_deltaetaseed    .push_back(tas::els_dEtaOut().at(index));
    eleID_pT              .push_back(tas::els_p4().at(index).pt()); 
    eleID_isbarrel        .push_back(fabs(tas::els_etaSC().at(index)) < 1.479 ? 1 : 0); 
    eleID_isendcap        .push_back(fabs(tas::els_etaSC().at(index)) > 1.479 ? 1 : 0); 
    eleID_scl_eta         .push_back(tas::els_etaSC().at(index));
    eleID_gsfhits                 .push_back(tas::els_validHits().at(index));
    eleID_expectedMissingInnerHits.push_back(tas::els_exp_innerlayers().at(index));
    eleID_convVtxFitProbability   .push_back(tas::els_conv_vtx_prob().at(index));

  }
  */
  //Muon ID variables
  for (unsigned int index = 0; index < mus_p4().size(); index++){
    if (tas::mus_p4().at(index).pt() < 3) continue;
    muID_dxyPV     .push_back(fabs(mus_dxyPV().at(index)));
    muID_dzPV      .push_back(fabs(mus_dzPV().at(index)));
    muID_type      .push_back(mus_type().at(index));
    muID_nlayers   .push_back(mus_nlayers().at(index));
    muID_ptSig     .push_back(mus_ptErr().at(index)/mus_trk_p4().at(index).pt());
    muID_ip3dSig   .push_back(fabs(mus_ip3d().at(index))/mus_ip3derr().at(index));
    muID_medMuonPOG.push_back(isMediumMuonPOG(index));
    muID_SoftMuon  .push_back(PassSoftMuonCut(index));
    muID_pt        .push_back(mus_p4().at(index).pt());
    muID_eta       .push_back(fabs(mus_p4().at(index).eta()));
  }
  
  
  //Fill Baby
  BabyTree->Fill();

  return 0;  

}
  
