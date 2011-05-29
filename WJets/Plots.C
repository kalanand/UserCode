#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TPaveText.h"

#include "gensetup2011.h"

using namespace std;

// ====================================================================================
// test code
// ====================================================================================
void TestPieceCode()
{
  return;
}

// ====================================================================================
// TDR setup
// ====================================================================================
void setTDRStyle()
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600);    //Height of canvas
  tdrStyle->SetCanvasDefW(600);    //Width of canvas
  tdrStyle->SetCanvasDefX(0);      //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  //tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);


  // For the histo:
  //tdrStyle->SetHistFillColor(1);
  //tdrStyle->SetHistFillStyle(0);
  //tdrStyle->SetHistLineColor(1);
  //tdrStyle->SetHistLineStyle(0);
  //tdrStyle->SetHistLineWidth(1);
  //tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  //tdrStyle->SetNumberContours(Int_t number = 20);


  //tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  //tdrStyle->SetErrorX(0.);
  //tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1111);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(112210); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatStyle(0);
  tdrStyle->SetStatFont(42);
  //tdrStyle->SetStatFontSize(0.05);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(0);
  //tdrStyle->SetStatStyle(Style_t style = 1001);
  tdrStyle->SetStatW( 0.18);
  tdrStyle->SetStatH( 0.12);
  tdrStyle->SetStatX( 0.94);
  tdrStyle->SetStatY( 0.93);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  //tdrStyle->SetTitleH(0); // Set the height of the title box
  //tdrStyle->SetTitleW(0); // Set the width of the title box
  //tdrStyle->SetTitleX(0); // Set the position of the title box
  //tdrStyle->SetTitleY(0.985); // Set the position of the title box
  //tdrStyle->SetTitleStyle(Style_t style = 1001);
  //tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  //tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  //tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  //tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.03, "XYZ");
  
  // For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.02, "XYZ");
  tdrStyle->SetNdivisions(408, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);
  
  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);
  tdrStyle->cd();
  return;
}

// ====================================================================================
// Function
// ====================================================================================
void thishisthaveoerflowbin(void) {
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(90);
  t->SetTextAlign(12);
  t->SetTextSize(0.02);
  t->DrawLatex(0.96, 0.18, "Overflow Bin");
  t->Draw();
}

// ====================================================================================
// Function
// ====================================================================================
void olcmspreliminary(void) {
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(0);
  t->SetTextAlign(12);
  t->SetTextSize(0.030);
  t->DrawLatex(0.65, 0.970, "CMS Preliminary  #sqrt{s}=7 TeV");
  t->Draw();
}

// ====================================================================================
// Function
// ====================================================================================
void ollumi(void) {
   char tmpc[100];   sprintf(tmpc, "#int #font[12]{L} dt = %d pb^{-1}", 
   xsecs[0][2]/(xsecs[0][0]*xsecs[0][1]*xsecs[0][3]));
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(0);
  t->SetTextAlign(12);
  t->SetTextSize(0.035);
  t->DrawLatex(0.65, 0.85, tmpc);
  t->Draw();
}

