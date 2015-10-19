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

#include <vector>
#include <string>

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
typedef vector<pair<const LorentzVector *, double> > jets_with_corr_t;
typedef ROOT::Math::Boost Boost;

using namespace std;

//Classes
class babyMaker {

  public:
    babyMaker(FactorizedJetCorrector* jetCorrector_pf_, FactorizedJetCorrector* jetCorrector_puppi_, bool debug = 0);
    void MakeBabyNtuple(const char* output_name);
    void InitBabyNtuple();
    void CloseBabyNtuple () { BabyFile->cd(); BabyTree->Write(); BabyFile->Close(); }
    int ProcessBaby(string filename_in, double fudge);
    void SetPileupHist(const char* pileup_file);

  protected:
    TFile* BabyFile;
    TTree* BabyTree;

  private:

    FactorizedJetCorrector* jetCorrector;
    FactorizedJetCorrector* jetCorrector_puppi;
    
    TH1D* scalePileupHist;
    float scalePileup;

    //Switches
    TString path;
    bool verbose;
    unsigned int evt_cut;

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
    int nGoodVertices; 

    //Filters
    bool filt_csc;
    bool filt_hbhe;
    bool filt_hcallaser;
    bool filt_ecaltp;
    bool filt_trkfail;
    bool filt_eebadsc;

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

    // jettoolbox MET
    float met_jettoolbox;
    float metPhi_jettoolbox;

    // track MET
    float met_track;
    float metPhi_track;

    float met_puppi;
    float metPhi_puppi;

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
    vector <float> jets_disc;
    vector <float> jets_CSVv2;
    vector <float> jets_CSVsm;
    vector <float> jets_CSVse;
    vector <float> jets_CSVtche;

    vector <LorentzVector> genjets_puppi;

    int njets_puppi;
    int njets20_puppi;
    int njets25_puppi;
    int njets30_puppi;
    int njets35_puppi;
    int njets40_puppi;
    float ht_puppi;
    float ht20_puppi;
    float ht25_puppi;
    float ht30_puppi;
    vector<float> pileup_jet_id_puppi;
    vector<int> jet_parton_flavor_puppi;
    vector<int> jet_hadron_flavor_puppi;
    vector <LorentzVector> jets_puppi;
    vector <float> jets_disc_puppi;
    vector <float> jets_CSVv2_puppi;
    vector <float> jets_CSVsm_puppi;
    vector <float> jets_CSVse_puppi;
    vector <float> jets_CSVtche_puppi;


    // DY studies
    float projmet              ;
    float projmet_track        ;
    float min_projmet          ;
    float metsig               ;
    float metsig30             ;
    float dilep_pt             ;
    float lep1_mt              ;
    float lep2_mt              ;
    float jet1_pt              ;
    float jet2_pt              ;
    float ut                   ;
    float lep1_met_dphi        ;
    float lep2_met_dphi        ;
    float dilep_jet1_dphi      ;
    float dilep_jet2_dphi      ;
    float dilep_met_dphi       ;
    float jet1_met_dphi        ;
    float jet2_met_dphi        ;
    float jet1_jet2_dphi       ;
    float min_jet_met_dphi     ;
    float max_jet_met_dphi     ;
    float min_lep_met_dphi     ;
    float max_lep_met_dphi     ;
    float min_mt               ;
    float max_mt               ;
    float upara                ;
    float uperp                ;

    // Other mt2lblb
    float mt2lblb_A            ;
    float mt2lblb_B            ;
    float mt2lblb_min          ;
    float mt2bb                ;

    // Melia's variables
    float x1V      ;
    float x2V      ;
    float costhetaV;

    //Hyp Class -- in this order
       //3 for num-num SS leptons
       //2 for num-den OS leptons
       //1 for den-den OS leptons
       //4 for num-num OS leptons
       //0 otherwise (not saved)
    int hyp_class;

    //Leptons
    LorentzVector lep1_p4;
    LorentzVector lep2_p4;
    LorentzVector dilep_p4;
    int lep1_id;
    int lep2_id;
    int lep1_idx;
    int lep2_idx;
    float lep1_MVA;
    float lep2_MVA;
    int hyp_type; 
    float lep1_coneCorrPt;
    float lep2_coneCorrPt;

    //Lepton Mother (old isFromW function)
    int lep1_motherID;
    int lep2_motherID;

