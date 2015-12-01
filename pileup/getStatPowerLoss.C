void getStatPowerLoss() {

  TFile* mc = TFile::Open("MyMCPileupHistogram.root");
  TH1D* hmc = (TH1D*) mc->Get("pileup");
  
  TFile* wgt = TFile::Open("MyRatioPileupHistogram.root");
  TH1D* hwgt = (TH1D*) wgt->Get("pileup");
  
  double old_sumx = 0;
  double old_sumx2 = 0;
  double new_sumx = 0;
  double new_sumx2 = 0;
  for (int i=0; i<=hmc->GetNbinsX()+1; i++) {
    double val = hmc->GetBinContent(i);
    double factor = hwgt->GetBinContent(i);
    old_sumx += val;
    old_sumx2 += val*val;
    
    new_sumx += val*factor;
    new_sumx2 += val*factor*val*factor;
  }
  
  double old_stat = old_sumx*old_sumx/old_sumx2;
  double new_stat = new_sumx*new_sumx/new_sumx2;

  double loss = 1-new_stat/old_stat;
  
  cout << "The fractional loss in statistical power is " << std::setprecision(4) << loss*100 << "%" << endl;

  mc->Close();
  wgt->Close();
}
