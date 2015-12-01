#ifndef WW_PT
#define WW_PT

#include "TGraph.h"

class WWpTreweight {
  
 public:
 
 TGraph* resum_0;
 TGraph* resum_1;
 TGraph* resum_2;
 TGraph* resum_3;
 TGraph* resum_4;
 TGraph* resum_5;

 TGraph* mc_0;
 TGraph* mc_1;
 TGraph* mc_2;
 TGraph* mc_3;
 TGraph* mc_4;
 TGraph* mc_5;

 WWpTreweight(TString central,
	      TString resum_up,
	      TString resum_down,
	      TString scale_up,
	      TString scale_down,
	      TString nnlo_central,
	      TString powheg_2l2nu_nlo,
	      TString powheg_2l2nu_qup_nlo,
	      TString powheg_2l2nu_qdown_nlo,
	      TString powheg_2l2nu_sup_nlo,
	      TString powheg_2l2nu_sdown_nlo,
	      TString powheg_2l2nu_nnlo) {

   resum_0 = new TGraph(central);
   resum_1 = new TGraph(resum_up);
   resum_2 = new TGraph(resum_down);
   resum_3 = new TGraph(scale_up);
   resum_4 = new TGraph(scale_down);
   resum_5 = new TGraph(nnlo_central);
   mc_0 = new TGraph(powheg_2l2nu_nlo);
   mc_1 = new TGraph(powheg_2l2nu_qup_nlo);
   mc_2 = new TGraph(powheg_2l2nu_qdown_nlo);
   mc_3 = new TGraph(powheg_2l2nu_sup_nlo);
   mc_4 = new TGraph(powheg_2l2nu_sdown_nlo);
   mc_5 = new TGraph(powheg_2l2nu_nnlo);

 }
 
 float reweight(double val, int calcType) {
      
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

};
  

 
 



#endif
