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

int looper::ScanChain(TChain* chain, TString prefix, TString suffix, TString whatTest, int nEvents, double fudge, int JECvar, vector<int> evtToDebug){

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
			
  //Instantiate Babymaker, if making a baby
  babyMaker* bm=0;
  bm = new babyMaker(isDataFromFileName, debug);
  bm->MakeBabyNtuple( Form( "%s%s", prefix.Data(), suffix.Data() ));

  // Apply DQ
  const char* json_file = "DQ/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_CMS3.txt"; // 2.11 /fb
  set_goodrun_file(json_file);

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
      }

      //If making a baby, init the baby ntuple
      bm->InitBabyNtuple();
      bm->ProcessBaby(currentFile->GetTitle(), fudge, JECvar);

    }//event loop

    delete tree;
    f.Close();
  }

  if ( nEventsChain != nEventsTotal ) std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;

  bm->CloseBabyNtuple();
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
