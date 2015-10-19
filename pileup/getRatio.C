void getRatio() {

  TFile* fdata = TFile::Open("MyDataPileupHistogram.root");
  TFile* fmc  = TFile::Open("MyMCPileupHistogram.root");
  TH1D*  hdata = (TH1D*) fdata->Get("pileup");
  TH1D*  hmc = (TH1D*) fmc->Get("pileup");
  double idata = hdata->Integral();
  double imc = hmc->Integral();
  hdata->Scale(1/idata);
  hmc->Scale(1/imc);

  TFile* fout = TFile::Open("MyRatioPileupHistogram.root", "RECREATE");
  TH1D* hratio = new TH1D("pileup", "", 50, 0., 50.);
  hratio->Divide(hdata, hmc, 1., 1., "b");

  fout->cd();
  hratio->Write("pileup");
  fout->Close();

}
  