// ====================================================================================
// Function
// ====================================================================================
void drawfreetext(string text = "Test", double xx=0.45, double yy=0.40, double tcolor=2 ,double tsize = 0.045) {
  TLatex * t = new TLatex;
  t->SetNDC();
  t->SetTextAngle(0);
  t->SetTextAlign(12);
  t->SetTextSize(tsize);
  t->SetTextColor(tcolor);
  t->DrawLatex(xx, yy, text.c_str());
  t->Draw();
}
// ====================================================================================
// Function
// ====================================================================================
TH1F * addoverflowbin(TH1F *h)
{ 
  UInt_t     nx    = h->GetNbinsX()+1;
  Double_t * xbins = new Double_t[nx+1];
  for (UInt_t i=0;i<nx;i++){
    xbins[i] = h->GetBinLowEdge(1) + i*h->GetBinWidth(1);
    if(h->GetXaxis()->GetXbins()->GetArray()) 
      xbins[i]=*(h->GetXaxis()->GetXbins()->GetArray()+i);
  }
  xbins[nx]=xbins[nx-1]+h->GetBinWidth(nx);
  // Book a temporary histogram having ab extra bin for overflows
  char tpname[100]; 
  sprintf(tpname, "%s-overflowbin", h->GetName());
  TH1F *htmp = new TH1F(tpname, h->GetTitle(), nx, xbins);
  // Reset the axis labels
  htmp->SetXTitle(h->GetXaxis()->GetTitle());
  htmp->SetYTitle(h->GetYaxis()->GetTitle());
  for (UInt_t i=1; i<=nx; i++)
    htmp->Fill(htmp->GetBinCenter(i), h->GetBinContent(i)); // Fill the new hitogram including the extra bin for overflows
  htmp->Fill(h->GetBinLowEdge(1)-1, h->GetBinContent(0));   // Fill the underflows
  htmp->SetEntries(h->GetEntries());                        // Restore the number of entries
  return htmp;
}

// ====================================================================================
// project histogram 
// ====================================================================================
TH1F * getHist( const char *filename, 
		const char *histname, 
		int   rflag  = 0        ) {
  TFile *f1 = (TFile *) gROOT->GetListOfFiles()->FindObject(filename);
  if (!f1 || !f1->IsOpen()) {
    //std::cout << "load into memory: " << filename << std::endl;
    f1 = new TFile(filename, "READ");
  }
  if (!f1) {
    std::cout << "can't open files" << std::endl;
    return 0;
  }
  f1->cd();
  TH1F  * h1  = (TH1F*) f1->Get(histname);
  if (!h1) return 0;
  if (rflag==0){
    return h1;}
  else         {
    TH1F * hov = addoverflowbin(h1); 
    return hov;
  }
}


