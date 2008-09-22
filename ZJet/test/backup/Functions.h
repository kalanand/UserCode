#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
using namespace std;

void GetMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma);
void GetClosureMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma);
void GetMEAN(TH1F* histo, double& peak, double& error, double& sigma);
void CalculateResponse(double GenPt, double eGenPt, double DPt, double eDPt, double& r, double& e);
void CalculateCorrection(double CaloPt, double eCaloPt, double DPt, double eDPt, double& c, double& e);
void Invert(TF1* f, double Min, double Max, double y, double& x);

#endif
