#ifndef USERDEFINEDDATA_H
#define USERDEFINEDDATA_H

const int NGenPtBins=21;
const double GenPt[NGenPtBins+1] = {0,15,20,30,50,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,2600,3000,3500,5000};

const char JetResponseFilename[1024]     = "Histograms_ZjetResponse.root"; 
const char FitterOutputFilename[1024]    = "FitterResults_ZjetResponse.root";
const char L3OutputROOTFilename[1024]    = "L3Graphs_ZjetResponse.root";
const char L3ResponseFilename[1024]      = "L3Response_ZjetResponse.txt";
const char L3CorrectionFilename[1024]    = "L3AbsoluteCorrection_ZjetResponse.txt";

#endif