// ====================================================================================
// Self Function
// ====================================================================================
void GenPlot(const char * outfilename = "tmp", 
	     const char * displayname = "tmp", 
	     const char * histname    = "hh_W_muon_pt", 
	     int    slog              = 0,
	     int    rflag             = 0              )
{
  TH1F    * hh[TOTAL_COMPONENTS];
  double    lumi[TOTAL_COMPONENTS];
  double    datalumi = xsecs[0][2]/(xsecs[0][0]*xsecs[0][1]*xsecs[0][3]);
  int       iColor = 2;
  THStack * Tss = new THStack("Ts", "");

  // float legX0=0.75, legX1=0.90, legY0=0.55, legY1=0.80;
  //if(slog==2) { legX0=0.25; legX1=0.55; legY0=0.7; legY1=0.89; }

  float legX0=0.2, legX1=0.55, legY0=0.7, legY1=0.89;

  TString strhname = TString(histname);
  if(strhname.Contains("mjj") || strhname.Contains("mlvjj") || 
  strhname.Contains("jjcentral") || strhname.Contains("jjdphi")) {
     legX0=0.7; legX1=0.90; legY0=0.52; legY1=0.82;
  }
  if(strhname.Contains("phipl")) {
     legX0=0.35; legX1=0.7; legY0=0.55; legY1=0.89;
  }

  TLegend * Leg = new TLegend( legX0, legY0, legX1, legY1);

  // All MC plot and stack together
  for (int i = 1; i<TOTAL_COMPONENTS; i++){
    if ( 
	i!=1  && //  WJets
	i!=2  && //  TT
	i!=3  && //  WZ
	i!=4  && //  WW
	i!=5  && //  H180
	i!=6     //  H300
	) continue;
    //cout << "here i am : " << names[i] << endl;
    lumi[i] = xsecs[i][2]/(xsecs[i][0]*xsecs[i][1]*xsecs[i][3]);
    char tpname[100];  sprintf(tpname, "hh[%i]", i);
    hh[i]   =    (TH1F*) (*(getHist(filenames[i], histname, rflag))).Clone(tpname);
    hh[i]->Sumw2(); 
    hh[i]->Scale(datalumi/lumi[i]);
    if( TString(filenames[i]).Contains("WJets") && strhname.Contains("mlvjj")) hh[i]->Scale(0.9);

    if (i!=5 && i!=6) { // skip signal MC
      hh[i]->SetFillColor(iColor++); if(i==6)hh[i]->SetFillColor(95);
      Leg->AddEntry(hh[i],  rootnames[i],  "f");  Tss->Add(hh[i]);}
  }
  // Data  
  TH1F * hdata    = (TH1F*) (*(getHist(filenames[0], histname, rflag))).Clone("hdata"); hdata->Sumw2();
  TH1F * hdasb    = (TH1F*) (*(getHist(filenames[0], histname, rflag))).Clone("hdasb"); hdasb->Sumw2();
  TH1F * hbakg    = (TH1F*) hh[1]->Clone("hbakg"); hbakg->Sumw2();
  hbakg->Add(hh[2],+1);  
  hdasb->Add(hbakg, -1);

  // set the correct error bars in the subtracted histogram
  for(int i=1; i<= hdata->GetNbinsX(); ++i) {
     hdasb->SetBinError( i, hdata->GetBinError(i) );
}

  TCanvas *  MyCa = new TCanvas(outfilename,outfilename,10,10,500,500);
  if (slog==2) MyCa->SetLogy(1);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  char tmpc[100];
  // sprintf(tmpc,"Events / %i GeV",int(hdata->GetBinWidth(1)) );
  //  if (slog==2)    
  sprintf(tmpc, "Events / %.2f GeV", hdata->GetBinWidth(1) );
  if(strhname.Contains("mjj") || strhname.Contains("mlvjj") )
    sprintf(tmpc, "Events / %d GeV", hdata->GetBinWidth(1) );
  hdata->SetMinimum(0.1);
  hdata->SetYTitle(tmpc);
  hdata->GetYaxis()->SetTitleOffset(1.8);
  hdata->SetMarkerStyle(20);
  hdata->SetMarkerSize(1.0);
  hdata->SetXTitle(displayname);
  hdata->GetXaxis()->SetTitleOffset(1.8);
  hdasb->SetYTitle(tmpc);
  hdasb->GetYaxis()->SetTitleOffset(1.8);
  hdasb->SetMarkerStyle(20);
  hdasb->SetMarkerSize(1.2);
  hdasb->SetXTitle(displayname);
  hdasb->GetXaxis()->SetTitleOffset(1.8);
  hdasb->SetMarkerStyle(20);
  hdasb->SetMarkerSize(1.2);
  double histmax = hdata->GetMaximum();
  if(slog==2) hdata->SetMaximum( 100. * histmax );
  else if(slog==0) hdata->SetMaximum( 1.5 * histmax );
  else {
     hdata->SetMaximum( 50 );
     hdata->SetMinimum( -20 );
  }
  if(histmax<100 && slog==0) hdata->SetMaximum( 2.5 * histmax );

  hdata->Draw("e");
  Tss->Draw("samehist");
  if (slog==1) hdasb->Draw("e");
  if (rflag==0 && slog==0 && !(TString(histname).Contains("ttb"))) hh[2]->Draw("samehist");
  hh[5]->SetLineStyle(2);
  hh[5]->SetLineColor(45);
  hh[5]->SetLineWidth(3);
  hh[5]->Draw("samehist");
  hh[6]->SetLineStyle(3);
  hh[6]->SetLineColor(55);
  hh[6]->SetLineWidth(3);
  hh[6]->Draw("samehist");
  TH1F * hww = (TH1F*) hh[4]->Clone("hww");
  hww->SetLineStyle(1);
  hww->SetLineColor(hh[4]->GetFillColor());
  hww->SetFillColor(0);
  hww->SetLineWidth(3);
  hww->Draw("samehist");
  if (slog==1) {hdasb->Draw("samee");}
  else{hdata->Draw("samee");}
  MyCa->RedrawAxis();
  Leg->AddEntry(hdata,   rootnames[0],  "pl");
  Leg->AddEntry(hh[5],   rootnames[5],  "l");
  Leg->AddEntry(hh[6],   rootnames[6],  "l");
  Leg->SetFillStyle(0);
  Leg->SetBorderSize(0);
  Leg->Draw();
  if (rflag==1) thishisthaveoerflowbin();
  ollumi();olcmspreliminary();
  MyCa->Print( (string(outfilename)+".eps").c_str());
  MyCa->Print( (string(outfilename)+".gif").c_str());
  MyCa->Print( (string(outfilename)+".root").c_str());
}






