//C++
#include <iostream>
#include <vector>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>

//ROOT
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TMath.h"

//Local
#include "./looper.h"
#include "./helper_babymaker.h"
#include "./tools.h"

#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/dorky/dorky.h"

//CMS3
#include "CORE/CMS3.h"

using namespace tas;
using namespace std;
using namespace WWAnalysis;
using namespace duplicate_removal;

int looper::ScanChain(TChain* chain, TString prefix, TString suffix, TString whatTest, int nEvents, double fudge, vector<int> evtToDebug){

  //Don't change these parameters by hand, please set them from main.cc
  makebaby       = 1;
  makehist       = 0;
  maketext       = 0;

  //Status Message
  cout << "Processing " << prefix << " " << suffix << " " << whatTest << endl;

  bool isDataFromFileName = false;
  if (prefix.Contains("2015A") || prefix.Contains("2015B") || prefix.Contains("2015C") || prefix.Contains("2015D"))
    isDataFromFileName = true;

  //Debug Flag
  bool debug = 0;  

  //Determine suffix
  if (suffix!="") suffix = "_"+suffix;

  std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3;
  FactorizedJetCorrector *jet_corrector_pfL1FastJetL2L3;

  std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3_puppi;
  FactorizedJetCorrector *jet_corrector_pfL1FastJetL2L3_puppi;

  jetcorr_filenames_pfL1FastJetL2L3.clear();
  jetcorr_filenames_pfL1FastJetL2L3_puppi.clear();

   if (isDataFromFileName) {
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L1FastJet_AK4PFchs.txt");
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L2Relative_AK4PFchs.txt");
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L3Absolute_AK4PFchs.txt");
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L2L3Residual_AK4PFchs.txt");

     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L1FastJet_AK4PFPuppi.txt");
     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L2Relative_AK4PFPuppi.txt");
     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L3Absolute_AK4PFPuppi.txt");
     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_DATA/Summer15_25nsV5_DATA_L2L3Residual_AK4PFPuppi.txt");
   } else {
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L1FastJet_AK4PFchs.txt");
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L2Relative_AK4PFchs.txt");
     jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L3Absolute_AK4PFchs.txt");

     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L1FastJet_AK4PFPuppi.txt");
     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L2Relative_AK4PFPuppi.txt");
     jetcorr_filenames_pfL1FastJetL2L3_puppi.push_back  ("jetCorrections/JECDatabase/textFiles/Summer15_25nsV5_MC/Summer15_25nsV5_MC_L3Absolute_AK4PFPuppi.txt");
   }

  jet_corrector_pfL1FastJetL2L3 = makeJetCorrector(jetcorr_filenames_pfL1FastJetL2L3);
  jet_corrector_pfL1FastJetL2L3_puppi = makeJetCorrector(jetcorr_filenames_pfL1FastJetL2L3_puppi);

  //Instantiate Babymaker, if making a baby
  babyMaker* bm=0;
  bm = new babyMaker(jet_corrector_pfL1FastJetL2L3, jet_corrector_pfL1FastJetL2L3_puppi, debug);
  bm->MakeBabyNtuple( Form( "%s%s", prefix.Data(), suffix.Data() ));

  //Instantiate MVA for electron ID
  createAndInitMVA("./CORE");

  // Apply DQ
  const char* json_file = "DQ/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3_CMS3.txt"; //  594.65/pb
  set_goodrun_file(json_file);

  // Apply pileup reweight
  const char* scalePileup_file = "pileup/MyRatioPileupHistogram.root"; //  594.65/pb
  bm->SetPileupHist(scalePileup_file);

  //Set up the file loop
  if(nEvents == -1) nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Begin the file loop!
  while ((currentFile = (TFile*)fileIter.Next())){
  
    //Get tree
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("Events");

    //Debug message
    if (debug) cout << "processing file: " << currentFile->GetTitle() << endl;

    //Initialize CMS3 tree
    cms3.Init(tree);

    //Event Loop
    for(unsigned int event = 0; event < tree->GetEntries(); ++event){

      //Get Event Content
      cms3.GetEntry(event);
 
      //Progress Bar
      nEventsTotal++;
      CMS3::progress(nEventsTotal, nEventsChain);

        // DQ
      if (cms3.evt_isRealData()) {
	if (!goodrun(cms3.evt_run(), cms3.evt_lumiBlock())) {
	  continue;
	}
	DorkyEventIdentifier id(cms3.evt_run(), cms3.evt_event(), cms3.evt_lumiBlock());
	if (is_duplicate(id)) continue;
      }

      //If making a baby, init the baby ntuple
      bm->InitBabyNtuple();
      bm->ProcessBaby(currentFile->GetTitle(), fudge);

    }//event loop

    delete tree;
    f.Close();
  }

  if ( nEventsChain != nEventsTotal ) std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;

  bm->CloseBabyNtuple();
  if (jet_corrector_pfL1FastJetL2L3) delete jet_corrector_pfL1FastJetL2L3;
  if (jet_corrector_pfL1FastJetL2L3_puppi) delete jet_corrector_pfL1FastJetL2L3_puppi;
  if (bm) delete bm;

  return 0;
}

void looper::printEvent(  ostream& ostr ){
  ostr << evt_run() << " " << evt_lumiBlock() << endl;
}

void looper::fillUnderOverFlow(TH1F *h1, float value, float weight){

  float min = h1->GetXaxis()->GetXmin();
  float max = h1->GetXaxis()->GetXmax();

  if (value > max) value = h1->GetBinCenter(h1->GetNbinsX());
  if (value < min) value = h1->GetBinCenter(1);

  h1->Fill(value, weight);
}
