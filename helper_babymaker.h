#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TF1.h"
#include "TROOT.h"
#include "TVector2.h"

#include "Math/VectorUtil.h" 
#include "Math/Boost.h"

#include "CORE/CMS3.h"

#include "CORE/Tools/utils.h"
#include "CORE/Tools/MT2/MT2.h"

#include "CORE/WWSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/IsolationTools.h"
#include "CORE/TriggerSelections.h"
#include "CORE/MetSelections.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"

#include "resummation/WWpTreweight.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#include <vector>
#include <string>

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
typedef vector<pair<const LorentzVector *, double> > jets_with_corr_t;
typedef ROOT::Math::Boost Boost;

using namespace std;

//Classes
class babyMaker {

 public:
  babyMaker(bool isDataFileFromName, bool debug = 0);
  void MakeBabyNtuple(const char* output_name);
  void InitBabyNtuple();
  void CloseBabyNtuple () { BabyFile->cd(); BabyTree->Write(); BabyFile->Close(); }
  int ProcessBaby(string filename_in, double fudge = 1., int JECvar = 0);
  
  void FillGenInformation(); 
  void FillJetInformation(int JECvar); 
  void FillLepInformation(); 
  void FillMVAInformation(); 
  void FillSMuInformation();

 protected:
  TFile* BabyFile;
  TTree* BabyTree;

 private:

  // Corrections
  FactorizedJetCorrector* jetCorrector;
  JetCorrectionUncertainty* jetCorrector_unc;
  WWpTreweight* resummation;    
  TH1D* scalePileupHist;
  TMVA::Reader *MVA_0jet;
  TMVA::Reader *MVA_1jet;
  TMVA::Reader *MVA_jets;
  TMVA::Reader *MVA_0jet_higgs;
  TMVA::Reader *MVA_1jet_higgs;
  TMVA::Reader *MVA_jets_higgs;
  vector<TMVA::Reader*> MVAs;

  //Switches
  TString path;
  bool verbose;
  unsigned int evt_cut;

  // scales
  float scalePileup;
  float scaleFR;
  float scaleDY;
  float scaleTOP;
  float scaleID;
  float scaleTrigger;
  vector<float> scaleWWpT;
  vector<float> dymva;

  //MET
  float met;
  float metPhi;

  //Meta Variables
  int event;
  int lumi;
  int run;
  bool is_real_data;
  float scale1fb;     
  float xsec;         
  float kfactor;      
  TString filename;

  //Pileup
  vector <float> trueNumInt;
  vector <int> nPUvertices;
  vector <int> bunchCrossing;
  int nGoodVertices; 
  float nGoodVertices_float;

  // gen weights
  vector <float> gen_weights;
    
  //Filters
  bool pass_met_filters;

  //Gen MET 
  float gen_met;      
  float gen_met_phi;  

  // MET3.0
  float met30;
  float metPhi30;
  float set30;
    
  // uncorr MET
  float met_raw;
  float metPhi_raw;

  // track MET
  float met_track;
  float metPhi_track;

  // Gen HT
  float gen_ht;

  //Jets
  vector <LorentzVector> genjets;

  int njets;
  int njets20;
  int njets25;
  int njets30;
  int njets35;
  int njets40;
  float ht;
  float ht20;
  float ht25;
  float ht30;
  float set;
  vector<float> pileup_jet_id;
  vector<int> jet_parton_flavor;
  vector<int> jet_hadron_flavor;
  vector <LorentzVector> jets;
  vector <float> jets_CSVv2;

  // DY studies
  float projmet;
  float projmet_track;
  float min_projmet;
  float metsig;
  float metsig30;
  float dilep_pt;
  float lep1_mt;
  float lep2_mt;
  float jet1_pt;
  float jet2_pt;
  float ut;
  float lep1_met_dphi;
  float lep2_met_dphi;
  float dilep_jet1_dphi;
  float dilep_jet2_dphi;
  float dilep_met_dphi;
  float jet1_met_dphi;
  float jet2_met_dphi;
  float jet1_jet2_dphi;
  float min_jet_met_dphi;
  float max_jet_met_dphi;
  float min_lep_met_dphi;
  float max_lep_met_dphi;
  float min_mt;
  float max_mt;
  float upara;
  float uperp;

