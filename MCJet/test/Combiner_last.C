#include "TGraphErrors.h"
#include "TFile.h"
#include "TKey.h"
#include "TObjArray.h"
#include <iostream>
#include <vector>


gROOT->Reset();
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0000);
gStyle->SetOptFit(000); 
gStyle->SetPalette(1);

const double rMin = 5.0;
const double rMax = 5000.0;
const int rBin = (int)(rMax - rMin);

void Combiner () {
  Combiner ( "L3Graphs_test_Icone5.root", "csa07.root");
}




void Combiner ( char* ZeeJetFile, char* GammaJetFile) {

  // Get Zee + Jet
  TFile* inf = new TFile( ZeeJetFile, "r" );
  TGraphErrors *g_Cor = (TGraphErrors*)inf->Get("Correction_vs_CaloPt");
  TMatrixD *COV_Cor = (TMatrixD*)inf->Get("CovMatrix_Correction");
  TF1 *CorFit = (TF1*)g_Cor->GetFunction("CorFit");
  CorFit->SetRange(rMin,rMax);  
  TH1F* hCorUncertainty = Uncertainty(*CorFit, *COV_Cor, "CorrErr0");
  TF1* MCOverCor = TransformationFunction( *CorFit);
  MCOverCor->SetLineColor(2);
  TGraphErrors* new_gr = ScaleGraph( *g_Cor, *MCOverCor);
  TH1F* newhCorUncertainty = Uncertainty(*CorFit, *COV_Cor, "CorrErr1");



  // Get dijet MC curve
  TF1* MCcurve = Dijet_MC_Curve();



  // Get Gamma + Jet
  TFile* inf2 = new TFile( GammaJetFile, "r" );
  TGraphErrors *g_Cor2 = (TGraphErrors*)inf2->Get("correction_ite");
  TF1 *CorFit2 = (TF1*)g_Cor2->GetFunction("biassum_sig");
  CorFit2->SetRange(rMin,rMax);  
  TF1* MCOverCor2 = TransformationFunction( *CorFit2);
  TGraphErrors* new_gr2 = ScaleGraph( *g_Cor2, *MCOverCor2);
  TF1* C_gamma = new TF1("C_gamma","[0]+[1]/(pow(log10(x),[2])+[3])",18.,rMax);
  new_gr2->Fit(C_gamma,"RQ");
  afitter = TVirtualFitter::GetFitter();
  TMatrixD* COV_Cor2 = new TMatrixD(4,4,afitter->GetCovarianceMatrix());
  TH1F* hCorUncertainty2 = Uncertainty(*CorFit2, *COV_Cor2, "CorrErr2");
  hCorUncertainty2->SetFillColor(11);



  // Now combine all the L3 corrections
  TF1* C_all = new TF1("C_all","[0]+[1]/(pow(log10(x),[2])+[3])",rMin,rMax);
  TGraphErrors* merged_gr = MergeGraph ( *new_gr, *new_gr2 );
  merged_gr->Fit(C_all,"RQ");
  fitter = TVirtualFitter::GetFitter();
  TMatrixD* cov = new TMatrixD(4,4,fitter->GetCovarianceMatrix());
  TH1F* hCorrErr = Uncertainty(*C_all, *cov, "CorrErrAll");
  C_all->SetLineColor(6);



  // Assume that the combined L3 corrections points in data are 10% higher 
  TGraphErrors* gr_10pc = ScaleGraph( *merged_gr, 1.1);
  TF1* C_10pc = new TF1("C_10pc","[0]+[1]/(pow(log10(x),[2])+[3])",rMin,rMax);
  gr_10pc->Fit(C_10pc,"RQ");
  fitter = TVirtualFitter::GetFitter();
  TMatrixD* cov_10pc = new TMatrixD(4,4,fitter->GetCovarianceMatrix());
  TH1F* hCorrErr_10pc = Uncertainty(*C_10pc, *cov_10pc, "CorrErrAll_10pc");
  C_10pc->SetLineColor(6);



  // Assume that the combined L3 corrections points in data are 10% higher 
  // at lowest Pt and only 2% higher at highest pT
  TF1* linear = new TF1("linear","1.1613 - 0.0471 * log10(x)",rMin,rMax);
  TGraphErrors* gr_linear = ScaleGraph( *merged_gr, *linear);
  TF1* C_linear = new TF1("C_linear","[0]+[1]/(pow(log10(x),[2])+[3])",
			  rMin,rMax);
  gr_linear->Fit(C_linear,"RQ");
  fitter = TVirtualFitter::GetFitter();
  TMatrixD* cov_linear = new TMatrixD(4,4,fitter->GetCovarianceMatrix());
  TH1F* hCorrErr_linear = Uncertainty(*C_linear, *cov_linear, 
				      "CorrErrAll_linear");
  C_linear->SetLineColor(6);




  //////////////////////////////////////////////////
  //////////////////////////////////////////////////

  TCanvas *c_Correction = new TCanvas("Correction","Correction",900,600);
  gStyle->SetOptStat(0000);
  c_Correction->cd(); 
  gPad->SetLogx();
  g_Cor->SetMarkerStyle(20);
  g_Cor->SetMarkerColor(1);
  g_Cor->SetLineColor(1);
  hCorUncertainty->SetMaximum(3.2);
  CorFit->SetLineColor(2);
  CorFit->SetLineWidth(2);

  const int  plotCase=5;

  if(plotCase==1) {
    g_Cor->SetMarkerColor(2);
    g_Cor->SetLineColor(2);
    g_Cor->SetMarkerSize(0.7);
    g_Cor2->SetMarkerStyle(20);
    g_Cor2->SetMarkerSize(0.5);
    g_Cor2->SetMarkerColor(1);
    g_Cor2->SetLineColor(1);
    hCorUncertainty->SetMaximum(4.3);
    hCorUncertainty->SetMinimum(0.7);
    hCorUncertainty->Draw("E3");
    hCorUncertainty2->Draw("E3same");
    g_Cor->Draw("Psame"); 
    g_Cor2->Draw("Psame"); 
    CorFit->Draw("Lsame"); 
    MCOverCor->Draw("Lsame"); 
    MCOverCor2->SetLineColor(1);
    MCOverCor2->Draw("Lsame"); 
    TLegend *leg = new TLegend(0.5,0.65,0.89,0.89);
    leg->AddEntry(CorFit,"Z#rightarrow e^{+}e^{-}+Jet Correction","L");
    leg->AddEntry(CorFit2,"#gamma+Jet Correction","L");
    leg->AddEntry(MCOverCor,"Transformation function: Zee+Jet","L");
    leg->AddEntry(MCOverCor2,"Transformation function: #gamma+Jet","L");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw();
  }

  if(plotCase==2) {
    new_gr->SetLineColor(2);
    new_gr->SetMarkerColor(2);
    new_gr->SetFillColor(2);
    new_gr->SetMaximum(4);
    hCorrErr->Draw("E3");
    new_gr->Draw("P");
    new_gr2->Draw("P"); 
    MCcurve->Draw("Lsame");
    C_all->Draw("Lsame");
    TLegend *leg = new TLegend(0.5,0.65,0.89,0.89);
    leg->AddEntry(new_gr,"Measurement from Z#rightarrow e^{+}e^{-}+Jet","LP");
    leg->AddEntry(new_gr2,"Measurement from #gamma+Jet","LP");
    leg->AddEntry(C_all,"Curve from fitting the data points","LP");
    leg->AddEntry(MCcurve,"Curve from dijet MC-truth","L");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw();
  }


  if(plotCase==3) {
    newhCorUncertainty->SetMaximum(4.3);
    newhCorUncertainty->Draw("E3");
    new_gr->Draw("Psame"); 
    new_gr->Fit(MCcurve,"RQ");
    MCcurve->SetLineColor(2);
    MCcurve->Draw("Lsame");
    TLegend *leg = new TLegend(0.5,0.65,0.89,0.89);
    leg->AddEntry(g_Cor,
		  "L3 measurement from Z#rightarrow e^{+}e^{-}+Jet","LP");
    leg->AddEntry(MCcurve,"fit","L");
    leg->AddEntry(newhCorUncertainty,"fit uncertainty","F");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw();
  }

  if(plotCase==4) {
    hCorrErr_10pc->SetMaximum(4.8);
    hCorrErr_10pc->Draw("E3");
    gr_10pc->Draw("Psame"); 
    C_10pc->Draw("Lsame");
    MCcurve->Draw("Lsame");
    TLegend *leg = new TLegend(0.5,0.65,0.89,0.89);
    leg->AddEntry(gr_10pc,"Measurement from data","LP");
    leg->AddEntry(C_10pc,"Curve from fitting the data points","LP");
    leg->AddEntry(MCcurve,"Curve from dijet MC-truth","L");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw();
  }

  if(plotCase==5) {
    hCorrErr_linear->SetMaximum(5.05);
    hCorrErr_linear->Draw("E3");
    gr_linear->Draw("Psame"); 
    C_linear->Draw("Lsame");
    MCcurve->Draw("Lsame");
    TLegend *leg = new TLegend(0.5,0.65,0.89,0.89);
    leg->AddEntry(gr_10pc,"Measurement from data","LP");
    leg->AddEntry(C_10pc,"Curve from fitting the data points","LP");
    leg->AddEntry(MCcurve,"Curve from dijet MC-truth","L");
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->Draw();
  }

}

