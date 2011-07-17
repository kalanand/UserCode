{

   TFile* inFile = new TFile( "WScale_histos_10GeVbin_comb.root", "read");
   TH1D* scale1 = (TH1D*) inFile->Get("h_jjmass_WJets");
   TH1D* scaleup = (TH1D*) inFile->Get("h_jjmass_WJetsscaleup");
   TH1D* scaledown = (TH1D*) inFile->Get("h_jjmass_WJetsscaledown");
   scaleup->Divide(scale1);
   scaleup->GetXaxis()->SetTitle("Dijet mass [GeV]");
   scaleup->GetXaxis()->SetNdivisions(505);
   scaleup->GetYaxis()->SetTitle("q^{2}_{up} / q^{2}_{0}");
   scaleup->GetYaxis()->SetTitleOffset(1.2);
   scaledown->Divide(scale1);
   scaledown->GetXaxis()->SetTitle("Dijet mass [GeV]");
   scaledown->GetXaxis()->SetNdivisions(505);
   scaledown->GetYaxis()->SetTitle("q^{2}_{down} / q^{2}_{0}");
   scaledown->GetYaxis()->SetTitleOffset(1.2);


   gROOT->ProcessLine(".L ~/tdrstyle.C");
   setTDRStyle();
   tdrStyle->SetErrorX(0.5);
   tdrStyle->SetPadRightMargin(0.08);
   tdrStyle->SetLegendBorderSize(0);
   
   TCanvas* c_ScaleUp = new TCanvas("c_ScaleUp","", 500, 500);
   scaleup->Fit("pol3");
   scaleup->Draw();
   c_ScaleUp->SaveAs("WJets_ScaleVariation_up.eps");
   c_ScaleUp->SaveAs("WJets_ScaleVariation_up.gif");
   c_ScaleUp->SaveAs("WJets_ScaleVariation_up.root");
   c_ScaleUp->SaveAs("WJets_ScaleVariation_up.pdf");


   TCanvas* c_ScaleDown = new TCanvas("c_ScaleDown","", 500, 500);
   scaledown->Fit("pol3");
   scaledown->Draw();
   c_ScaleDown->SaveAs("WJets_ScaleVariation_down.eps");
   c_ScaleDown->SaveAs("WJets_ScaleVariation_down.gif");
   c_ScaleDown->SaveAs("WJets_ScaleVariation_down.root");
   c_ScaleDown->SaveAs("WJets_ScaleVariation_down.pdf");

}
