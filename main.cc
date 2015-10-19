#include "TChain.h"
#include "TFileCollection.h"
#include "looper.h"

using namespace WWAnalysis;

int main(int argc, char** argv) {

  looper *l = new looper();
  TChain *ch = new TChain("Events");

  TFileCollection* fc = new TFileCollection(argv[2],"",argv[1]);
  ch->AddFileInfoList((TCollection*) fc->GetList());

  TString fudgeText(argv[3]);
  l->ScanChain(ch , argv[2], "baby", "MakeBaby", -1, fudgeText.Atof());

  return 0;

}