// ---------------------------------------------------






//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

double FitUncertainty( TF1* f, TMatrixD* COV, double x) {
  int i,j;
  double df,sum,y,z,x;
  double PartialDerivative[10],Parameter[10];
  int npar = 4;
  int N = f->GetNumberFreeParameters();
  int dim = COV->GetNrows();

  if (dim != npar || N != npar)
    {
      cout<<"ERROR: wrong number of parameters !!!!"<<endl;
      return(-1);
    }  
  for(i=0;i<npar;i++)
    Parameter[i] = f->GetParameter(i);
  z = pow(log10(x),Parameter[2]);  
  PartialDerivative[0] = 1.;
  PartialDerivative[1] = 1./(z+Parameter[3]);
  PartialDerivative[3] = -Parameter[1]/pow(z+Parameter[3],2);
  PartialDerivative[2] = PartialDerivative[3]*log(log10(x))*z;
  sum = 0.;
  for(i=0;i<npar;i++)
    for(j=0;j<npar;j++)
      {
        y = PartialDerivative[i]*PartialDerivative[j]*COV(i,j);
        sum+=y;
      }
  df = sqrt(sum);
  return df;
}






//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

TGraphErrors* MergeGraph(const TGraphErrors& g1, 
		    const TGraphErrors& g2) {
  vector<double> x;
  vector<double> y;
  vector<double> xerr;
  vector<double> yerr;
 
  for ( int i = 0; i < g1.GetN(); ++i ) {
    x.push_back(g1.GetX()[i]);
    xerr.push_back(g1.GetEX()[i]);
    y.push_back(g1.GetY()[i]);
    yerr.push_back(g1.GetEY()[i]);
  }

  for ( int i = 0; i < g2.GetN(); ++i ) {
    x.push_back(g2.GetX()[i]);
    xerr.push_back(g2.GetEX()[i]);
    y.push_back(g2.GetY()[i]);
    yerr.push_back(g2.GetEY()[i]);
  }
  
  TGraphErrors* g = new TGraphErrors(x.size(),
				     &(x[0]),&(y[0]),
                                     &(xerr[0]),&(yerr[0]));
  g->SetName(g1.GetName());
  return g;  
}