void Plots(int outflag = 2){
  // Start to make  plots
  setTDRStyle();
  
  if (outflag == 2) {
    ////  2011 Certified Data
	    GenPlot("Figures/certified11-topEvents-mjj",                             "m_{jj} [GeV]",                                "hh_ttb_mjj_all",    0,0);
    GenPlot("Figures/certified11-topEvents-noBtag-mjj",                      "m_{jj} [GeV]",                                "hh_ttb_mjj_nob",    0,0);

    GenPlot("Figures/certified11-mjj-outOfBox",                              "m_{jj} [GeV]",                                "hh_lvjj_mjj",       0,0);
    GenPlot("Figures/certified11-mjj-deltaPhiCutOnly",                       "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     0,0);
    GenPlot("Figures/certified11-subtracted-mjj-deltaPhiCutOnly",            "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     1,0);
	  
    GenPlot("Figures/certified11-mjj-dPhiCutOnly",                           "m_{jj} [GeV]",                                "hh_lvjj_kkmjj_dphi",     0,0);
    GenPlot("Figures/certified11-subtracted-mjj-dPhiCutOnly",                "m_{jj} [GeV]",                                "hh_lvjj_kkmjj_dphi",     1,0);

	
	GenPlot("Figures/certified11-mjj-dgAngularCuts",                         "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     0,0);
    GenPlot("Figures/certified11-subtracted-mjj-dgAngularCuts",              "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     1,0);

    GenPlot("Figures/certified11-mlvjj-outOfBox",                            "m_{l#nuJJ} [GeV]",                            "hh_lvjj_mlvjj",     0,0);
    GenPlot("Figures/certified11-mlvjj-dgAngularCuts-mjj60to100",            "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   0,0);
    GenPlot("Figures/certified11-subtracted-mlvjj-dgAngularCuts-mjj60to100", "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   1,0);
    GenPlot("Figures/certified11-mlnujj-deltaPhiCut_mjj60to100",             "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   0,0);
    GenPlot("Figures/certified11-subtracted-mlnujj-deltaPhiCut_mjj60to100",  "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   1,0);
	
    GenPlot("Figures/certified11-mlnujj-dPhiCut_mjj60to100",             "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj_dphi",   0,0);
    GenPlot("Figures/certified11-subtracted-mlnujj-dPhiCut_mjj60to100",  "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj_dphi",   1,0);

/*	
    GenPlot("Figures/certified11-cosJacksonW_inWWframe",                     "cos#theta_{Jackson} (W, WW)",                 "hh_lvjj_cosanwinww",0,0);
    GenPlot("Figures/certified11-cosDecayPlaneWW",                           "cos#phi_{DecayPlane}^{CM} WW",                "hh_lvjj_phipl",     0,0);
    GenPlot("Figures/certified11-cosJacksonL_inWframe",                      "cos#theta_{Jackson} (l, l#nu)",               "hh_lvjj_ctuv",      0,0);
    GenPlot("Figures/certified11-cosJacksonJ_inJJframe",                     "cos#theta_{Jackson} (lead jet, jj)",          "hh_lvjj_ctjj",      0,0);
    GenPlot("Figures/certified11-Jacobian",                                  "Jacobian: p^{T}_{W}/m_{WW}",                  "hh_lvjj_jac",       0,0);
    GenPlot("Figures/certified11-jjcentrality",                              " Centrality = |tanh(0.5*(Y_{j1} - Y_{j2}))|", "hh_lvjj_jjcentral", 0,0);
    GenPlot("Figures/certified11-jjdphi",                                    "#Delta #phi(j1, j2)",      "hh_lvjj_jjdphi",                       0,0);
    GenPlot("Figures/certified11-j1metdphi",                                 "#Delta #phi(j1, MET)", "hh_lvjj_ljmetdphi",                        0,0);
    GenPlot("Figures/certified11-wwdphi",                                    "#Delta #phi (W_{#mu#nu}, W_{jj})", "hh_lvjj_wwdphi",               0,0);
*/

    ////  2011 DCS ONLY Data
/*
    GenPlot("Figures/dcsOnly-topEvents-mjj.eps",                             "m_{jj} [GeV]",                                "hh_ttb_mjj_all",    0,0);
    GenPlot("Figures/dcsOnly-topEvents-noBtag-mjj.eps",                      "m_{jj} [GeV]",                                "hh_ttb_mjj_nob",    0,0);

    GenPlot("Figures/dcsOnly-mjj-outOfBox.eps",                              "m_{jj} [GeV]",                                "hh_lvjj_mjj",       0,0);
    GenPlot("Figures/dcsOnly-mjj-deltaPhiCutOnly.eps",                       "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     0,0);
    GenPlot("Figures/dcsOnly-subtracted-mjj-deltaPhiCutOnly.eps",            "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     1,0);
    GenPlot("Figures/dcsOnly-mjj-dgAngularCuts.eps",                         "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     0,0);
    GenPlot("Figures/dcsOnly-subtracted-mjj-dgAngularCuts.eps",              "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     1,0);

    GenPlot("Figures/dcsOnly-mlvjj-outOfBox.eps",                            "m_{l#nuJJ} [GeV]",                            "hh_lvjj_mlvjj",     0,0);
    GenPlot("Figures/dcsOnly-mlvjj-dgAngularCuts-mjj60to100.eps",            "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   0,0);
    GenPlot("Figures/dcsOnly-subtracted-mlvjj-dgAngularCuts-mjj60to100.eps", "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   1,0);
    GenPlot("Figures/dcsOnly-mlnujj-deltaPhiCut_mjj60to100.eps",             "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   0,0);
    GenPlot("Figures/dcsOnly-subtracted-mlnujj-deltaPhiCut_mjj60to100.eps",  "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   1,0);

    GenPlot("Figures/dcsOnly-cosJacksonW_inWWframe.eps",                     "cos#theta_{Jackson} (W, WW)",                 "hh_lvjj_cosanwinww",2,0);
    GenPlot("Figures/dcsOnly-cosDecayPlaneWW.eps",                           "cos#phi_{DecayPlane}^{CM} WW",                "hh_lvjj_phipl",     2,0);
    GenPlot("Figures/dcsOnly-cosJacksonL_inWframe.eps",                      "cos#theta_{Jackson} (l, l#nu)",               "hh_lvjj_ctuv",      2,0);
    GenPlot("Figures/dcsOnly-cosJacksonJ_inJJframe.eps",                     "cos#theta_{Jackson} (lead jet, jj)",          "hh_lvjj_ctjj",      2,0);
    GenPlot("Figures/dcsOnly-Jacobian.eps",                                  "Jacobian: p^{T}_{W}/m_{WW}",                  "hh_lvjj_jac",       2,0);
    GenPlot("Figures/dcsOnly-jjcentrality.eps",                              " Centrality = |tanh(0.5*(Y_{j1} - Y_{j2}))|", "hh_lvjj_jjcentral", 2,0);
    GenPlot("Figures/dcsOnly-jjdphi.eps",                                    "#Delta #phi(j1, j2)",      "hh_lvjj_jjdphi",                       2,0);
    GenPlot("Figures/dcsOnly-j1metdphi.eps",                                 "#Delta #phi(j1, MET)", "hh_lvjj_ljmetdphi",                        2,0);
    GenPlot("Figures/dcsOnly-wwdphi.eps",                                    "#Delta #phi (W_{#mu#nu}, W_{jj})", "hh_lvjj_wwdphi",               2,0);

*/
    //// 2010 Certified Data
/*
    GenPlot("Figures/certified10-topEvents-mjj.eps",                             "m_{jj} [GeV]",                                "hh_ttb_mjj_all",    0,0);
    GenPlot("Figures/certified10-topEvents-noBtag-mjj.eps",                      "m_{jj} [GeV]",                                "hh_ttb_mjj_nob",    0,0);

    GenPlot("Figures/certified10-mjj-outOfBox.eps",                              "m_{jj} [GeV]",                                "hh_lvjj_mjj",       0,0);
    GenPlot("Figures/certified10-mjj-deltaPhiCutOnly.eps",                       "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     0,0);
    GenPlot("Figures/certified10-subtracted-mjj-deltaPhiCutOnly.eps",            "m_{jj} [GeV]",                                "hh_lvjj_kkmjj",     1,0);
    GenPlot("Figures/certified10-mjj-dgAngularCuts.eps",                         "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     0,0);
    GenPlot("Figures/certified10-subtracted-mjj-dgAngularCuts.eps",              "m_{jj} [GeV]",                                "hh_lvjj_dgmjj",     1,0);

    GenPlot("Figures/certified10-mlvjj-outOfBox.eps",                            "m_{l#nuJJ} [GeV]",                            "hh_lvjj_mlvjj",     0,0);
    GenPlot("Figures/certified10-mlvjj-dgAngularCuts-mjj60to100.eps",            "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   0,0);
    GenPlot("Figures/certified10-subtracted-mlvjj-dgAngularCuts-mjj60to100.eps", "m_{l#nujj} [GeV]",                            "hh_lvjj_dgmlvjj",   1,0);
    GenPlot("Figures/certified10-mlnujj-deltaPhiCut_mjj60to100.eps",             "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   0,0);
    GenPlot("Figures/certified10-subtracted-mlnujj-deltaPhiCut_mjj60to100.eps",  "m_{l#nujj} [GeV]",                            "hh_lvjj_kkmlvjj",   1,0);

    GenPlot("Figures/certified10-cosJacksonW_inWWframe.eps",                     "cos#theta_{Jackson} (W, WW)",                 "hh_lvjj_cosanwinww",2,0);
    GenPlot("Figures/certified10-cosDecayPlaneWW.eps",                           "cos#phi_{DecayPlane}^{CM} WW",                "hh_lvjj_phipl",     2,0);
    GenPlot("Figures/certified10-cosJacksonL_inWframe.eps",                      "cos#theta_{Jackson} (l, l#nu)",               "hh_lvjj_ctuv",      2,0);
    GenPlot("Figures/certified10-cosJacksonJ_inJJframe.eps",                     "cos#theta_{Jackson} (lead jet, jj)",          "hh_lvjj_ctjj",      2,0);
    GenPlot("Figures/certified10-Jacobian.eps",                                  "Jacobian: p^{T}_{W}/m_{WW}",                  "hh_lvjj_jac",       2,0);
    GenPlot("Figures/certified10-jjcentrality.eps",                              " Centrality = |tanh(0.5*(Y_{j1} - Y_{j2}))|", "hh_lvjj_jjcentral", 2,0);
    GenPlot("Figures/certified10-jjdphi.eps",                                    "#Delta #phi(j1, j2)",      "hh_lvjj_jjdphi",                       2,0);
    GenPlot("Figures/certified10-j1metdphi.eps",                                 "#Delta #phi(j1, MET)", "hh_lvjj_ljmetdphi",                        2,0);
    GenPlot("Figures/certified10-wwdphi.eps",                                    "#Delta #phi (W_{#mu#nu}, W_{jj})", "hh_lvjj_wwdphi",               2,0);
*/
  }
}