    //Lepton Truth ID
      //From src/MatchUtilities/matchCandToGen function
      //delta-R matches to nearest particle other than neutrino or LSP
      //must be within 0.2
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

    vector <LorentzVector> genbtags_puppi;

    vector <LorentzVector> btags_puppi;
    vector <float> btags_disc_puppi;
    int nbtags_puppi;
    int nbtags15_puppi;
    int nbtags20_puppi;
    int nbtags25_puppi;
    int nbtags30_puppi;

    //Scale factors (from 8 TeV, outdated)
    int lep1_trigger_highpt;
    int lep2_trigger_highpt;
    int lep1_trigger_lowpt ;
    int lep2_trigger_lowpt ;
    bool trigger_highpt;
    bool trigger_lowpt;
    float sf_dilep_eff;

    //mT for both leptons, mt2
    float mt;
    float mt_l2;
    float mt2;
    float mtmin;

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

    //Isolation
    float lep1_iso;
    float lep2_iso;

    //Gen Leptons
    vector <LorentzVector> genps_p4;
    vector <int> genps_id;
    vector <int> genps_id_mother;
    vector <int> genps_status;
    vector <int> genps_id_grandma;

    //Leptons pass numerator ID
    bool lep1_passes_id;
    bool lep2_passes_id;

    //Imparct parameter
    float lep1_dxyPV;
    float lep2_dxyPV;
    float lep1_d0_err;
    float lep2_d0_err;
    float lep1_dZ;
    float lep2_dZ;
    float lep1_ip3d;
    float lep1_ip3d_err;
    float lep2_ip3d;
    float lep2_ip3d_err;

    //nVetoElectrons passing certain pT cuts
    int nVetoElectrons7;
    int nVetoElectrons10;
    int nVetoElectrons25;
    int nVetoSoftMuons;
    int nVetoMuons5;
    int nVetoMuons10;
    int nVetoMuons25;

    //PtRel, v0 and v1 (v1 is lepton-subtracted)
    float lep1_ptrel_v0;
    float lep1_ptrel_v1;
    float lep2_ptrel_v0;
    float lep2_ptrel_v1;

    //MiniIso
    float lep1_miniIso;
    float lep2_miniIso;

    //Closest Jet
    LorentzVector jet_close_lep1;
    LorentzVector jet_close_lep2;

    //Electron ID variables
    vector <int>   eleID_kfhits;
    vector <float> eleID_oldsigmaietaieta;
    vector <float> eleID_oldsigmaiphiiphi;
    vector <float> eleID_oldcircularity;
    vector <float> eleID_oldr9;
    vector <float> eleID_scletawidth;
    vector <float> eleID_sclphiwidth;
    vector <float> eleID_he;
    vector <float> eleID_psEoverEraw;
    vector <float> eleID_kfchi2;
    vector <float> eleID_chi2_hits;
    vector <float> eleID_fbrem;
    vector <float> eleID_ep;
    vector <float> eleID_eelepout;
    vector <float> eleID_IoEmIop;
    vector <float> eleID_deltaetain;
    vector <float> eleID_deltaphiin;
    vector <float> eleID_deltaetaseed;
    vector <float> eleID_pT;
    vector <bool>  eleID_isbarrel;
    vector <bool>  eleID_isendcap;
    vector <float> eleID_scl_eta;
    vector <float> eleID_MVA;
    vector <float> eleID_gsfhits;
    vector <float> eleID_expectedMissingInnerHits;
    vector <float> eleID_convVtxFitProbability;
        
    //Muon ID variables
    vector <float> muID_dzPV;      
    vector <float> muID_ptSig;     
    vector <float> muID_ip3dSig;   
    vector <float> muID_medMuonPOG;
    vector <float> muID_pt;        
    vector <float> muID_eta;       

    //InSituFR
    bool lep1_isGoodLeg;
    bool lep2_isGoodLeg;
    bool lep1_isFakeLeg;
    bool lep2_isFakeLeg;
    bool truth_inSituFR;
    bool lep1_multiIso;
    bool lep2_multiIso;
    float lep1_sip; 
    float lep2_sip; 
    LorentzVector lep1_closeJet;
    LorentzVector lep2_closeJet;
    bool passed_id_inSituFR_lep1;
    bool passed_id_inSituFR_lep2;

};