  // Other mt2lblb
  float mt2lblb_A;
  float mt2lblb_B;
  float mt2lblb_min;
  float mt2bb;

  //Hyp Class -- in this order
  int hyp_class;

  //Leptons
  LorentzVector lep1_p4;
  LorentzVector lep2_p4;
  LorentzVector dilep_p4;
  int lep1_id;
  int lep2_id;
  int lep1_idx;
  int lep2_idx;
  int hyp_type; 

  //Lepton Mother (old isFromW function)
  int lep1_motherID;
  int lep2_motherID;

  //Lepton Truth ID
  int lep1_mc_id;
  int lep2_mc_id;

  //b-tags
  vector <LorentzVector> genbtags;
  vector <LorentzVector> btags;
  vector <float> btags_disc;
  int nbtags;
  int nbtags15;
  int nbtags20;
  int nbtags25;
  int nbtags30;

  int lep1_trigger_highpt;
  int lep2_trigger_highpt;
  int lep1_trigger_lowpt ;
  int lep2_trigger_lowpt ;
  bool trigger_double_highpt;
  bool trigger_single_highpt;
  bool trigger_double_lowpt;

  //mT for both leptons, mt2
  float mt;
  float mt2;

  //SUSY sparms
  float mLSP;
  float mGluino;
  float mSbottom;
  float mStop;
  float mChargino;
 
  //Look at gen-level particles and choose favorite hypothesis
  //Limited usefulness as-is; probably not smart enough to reject non-prompt leptons
  int lep1_id_gen;
  int lep2_id_gen;
  LorentzVector lep1_p4_gen;
  LorentzVector lep2_p4_gen;

  //Third lepton -- quality is 2 for good, 1 for fakeable, 0 for veto
  int lep3_id;
  int lep3_idx;
  int lep3_quality;
  LorentzVector lep3_p4;

  //Gen Leptons
  vector <LorentzVector> genps_p4;
  vector <int> genps_id;
  vector <int> genps_id_mother;
  vector <int> genps_status;
  vector <int> genps_id_grandma;
  vector <LorentzVector> genps_genjets_p4;

  int gen_bs_n;
  vector<LorentzVector> gen_bs_p4;
  vector<int> gen_bs_mother_id;
  vector<int> gen_bs_grandma_id;
  vector<int> gen_bs_closest_genjet;

  int gen_els_n;
  vector<LorentzVector> gen_els_p4;
  vector<LorentzVector> gen_els_reco_p4;
  vector<int> gen_els_mother_id;
  vector<int> gen_els_grandma_id;
  vector<bool> gen_els_reco_id;
  vector<bool> gen_els_reco_iso;
    
  int gen_mus_n;
  vector<LorentzVector> gen_mus_p4;
  vector<LorentzVector> gen_mus_reco_p4;
  vector<int> gen_mus_mother_id;
  vector<int> gen_mus_grandma_id;
  vector<bool> gen_mus_reco_id;
  vector<bool> gen_mus_reco_iso;
    
  int gen_taus_n;
  vector<LorentzVector> gen_taus_p4;
  vector<int> gen_taus_decaymode;
  vector<int> gen_taus_mother_id;
  vector<int> gen_taus_grandma_id;

  //Leptons pass numerator ID
  bool lep1_passes_id;
  bool lep2_passes_id;

  int nVetoSoftMuons;
        
  //Muon ID variables
  vector <float> muID_dzPV;      
  vector <float> muID_dxyPV;      
  vector <float> muID_ptSig;     
  vector <int>   muID_type;
  vector <int>   muID_nlayers;
  vector <float> muID_ip3dSig;   
  vector <float> muID_medMuonPOG;
  vector <float> muID_SoftMuon;
  vector <LorentzVector> muID_p4;

};
