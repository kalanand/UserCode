const char HistoFilename[1024] = "../../ZJet/test/Histograms_ZjetResponse_ic5.root";
const char FitterFilename[1024] = "FitterResults_test_Icone5.root";
const char L3OutputROOTFilename[1024] = "L3Graphs_test_Icone5.root";
const char L2OutputROOTFilename[1024] = "L2Graphs_test_Icone5.root";
const char Algorithm[100] = "Icone5";
const char Version[1024] = "test"; 
const bool UseRatioForResponse = true;
const int NPtBins=17;
const int NETA = 1;
const double Pt[NPtBins+1] = {0,30,60,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,3000,5000};
const double eta_boundaries[NETA+1] = {-1.3,1.3};
/*
const double eta_boundaries[NETA+1] = {-5.191,-4.889,-4.716,-4.538,-4.363,-4.191,-4.013,-3.839,-3.664,-3.489,
-3.314,-3.139,-2.964,-2.853,-2.650,-2.500,-2.322,-2.172,-2.043,-1.930,
-1.830,-1.740,-1.653,-1.566,-1.479,-1.392,-1.305,-1.218,-1.131,-1.044,
-0.957,-0.879,-0.783,-0.696,-0.609,-0.522,-0.435,-0.348,-0.261,-0.174,
-0.087,0.000,0.087,0.174,0.261,0.348,0.435,0.522,0.609,0.696,
0.783,0.879,0.957,1.044,1.131,1.218,1.305,1.392,1.479,1.566,
1.653,1.740,1.830,1.930,2.043,2.172,2.322,2.500,2.650,2.853,
2.964,3.139,3.314,3.489,3.664,3.839,4.013,4.191,4.363,4.538,4.716,4.889,5.191};
*/