//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
TGraphErrors* ScaleGraph(const TGraphErrors& g, 
			 const TF1& f) {

  vector<double> x;
  vector<double> y;
  vector<double> xerr;
  vector<double> yerr;

  for(int i=0; i<g.GetN(); ++i) {
    double xi = g.GetX()[i];
    x.push_back(xi);
    xerr.push_back( g.GetEX()[i] );
    y.push_back( g.GetY()[i] * f.Eval(xi) );
    yerr.push_back( g.GetEY()[i] * f.Eval(xi) ); 
  }

  TGraphErrors* new_gr = new TGraphErrors(x.size(),
				     &(x[0]),&(y[0]),
                                     &(xerr[0]),&(yerr[0]));
  new_gr->SetName(g.GetName());
  new_gr->SetTitle("");
  new_gr->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  new_gr->GetYaxis()->SetTitle("Correction factor");
  return new_gr;  
}



//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
TGraphErrors* ScaleGraph(const TGraphErrors& g, 
			 const double f) {

  vector<double> x;
  vector<double> y;
  vector<double> xerr;
  vector<double> yerr;

  for(int i=0; i<g.GetN(); ++i) {
    double xi = g.GetX()[i];
    x.push_back(xi);
    xerr.push_back( g.GetEX()[i] );
    y.push_back( g.GetY()[i] * f );
    yerr.push_back( g.GetEY()[i] * f ); 
  }

  TGraphErrors* new_gr = new TGraphErrors(x.size(),
				     &(x[0]),&(y[0]),
                                     &(xerr[0]),&(yerr[0]));
  new_gr->SetName(g.GetName());
  new_gr->SetTitle("");
  new_gr->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  new_gr->GetYaxis()->SetTitle("Correction factor");
  return new_gr;  
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
TF1* TransformationFunction(const TF1& f) {
  
  // This is from dijet MC
  TF1 f0("f0","[0]+[1]/(pow(log10(x),[2])+[3])", rMin, rMax); 
  f0.SetParameter(0, 0.996998);
  f0.SetParameter(1, 4.39412);
  f0.SetParameter(2, 2.96134);
  f0.SetParameter(3, 1.69966);
  f0.SetLineColor(4);
  f0.SetLineStyle(1);
  f0.SetLineWidth(2);
 

  // This is from X+jet MC
  TF1 f1("f1","[0]+[1]/(pow(log10(x),[2])+[3])", rMin, rMax); 
  f1.SetParameter(0, f.GetParameter(0));
  f1.SetParameter(1, f.GetParameter(1));
  f1.SetParameter(2, f.GetParameter(2));
  f1.SetParameter(3, f.GetParameter(3));
  
  // take ratio
  TF1* tf = new TF1("tf","f0/f1", rMin, rMax); 
  tf->SetLineColor(2);
  tf->SetLineStyle(2);
  tf->SetLineWidth(2);

  tf->SetName(f.GetName());
  tf->SetLineStyle(2);
  tf->SetLineWidth(2);
  return tf;  
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

TH1F* Uncertainty(const TF1& f, TMatrixD& COV, 
		  char* name, Color_t color=5) {
  
  TH1F* h = new TH1F( name, "", rBin, rMin, rMax);
  double x,y,e;

  for(int i=0; i<rBin; i++) {
      x = h->GetBinCenter(i+1);
      y = f.Eval(x);
      e = FitUncertainty( &f, &COV, x );
      h->SetBinContent( i+1, y);
      h->SetBinError( i+1, e ); 
    }

  h->SetMinimum(1);
  h->GetYaxis()->SetNdivisions(505);
  h->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  h->GetYaxis()->SetTitle("Correction factor");  
  h->SetLineColor(5);
  h->SetFillColor(color);
  h->SetMarkerColor(color);  

  return h;  
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

TF1* Dijet_MC_Curve() {
  
  TF1* C = new TF1("C","[0]+[1]/(pow(log10(x),[2])+[3])", rMin, rMax); 
  C->SetParameter(0, 0.996998);
  C->SetParameter(1, 4.39412);
  C->SetParameter(2, 2.96134);
  C->SetParameter(3, 1.69966);
  C->SetLineColor(4);
  C->SetLineStyle(2);

  C->SetName("MCcurve");
  return C;
} 

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
