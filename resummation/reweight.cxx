// 1 - nlo central
// 2 - nlo resum up
// 3 - nlo resum down
// 4 - nlo scale up
// 5 - nlo scale down

#include "TGraph.h"

enum precision {NLO, NLO_QUP, NLO_QDOWN, NLO_SUP, NLO_SDOWN, NNLO};

TGraph* resum_0 = new TGraph("central.dat");
TGraph* resum_1 = new TGraph("resum_up.dat");
TGraph* resum_2 = new TGraph("resum_down.dat");
TGraph* resum_3 = new TGraph("scale_up.dat");
TGraph* resum_4 = new TGraph("scale_down.dat");
TGraph* resum_5 = new TGraph("nnlo_central.dat");

TGraph* mc_0 = new TGraph("powheg_2l2nu_nlo.dat");
TGraph* mc_1 = new TGraph("powheg_2l2nu_qup_nlo.dat");
TGraph* mc_2 = new TGraph("powheg_2l2nu_qdown_nlo.dat");
TGraph* mc_3 = new TGraph("powheg_2l2nu_sup_nlo.dat");
TGraph* mc_4 = new TGraph("powheg_2l2nu_sdown_nlo.dat");
TGraph* mc_5 = new TGraph("powheg_2l2nu_nnlo.dat");

double reweight(double val, int calcType) {

  TGraph* resum;
  TGraph* mc;
  
  double retval = 1.0;

  switch(calcType) {
    
  case(0):
    retval = val < 160. ? resum_0->Eval(val)/mc_0->Eval(val) : 1;
    break;

  case(1):
    retval = val < 160. ? resum_1->Eval(val)/mc_1->Eval(val) : 1;
    break;

  case(2):
    retval = val < 160. ? resum_2->Eval(val)/mc_2->Eval(val) : 1;
    break;

  case(3):
    retval = val < 160. ? resum_3->Eval(val)/mc_3->Eval(val) : 1;
    break;

  case(4):
    retval = val < 160. ? resum_4->Eval(val)/mc_4->Eval(val) : 1;
    break;

  case(5):
    retval = val < 500. ? resum_5->Eval(val)/mc_5->Eval(val) : 1;
    break;
    
  }
  
  return retval;
}
    
  
