#include <iostream>
#include <vector>

using namespace std;


// ********* Input and output file names ******************* //
// const double MINMASS = 85.0; // min. value of Mee
// const double MAXMASS = 95.0; // max. value of Mee
// const double METCUTMAX = 20.0; // max. MET cut

// char* inputFileName = "../allTPtrees_35invpb.root";
// char* inputTreeName = "GsfToIso/fitter_tree";
// char* outputFileName = "signal.root";
// const bool Running_Over_Background = false;


const double MINMASS = 0.0; // min. value of Mee
const double MAXMASS = 10000.0; // max. value of Mee
const double METCUTMAX = 10000.0; // max. MET cut

char* inputFileName = "../TPtrees_Jet_PromptReco_All.root";
char* inputTreeName = "GsfGsfToIso/fitter_tree";
char* outputFileName = "background.root";
const bool Running_Over_Background = true;


// ********* List energy scale here ******************* //
const double ebScale = 1.0045; // EB energy scale
const double eeScale = 1.045;  // EE energy scale


// ********* List all cuts here ******************* //
// WP95 cuts
const float MAX_MissingHits_WP95      = 1.0;
const float MIN_Dist_WP95             = 0.0;
const float MIN_Dcot_WP95             = 0.0;

const float cut_EB_trackRel03_WP95    = 0.15;
const float cut_EB_ecalRel03_WP95     = 2.00;
const float cut_EB_hcalRel03_WP95     = 0.12;
const float cut_EB_sigmaIetaIeta_WP95 = 0.01;
const float cut_EB_deltaPhi_WP95      = 0.8;
const float cut_EB_deltaEta_WP95      = 0.007;
const float cut_EB_HoverE_WP95        = 0.15;

const float cut_EE_trackRel03_WP95    = 0.08;
const float cut_EE_ecalRel03_WP95     = 0.06;
const float cut_EE_hcalRel03_WP95     = 0.05;
const float cut_EE_sigmaIetaIeta_WP95 = 0.03;
const float cut_EE_deltaPhi_WP95      = 0.7;
const float cut_EE_deltaEta_WP95      = 0.01; // = 1000000.0; no cut in 3.6.X
const float cut_EE_HoverE_WP95        = 0.07;


// WP90 cuts
const float MAX_MissingHits_WP90      = 1.0;
const float MIN_Dist_WP90             = 0.02;
const float MIN_Dcot_WP90             = 0.02;

const float cut_EB_trackRel03_WP90    = 0.12;
const float cut_EB_ecalRel03_WP90     = 0.09;
const float cut_EB_hcalRel03_WP90     = 0.10;
const float cut_EB_sigmaIetaIeta_WP90 = 0.01;
const float cut_EB_deltaPhi_WP90      = 0.8;
const float cut_EB_deltaEta_WP90      = 0.007;
const float cut_EB_HoverE_WP90        = 0.12;

const float cut_EE_trackRel03_WP90    = 0.05;
const float cut_EE_ecalRel03_WP90     = 0.06;
const float cut_EE_hcalRel03_WP90     = 0.03;
const float cut_EE_sigmaIetaIeta_WP90 = 0.03;
const float cut_EE_deltaPhi_WP90      = 0.7;
const float cut_EE_deltaEta_WP90      = 0.009; // = 1000000.0; no cut in 3.6.X
const float cut_EE_HoverE_WP90        = 0.05;



// WP80 cuts
const float MAX_MissingHits_WP80      = 0.0;
const float MIN_Dist_WP80             = 0.02;
const float MIN_Dcot_WP80             = 0.02;

const float cut_EB_trackRel03_WP80    = 0.09;
const float cut_EB_ecalRel03_WP80     = 0.07;
const float cut_EB_hcalRel03_WP80     = 0.10;
const float cut_EB_sigmaIetaIeta_WP80 = 0.01;
const float cut_EB_deltaPhi_WP80      = 0.06;
const float cut_EB_deltaEta_WP80      = 0.004;
const float cut_EB_HoverE_WP80        = 0.04;

const float cut_EE_trackRel03_WP80    = 0.04;
const float cut_EE_ecalRel03_WP80     = 0.05;
const float cut_EE_hcalRel03_WP80     = 0.025;
const float cut_EE_sigmaIetaIeta_WP80 = 0.03;
const float cut_EE_deltaPhi_WP80      = 0.03;
const float cut_EE_deltaEta_WP80      = 0.007; // = 1000000.0;  no cut in 3.6.X
const float cut_EE_HoverE_WP80        = 0.025;


// WP70 cuts
const float MAX_MissingHits_WP70      = 0.0;
const float MIN_Dist_WP70             = 0.02;
const float MIN_Dcot_WP70             = 0.02;

const float cut_EB_trackRel03_WP70    = 0.05;
const float cut_EB_ecalRel03_WP70     = 0.06;
const float cut_EB_hcalRel03_WP70     = 0.03;
const float cut_EB_sigmaIetaIeta_WP70 = 0.01;
const float cut_EB_deltaPhi_WP70      = 0.03;
const float cut_EB_deltaEta_WP70      = 0.004;
const float cut_EB_HoverE_WP70        = 0.025;

const float cut_EE_trackRel03_WP70    = 0.025;
const float cut_EE_ecalRel03_WP70     = 0.025;
const float cut_EE_hcalRel03_WP70     = 0.02;
const float cut_EE_sigmaIetaIeta_WP70 = 0.03;
const float cut_EE_deltaPhi_WP70      = 0.02;
const float cut_EE_deltaEta_WP70      = 0.005; // = 1000000.0;  no cut in 3.6.X
const float cut_EE_HoverE_WP70        = 0.025;



// WP60 cuts
const float MAX_MissingHits_WP60      = 0.0;
const float MIN_Dist_WP60             = 0.02;
const float MIN_Dcot_WP60             = 0.02;

const float cut_EB_trackRel03_WP60    = 0.04;
const float cut_EB_ecalRel03_WP60     = 0.04;
const float cut_EB_hcalRel03_WP60     = 0.03;
const float cut_EB_sigmaIetaIeta_WP60 = 0.01;
const float cut_EB_deltaPhi_WP60      = 0.025;
const float cut_EB_deltaEta_WP60      = 0.004;
const float cut_EB_HoverE_WP60        = 0.025;

const float cut_EE_trackRel03_WP60    = 0.025;
const float cut_EE_ecalRel03_WP60     = 0.02;
const float cut_EE_hcalRel03_WP60     = 0.02;
const float cut_EE_sigmaIetaIeta_WP60 = 0.03;
const float cut_EE_deltaPhi_WP60      = 0.02;
const float cut_EE_deltaEta_WP60      = 0.005; // = 1000000.0;  no cut in 3.6.X
const float cut_EE_HoverE_WP60        = 0.025;

// CiC cut values
const int ncuts = 9;
const int nvars = 15;
double cutdeta[6][ncuts];  
double cutdetal[6][ncuts]; 
double cutdphi[6][ncuts];  
double cutdphil[6][ncuts]; 
double cuteopin[6][ncuts]; 
double cutmishits[6][ncuts]; 
double cuthoe[6][ncuts]; 
double cuthoel[6][ncuts]; 
double cutip[6][ncuts]; 
double cutipl[6][ncuts]; 
double cutIsoSum[6][ncuts]; 
double cutIsoSumCorr[6][ncuts];
double cutIsoSumCorrl[6][ncuts];
double cutsee[6][ncuts];
double cutseel[6][ncuts];


// ****** Define your selection: "WP95" or "WP80" *********** //
   
void makeSignalElectronSample(char* selection = "WP80") //char* selection = "WP95";
{
   TFile fin(inputFileName, "read");
   TTree* in_tree = (TTree*) fin.Get( inputTreeName );
   fillCiCcutValues();
 
  TFile newfile( outputFileName, "RECREATE");
  TTree *newtree = in_tree->CloneTree(0);


  Long64_t nentries = in_tree->GetEntries();
  Float_t mass, tag_sc_eta, probe_sc_eta, tag_sc_phi,probe_sc_et,probe_sc_phi, 
     tag_gsfEle_dcot, tag_gsfEle_dist,
     probe_gsfEle_dcot, probe_gsfEle_dist,
     tag_gsfEle_deltaPhi, tag_gsfEle_deltaEta, tag_gsfEle_HoverE,
     tag_gsfEle_trackiso_dr03, tag_gsfEle_et, tag_gsfEle_ecaliso_dr03, 
     tag_gsfEle_hcaliso_dr03, tag_sc_et, probe_sc_et,
     tag_gsfEle_ecalDrivenSeed, probe_gsfEle_ecalDrivenSeed,
     tag_gsfEle_missingHits, probe_gsfEle_missingHits, 
     tag_gsfEle_isEB, tag_gsfEle_isEE, probe_gsfEle_isEB,probe_gsfEle_isEE,
     probe_gsfEle_deltaPhi, probe_gsfEle_deltaEta, probe_gsfEle_HoverE,
     probe_gsfEle_trackiso_dr03, probe_gsfEle_et, probe_gsfEle_ecaliso_dr03, 
     probe_gsfEle_hcaliso_dr03, probe_gsfEle_sigmaIetaIeta, 
     probe_gsfEle_vx, probe_gsfEle_vy, probe_gsfEle_EoverP,
     probe_gsfEle_EoverPout, probe_gsfEle_bremFraction,
     probe_gsfEle_px, probe_gsfEle_py, probe_gsfEle_pt,
     tag_gsfEle_sigmaIetaIeta, event_met_pfmet; 

  Int_t probe_passingId80;
  UInt_t run,lumi,event;

  float  probe_gsfEle_d0, probe_myLikelihood, probe_myBDTresponse;
  bool isWP95=false, isWP90=false, isWP80=false, isWP70=false, isWP60=false;
  bool isCiCVeryLoose=false, isCiCLoose=false, isCiCMedium=false, isCiCTight=false, 
     isCiCSuperTight=false, isCiCHyperTight=false;

  newtree->Branch("probe_isWP95", &isWP95,"probe_isWP95/O");
  newtree->Branch("probe_isWP90", &isWP90,"probe_isWP90/O");
  newtree->Branch("probe_isWP80", &isWP80,"probe_isWP80/O");
  newtree->Branch("probe_isWP70", &isWP70,"probe_isWP70/O");
  newtree->Branch("probe_isWP60", &isWP60,"probe_isWP60/O");
  newtree->Branch("probe_isCiCVeryLoose", &isCiCVeryLoose,"probe_isCiCVeryLoose/O");
  newtree->Branch("probe_isCiCLoose", &isCiCLoose,"probe_isCiCLoose/O");
  newtree->Branch("probe_isCiCMedium", &isCiCMedium,"probe_isCiCMedium/O");
  newtree->Branch("probe_isCiCTight", &isCiCTight,"probe_isCiCTight/O");
  newtree->Branch("probe_isCiCSuperTight", &isCiCSuperTight,"probe_isCiCSuperTight/O");
  newtree->Branch("probe_isCiCHyperTight", &isCiCHyperTight,"probe_isCiCHyperTight/O");
  newtree->Branch("probe_gsfEle_d0", &probe_gsfEle_d0,"probe_gsfEle_d0/F");
  newtree->Branch("probe_myLikelihood", &probe_myLikelihood,"probe_myLikelihood/F");
  newtree->Branch("probe_myBDTresponse", &probe_myBDTresponse,"probe_myBDTresponse/F");

  in_tree->SetBranchAddress("mass", &mass);
  in_tree->SetBranchAddress("tag_sc_eta", &tag_sc_eta);
  in_tree->SetBranchAddress("probe_sc_eta", &probe_sc_eta);
  in_tree->SetBranchAddress("tag_gsfEle_ecalDrivenSeed", &tag_gsfEle_ecalDrivenSeed);
  in_tree->SetBranchAddress("probe_gsfEle_ecalDrivenSeed", &probe_gsfEle_ecalDrivenSeed);
  if(!Running_Over_Background) {
     in_tree->SetBranchAddress("tag_dcot", &tag_gsfEle_dcot);
     in_tree->SetBranchAddress("tag_dist", &tag_gsfEle_dist);
     in_tree->SetBranchAddress("probe_dcot", &probe_gsfEle_dcot);
     in_tree->SetBranchAddress("probe_dist", &probe_gsfEle_dist);
  }
  in_tree->SetBranchAddress("tag_gsfEle_missingHits", &tag_gsfEle_missingHits);
  in_tree->SetBranchAddress("probe_gsfEle_missingHits", &probe_gsfEle_missingHits);
  in_tree->SetBranchAddress("tag_gsfEle_deltaPhi", &tag_gsfEle_deltaPhi);
  in_tree->SetBranchAddress("tag_gsfEle_deltaEta", &tag_gsfEle_deltaEta);
  in_tree->SetBranchAddress("tag_gsfEle_HoverE", &tag_gsfEle_HoverE);
  in_tree->SetBranchAddress("tag_gsfEle_trackiso_dr03", &tag_gsfEle_trackiso_dr03);
  in_tree->SetBranchAddress("tag_gsfEle_et", &tag_gsfEle_et);
  in_tree->SetBranchAddress("tag_gsfEle_ecaliso_dr03", &tag_gsfEle_ecaliso_dr03);
  in_tree->SetBranchAddress("tag_gsfEle_hcaliso_dr03", &tag_gsfEle_hcaliso_dr03);
  in_tree->SetBranchAddress("tag_sc_et", &tag_sc_et);
  in_tree->SetBranchAddress("probe_sc_et", &probe_sc_et);
  in_tree->SetBranchAddress("tag_gsfEle_isEB", &tag_gsfEle_isEB);
  in_tree->SetBranchAddress("tag_gsfEle_isEE", &tag_gsfEle_isEE);
  in_tree->SetBranchAddress("probe_gsfEle_isEB", &probe_gsfEle_isEB);
  in_tree->SetBranchAddress("probe_gsfEle_isEE", &probe_gsfEle_isEE);
  in_tree->SetBranchAddress("probe_gsfEle_deltaPhi", &probe_gsfEle_deltaPhi);
  in_tree->SetBranchAddress("probe_gsfEle_deltaEta", &probe_gsfEle_deltaEta);
  in_tree->SetBranchAddress("probe_gsfEle_HoverE", &probe_gsfEle_HoverE);
  in_tree->SetBranchAddress("probe_gsfEle_et", &probe_gsfEle_et);
  in_tree->SetBranchAddress("probe_gsfEle_trackiso_dr03", &probe_gsfEle_trackiso_dr03);
  in_tree->SetBranchAddress("probe_gsfEle_ecaliso_dr03", &probe_gsfEle_ecaliso_dr03);
  in_tree->SetBranchAddress("probe_gsfEle_hcaliso_dr03", &probe_gsfEle_hcaliso_dr03);
  in_tree->SetBranchAddress("probe_gsfEle_sigmaIetaIeta", &probe_gsfEle_sigmaIetaIeta);
  in_tree->SetBranchAddress("probe_gsfEle_vx", &probe_gsfEle_vx);
  in_tree->SetBranchAddress("probe_gsfEle_vy", &probe_gsfEle_vy);
  in_tree->SetBranchAddress("probe_gsfEle_EoverPout", &probe_gsfEle_EoverPout); 
  in_tree->SetBranchAddress("probe_gsfEle_EoverP", &probe_gsfEle_EoverP); 
  in_tree->SetBranchAddress("probe_gsfEle_bremFraction", &probe_gsfEle_bremFraction);
  in_tree->SetBranchAddress("probe_gsfEle_px", &probe_gsfEle_px);
  in_tree->SetBranchAddress("probe_gsfEle_py", &probe_gsfEle_py);
  in_tree->SetBranchAddress("probe_gsfEle_pt", &probe_gsfEle_pt);
  in_tree->SetBranchAddress("tag_gsfEle_sigmaIetaIeta", &tag_gsfEle_sigmaIetaIeta);

  in_tree->SetBranchAddress("run", &run);
  in_tree->SetBranchAddress("lumi", &lumi);
  in_tree->SetBranchAddress("event", &event);
  in_tree->SetBranchAddress("tag_sc_phi", &tag_sc_phi);
  in_tree->SetBranchAddress("probe_sc_et", &probe_sc_et);
  in_tree->SetBranchAddress("probe_sc_phi", &probe_sc_phi);
  in_tree->SetBranchAddress( "event_met_pfmet", &event_met_pfmet);


  std::vector<std::string> inputVars;
  inputVars.push_back("eta");
  inputVars.push_back("deta");
  inputVars.push_back("dphi");
  inputVars.push_back("sieie");
  inputVars.push_back("hoe");
  inputVars.push_back("trackiso");
  inputVars.push_back("ecaliso");
  inputVars.push_back("hcaliso");

  gSystem->Load( "weights/TMVAClassification_Likelihood.class_C.so");
  ReadLikelihood lhReader(inputVars);
  gSystem->Load( "weights/TMVAClassification_BDT.class_C.so");
  ReadBDT bdtReader(inputVars);


  for (Int_t i = 0; i < nentries; i++) {
    in_tree->GetEntry(i);

    if( abs(tag_gsfEle_ecalDrivenSeed)==0 ) continue;
    if( abs(probe_gsfEle_ecalDrivenSeed)==0 ) continue;
    if(event_met_pfmet > METCUTMAX) continue;


// require ET>20 GeV
    if(tag_gsfEle_isEB) {
       tag_sc_et = ebScale*tag_sc_et;
       tag_gsfEle_et = ebScale*tag_gsfEle_et;
    }
    else if(tag_gsfEle_isEE) {
       tag_sc_et = eeScale*tag_sc_et;
       tag_gsfEle_et = eeScale*tag_gsfEle_et;
    }
    if(probe_gsfEle_isEB) {
       probe_sc_et = ebScale*probe_sc_et;
       probe_gsfEle_et = ebScale*probe_gsfEle_et;
    }
    else if(probe_gsfEle_isEE) {
       probe_sc_et = eeScale*probe_sc_et;
       probe_gsfEle_et = eeScale*probe_gsfEle_et;
    }
    if(tag_sc_et<20.0  || probe_sc_et<20.0 ) continue;


// all tag cuts
    bool select = true;
    if( !Running_Over_Background)
       select = isCutBasedVBTF( selection, tag_gsfEle_et, tag_gsfEle_isEB, 
       tag_gsfEle_missingHits, tag_gsfEle_dcot, tag_gsfEle_dist,
       tag_gsfEle_deltaPhi, tag_gsfEle_deltaEta, 
       tag_gsfEle_HoverE, tag_gsfEle_trackiso_dr03, tag_gsfEle_ecaliso_dr03, 
       tag_gsfEle_hcaliso_dr03, tag_gsfEle_sigmaIetaIeta);


// mass cut 
    if(tag_gsfEle_isEB && probe_gsfEle_isEB)  mass = ebScale*mass; 
    if(tag_gsfEle_isEE && probe_gsfEle_isEE)  mass = eeScale*mass; 
    if( (tag_gsfEle_isEB && probe_gsfEle_isEE) || 
    (tag_gsfEle_isEE && probe_gsfEle_isEB) ) 
       mass = sqrt(ebScale*eeScale)*mass;    
    if( mass<MINMASS || mass>MAXMASS ) select = false; 

    float et = probe_gsfEle_et;
    float eta = probe_sc_eta;
    float isEB = probe_gsfEle_isEB;
    float dphi = probe_gsfEle_deltaPhi;
    float deta = probe_gsfEle_deltaEta;
    float mishits = probe_gsfEle_missingHits;
    float dcot = probe_gsfEle_dcot;
    float dist = probe_gsfEle_dist;
    float tkiso = probe_gsfEle_trackiso_dr03 / et;
    float eciso = probe_gsfEle_ecaliso_dr03 / et;
    float hciso = probe_gsfEle_hcaliso_dr03 / et;
    float see = probe_gsfEle_sigmaIetaIeta;
    float hoe = probe_gsfEle_HoverE;

    float vx = probe_gsfEle_vx;
    float vy = probe_gsfEle_vy;
    float px = probe_gsfEle_px;
    float py = probe_gsfEle_py;
    float pt = probe_gsfEle_pt;
    float ip = ( - vx*py + vy*px ) / pt;


    float esop = probe_gsfEle_EoverPout;
    float fbrem = probe_gsfEle_bremFraction;
    float eOverP = probe_gsfEle_EoverP;

    std::vector<double> tmvaInputVals;
    tmvaInputVals.push_back(eta);
    tmvaInputVals.push_back(deta);
    tmvaInputVals.push_back(dphi);
    tmvaInputVals.push_back(see);
    tmvaInputVals.push_back(hoe);
    tmvaInputVals.push_back(tkiso);
    tmvaInputVals.push_back(eciso);
    tmvaInputVals.push_back(hciso);

    probe_myLikelihood = (float) lhReader.GetMvaValue(tmvaInputVals );
    probe_myBDTresponse = (float) bdtReader.GetMvaValue(tmvaInputVals );

    probe_gsfEle_d0 = fabs(ip);

    isWP95 = isCutBasedVBTF( "WP95", et, isEB, mishits, dcot, 
             dist, dphi, deta, hoe, tkiso, eciso, hciso, see); 
    isWP90 = isCutBasedVBTF( "WP90", et, isEB, mishits, dcot, 
             dist, dphi, deta, hoe, tkiso, eciso, hciso, see);
    isWP80 = isCutBasedVBTF( "WP80", et, isEB, mishits, dcot, 
             dist, dphi, deta, hoe, tkiso, eciso, hciso, see);
    isWP70 = isCutBasedVBTF( "WP70", et, isEB, mishits, dcot, 
             dist, dphi, deta, hoe, tkiso, eciso, hciso, see);
    isWP60 = isCutBasedVBTF( "WP60",et, isEB, mishits, dcot, 
             dist, dphi, deta, hoe, tkiso, eciso, hciso, see); 


    tkiso = probe_gsfEle_trackiso_dr03;
    eciso = probe_gsfEle_ecaliso_dr03;
    hciso = probe_gsfEle_hcaliso_dr03;

    isCiCVeryLoose  = isCiC( "veryloose", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP); 
    isCiCLoose      = isCiC( "loose", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP); 
    isCiCMedium     = isCiC( "medium", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP); 
    isCiCTight      = isCiC( "tight", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP); 
    isCiCSuperTight = isCiC( "supertight", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP); 
    isCiCHyperTight = isCiC( "hypertight", et, eta, esop, fbrem, 
    mishits, ip, dphi, deta, hoe, tkiso, eciso, hciso, see, eOverP);
    
// now we have selected events
/*
    if(select) {
       printf("%.4f %.6f %.6f %.7f %.7f %.7f %.7f %.4f %.4f %.4f\n", 
       probe_sc_et, eta,probe_sc_phi, deta, dphi, see, hoe, 
       tkiso, eciso, hciso);        
    }
*/
    if(select) newtree->Fill();

  }

  newfile.cd();
  newtree->Write("tree");
  newfile .Close();
}




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool isCutBasedVBTF( char* level, float et, float isEB, 
             float missingHits, float dcot, float dist,
             float deltaPhi, float deltaEta, 
             float HoverE, float trackiso, float ecaliso, 
             float hcaliso, float sigmaIetaIeta) {


   if(Running_Over_Background) {
      dcot = -1000.;
      dist = -1000.;
   }
   // ********* List all cuts here ******************* //
   //////////////////// default is WP95 //////////////////

   float MAX_MissingHits      = MAX_MissingHits_WP95;
   float MIN_Dist             = MIN_Dist_WP95;
   float MIN_Dcot             = MIN_Dcot_WP95;

   float cut_EB_trackRel03    = cut_EB_trackRel03_WP95;
   float cut_EB_ecalRel03     = cut_EB_ecalRel03_WP95;
   float cut_EB_hcalRel03     = cut_EB_hcalRel03_WP95;
   float cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP95;
   float cut_EB_deltaPhi      = cut_EB_deltaPhi_WP95;
   float cut_EB_deltaEta      = cut_EB_deltaEta_WP95;
   float cut_EB_HoverE        = cut_EB_HoverE_WP95;

   float cut_EE_trackRel03    = cut_EE_trackRel03_WP95;
   float cut_EE_ecalRel03     = cut_EE_ecalRel03_WP95;
   float cut_EE_hcalRel03     = cut_EE_hcalRel03_WP95;
   float cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP95;
   float cut_EE_deltaPhi      = cut_EE_deltaPhi_WP95;
   float cut_EE_deltaEta      = cut_EE_deltaEta_WP95; 
   float cut_EE_HoverE        = cut_EE_HoverE_WP95;

   ////////////////////
   if(level == "WP95") {
      MAX_MissingHits      = MAX_MissingHits_WP95;
      MIN_Dist             = MIN_Dist_WP95;
      MIN_Dcot             = MIN_Dcot_WP95;

      cut_EB_trackRel03    = cut_EB_trackRel03_WP95;
      cut_EB_ecalRel03     = cut_EB_ecalRel03_WP95;
      cut_EB_hcalRel03     = cut_EB_hcalRel03_WP95;
      cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP95;
      cut_EB_deltaPhi      = cut_EB_deltaPhi_WP95;
      cut_EB_deltaEta      = cut_EB_deltaEta_WP95;
      cut_EB_HoverE        = cut_EB_HoverE_WP95;

      cut_EE_trackRel03    = cut_EE_trackRel03_WP95;
      cut_EE_ecalRel03     = cut_EE_ecalRel03_WP95;
      cut_EE_hcalRel03     = cut_EE_hcalRel03_WP95;
      cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP95;
      cut_EE_deltaPhi      = cut_EE_deltaPhi_WP95;
      cut_EE_deltaEta      = cut_EE_deltaEta_WP95; 
      cut_EE_HoverE        = cut_EE_HoverE_WP95;
   }
   ////////////////////
   if(level == "WP90") {
      MAX_MissingHits      = MAX_MissingHits_WP90;
      MIN_Dist             = MIN_Dist_WP90;
      MIN_Dcot             = MIN_Dcot_WP90;

      cut_EB_trackRel03    = cut_EB_trackRel03_WP90;
      cut_EB_ecalRel03     = cut_EB_ecalRel03_WP90;
      cut_EB_hcalRel03     = cut_EB_hcalRel03_WP90;
      cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP90;
      cut_EB_deltaPhi      = cut_EB_deltaPhi_WP90;
      cut_EB_deltaEta      = cut_EB_deltaEta_WP90;
      cut_EB_HoverE        = cut_EB_HoverE_WP90;

      cut_EE_trackRel03    = cut_EE_trackRel03_WP90;
      cut_EE_ecalRel03     = cut_EE_ecalRel03_WP90;
      cut_EE_hcalRel03     = cut_EE_hcalRel03_WP90;
      cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP90;
      cut_EE_deltaPhi      = cut_EE_deltaPhi_WP90;
      cut_EE_deltaEta      = cut_EE_deltaEta_WP90; 
      cut_EE_HoverE        = cut_EE_HoverE_WP90;
   }
   ////////////////////
   if(level == "WP80") {
      MAX_MissingHits      = MAX_MissingHits_WP80;
      MIN_Dist             = MIN_Dist_WP80;
      MIN_Dcot             = MIN_Dcot_WP80;

      cut_EB_trackRel03    = cut_EB_trackRel03_WP80;
      cut_EB_ecalRel03     = cut_EB_ecalRel03_WP80;
      cut_EB_hcalRel03     = cut_EB_hcalRel03_WP80;
      cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP80;
      cut_EB_deltaPhi      = cut_EB_deltaPhi_WP80;
      cut_EB_deltaEta      = cut_EB_deltaEta_WP80;
      cut_EB_HoverE        = cut_EB_HoverE_WP80;

      cut_EE_trackRel03    = cut_EE_trackRel03_WP80;
      cut_EE_ecalRel03     = cut_EE_ecalRel03_WP80;
      cut_EE_hcalRel03     = cut_EE_hcalRel03_WP80;
      cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP80;
      cut_EE_deltaPhi      = cut_EE_deltaPhi_WP80;
      cut_EE_deltaEta      = cut_EE_deltaEta_WP80; 
      cut_EE_HoverE        = cut_EE_HoverE_WP80;
   }
   ////////////////////
   if(level == "WP70") {
      MAX_MissingHits      = MAX_MissingHits_WP70;
      MIN_Dist             = MIN_Dist_WP70;
      MIN_Dcot             = MIN_Dcot_WP70;

      cut_EB_trackRel03    = cut_EB_trackRel03_WP70;
      cut_EB_ecalRel03     = cut_EB_ecalRel03_WP70;
      cut_EB_hcalRel03     = cut_EB_hcalRel03_WP70;
      cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP70;
      cut_EB_deltaPhi      = cut_EB_deltaPhi_WP70;
      cut_EB_deltaEta      = cut_EB_deltaEta_WP70;
      cut_EB_HoverE        = cut_EB_HoverE_WP70;

      cut_EE_trackRel03    = cut_EE_trackRel03_WP70;
      cut_EE_ecalRel03     = cut_EE_ecalRel03_WP70;
      cut_EE_hcalRel03     = cut_EE_hcalRel03_WP70;
      cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP70;
      cut_EE_deltaPhi      = cut_EE_deltaPhi_WP70;
      cut_EE_deltaEta      = cut_EE_deltaEta_WP70; 
      cut_EE_HoverE        = cut_EE_HoverE_WP70;
   }
   ////////////////////
   if(level == "WP60") {
      MAX_MissingHits      = MAX_MissingHits_WP60;
      MIN_Dist             = MIN_Dist_WP60;
      MIN_Dcot             = MIN_Dcot_WP60;

      cut_EB_trackRel03    = cut_EB_trackRel03_WP60;
      cut_EB_ecalRel03     = cut_EB_ecalRel03_WP60;
      cut_EB_hcalRel03     = cut_EB_hcalRel03_WP60;
      cut_EB_sigmaIetaIeta = cut_EB_sigmaIetaIeta_WP60;
      cut_EB_deltaPhi      = cut_EB_deltaPhi_WP60;
      cut_EB_deltaEta      = cut_EB_deltaEta_WP60;
      cut_EB_HoverE        = cut_EB_HoverE_WP60;

      cut_EE_trackRel03    = cut_EE_trackRel03_WP60;
      cut_EE_ecalRel03     = cut_EE_ecalRel03_WP60;
      cut_EE_hcalRel03     = cut_EE_hcalRel03_WP60;
      cut_EE_sigmaIetaIeta = cut_EE_sigmaIetaIeta_WP60;
      cut_EE_deltaPhi      = cut_EE_deltaPhi_WP60;
      cut_EE_deltaEta      = cut_EE_deltaEta_WP60; 
      cut_EE_HoverE        = cut_EE_HoverE_WP60;
   }
   // ************************************************ //

    bool passing = true;
    if( et<20.0 ) passing = false; 
    if( missingHits>MAX_MissingHits )  passing = false;
    if( fabs(dcot)<MIN_Dcot && fabs(dist)<MIN_Dist ) passing = false;

    if( abs(isEB) >0 ) {

       if( fabs(deltaPhi)>cut_EB_deltaPhi ) passing = false;
       if( fabs(deltaEta)>cut_EB_deltaEta ) passing = false;
       if( HoverE>cut_EB_HoverE ) passing = false;
       if( trackiso/et>cut_EB_trackRel03 ) passing = false;
       if( ecaliso/et>cut_EB_ecalRel03 ) passing = false;
       if( hcaliso/et>cut_EB_hcalRel03 ) passing = false;
       if( sigmaIetaIeta > cut_EB_sigmaIetaIeta ) passing = false;
    }
    else {
       if( fabs(deltaPhi)>cut_EE_deltaPhi ) passing = false;
       if( fabs(deltaEta)>cut_EE_deltaEta ) passing = false;
       if( HoverE>cut_EE_HoverE ) passing = false;
       if( trackiso/et>cut_EE_trackRel03 ) passing = false;
       if( ecaliso/et>cut_EE_ecalRel03 ) passing = false;
       if( hcaliso/et>cut_EE_hcalRel03 ) passing = false;
       if( sigmaIetaIeta > cut_EE_sigmaIetaIeta) passing = false;
    }
 
    return passing;
}





/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void fillCiCcutValues() {
   // system("./parseTest.sh cutsInCategoriesElectronIdentificationV06_cfi.py");
   for (int i=0; i<6; i++) fillCiCcutValues(i);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void fillCiCcutValues(int index) {
   char* filename= "veryLooseOut.txt";
   if(index==0)  filename = "veryLooseOut.txt"; 
   else if(index==1) filename = "looseOut.txt"; 
   else if(index==2) filename = "mediumOut.txt"; 
   else if(index==3) filename = "tightOut.txt"; 
   else if(index==4) filename = "superTightOut.txt"; 
   else if(index==5) filename = "hyperTight1Out.txt"; 


   ifstream fin;     // declare stream variable name
   fin.open(filename,ios::in);    // open file
   assert (!fin.fail( ));     
   double tempdouble=0.0;

   // ********* List all cuts here ******************* //

   for (int i = 0; i < nvars*ncuts; i++) {
      fin >> tempdouble;
      if( i<ncuts ) 
         cutdeta[index][i]= tempdouble;
      else if ( i < 2*ncuts) 
         cutdetal[index][i%ncuts]= tempdouble;
      else if ( i < 3*ncuts) 
         cutdphi[index][i%ncuts]= tempdouble;
      else if ( i < 4*ncuts) 
         cutdphil[index][i%ncuts]= tempdouble;
      else if ( i < 5*ncuts) 
         cuteopin[index][i%ncuts]= tempdouble;
      else if ( i < 6*ncuts) 
         cutmishits[index][i%ncuts]= tempdouble;
      else if ( i < 7*ncuts) 
         cuthoe[index][i%ncuts]= tempdouble;
      else if ( i < 8*ncuts) 
         cuthoel[index][i%ncuts]= tempdouble;
      else if ( i < 9*ncuts) 
         cutip[index][i%ncuts]= tempdouble;
      else if ( i < 10*ncuts) 
         cutipl[index][i%ncuts]= tempdouble;
      else if ( i < 11*ncuts) 
         cutIsoSum[index][i%ncuts]= tempdouble;
      else if ( i < 12*ncuts) 
         cutIsoSumCorr[index][i%ncuts]= tempdouble;
      else if ( i < 13*ncuts) 
         cutIsoSumCorrl[index][i%ncuts]= tempdouble;
      else if ( i < 14*ncuts) 
         cutsee[index][i%ncuts]= tempdouble;
      else if ( i < 15*ncuts) 
         cutseel[index][i%ncuts]= tempdouble;
   }

}


/////////////////////////////////////////////////////////////
bool isCiC( char* level, float et, float eta, float eSeedOverP, float fBrem,
             float missingHits, float ip,
             float deltaPhi, float deltaEta, 
             float HoverE, float trackiso, float ecaliso, 
             float hcaliso, float sigmaIetaIeta, float eOverP) {


   int index=0;
   if(level == "veryloose") index=0;
   else if(level == "loose") index=1;
   else if(level == "medium") index=2;
   else if(level == "tight") index=3;
   else if(level == "supertight") index=4;
   else if(level == "hypertight") index=5;

    int result = 0;
    bool cut_results[9];
    for(int count=0; count<9; count++) { cut_results[count] = false; }

    float iso_sum =trackiso + ecaliso + hcaliso;
    if( fabs(eta)>1.5 ) 
      iso_sum += (fabs(eta)-1.5)*1.09;
    
    float iso_sumoet = iso_sum*(40./et);
    float eseedopincor = eSeedOverP + fBrem;
    if(fBrem < 0)
      eseedopincor = eSeedOverP;
    int cat = classify(eta, eOverP, fBrem);

    for (int cut=0; cut<ncuts; cut++) {
      switch (cut) {
      case 0:
        cut_results[cut] = compute_cut(fabs(deltaEta), et, cutdetal[index][cat], cutdeta[index][cat]);
        break;
      case 1:
        cut_results[cut] = compute_cut(fabs(deltaPhi), et, cutdphil[index][cat], cutdphi[index][cat]);
        break;
      case 2:
        cut_results[cut] = (eseedopincor > cuteopin[index][cat]);
        break;
      case 3:
        cut_results[cut] = compute_cut(HoverE, et, cuthoel[index][cat], cuthoe[index][cat]);
        break;
      case 4:
        cut_results[cut] = compute_cut(sigmaIetaIeta, et, cutseel[index][cat], cutsee[index][cat]);
        break;
      case 5:
        cut_results[cut] = compute_cut(iso_sumoet, et, cutIsoSumCorrl[index][cat], cutIsoSumCorr[index][cat]);
        break;
      case 6:
        cut_results[cut] = (iso_sum < cutIsoSum[index][cat]);
        break;
      case 7:
        cut_results[cut] = compute_cut(fabs(ip), et, cutipl[index][cat], cutip[index][cat]);
        break;
      case 8:
        cut_results[cut] = (missingHits < cutmishits[index][cat]);
        break;
      }
    }

    // ID part: ignore seedcluster/p cut for now
    // if (cut_results[0] && cut_results[1] && cut_results[2] && cut_results[3] && cut_results[4])
    if (cut_results[0] && cut_results[1] && cut_results[3] && cut_results[4])
      result = result + 1;
    
    // ISO part
    if (cut_results[5] && cut_results[6])
      result = result + 2;
    
//     // IP part: ignore for now
//     if (cut_results[7])
//       result = result + 8;
    
    // Conversion part
    if (cut_results[8])
      result = result + 4;

    bool boolresult = false;
    if(result==7) boolresult = true;
    return boolresult;

}




///////////////////////////////////////////////////////////////////////////////////
bool compute_cut(double x, double et, double cut_min, double cut_max, bool gtn=false) {

  float et_min = 10;
  float et_max = 40;

  bool accept = false;
  float cut = cut_max; //  the cut at et=40 GeV

  if(et < et_max) {
    cut = cut_min + (1/et_min - 1/et)*(cut_max - cut_min)/(1/et_min - 1/et_max);
  } 
  
  if(et < et_min) {
    cut = cut_min;
  } 

  if(gtn) {   // useful for e/p cut which is gt
    accept = (x >= cut);
  } 
  else {
    accept = (x <= cut);
  }

  return accept;
}




///////////////////////////////////////////////////////////
int classify(float eta, float eOverP, float fBrem) {

   int cat = -1;
   eta = fabs(eta);

   if ( eta<1.5 ) {
      if ((fBrem >= 0.12) && (eOverP > 0.9) && (eOverP < 1.2)) cat = 0;
      else if (((eta >  .445   && eta <  .45  ) ||
      (eta >  .79    && eta <  .81  ) ||
      (eta > 1.137   && eta < 1.157 ) ||
      (eta > 1.47285 && eta < 1.4744) )) cat = 6;
      else if (fBrem < 0.12)
         cat = 1;
      else
         cat = 2;
   } else {
      if ((fBrem >= 0.2) && (eOverP > 0.82) && (eOverP < 1.22))
         cat = 3;
      else if (eta > 1.5 && eta <  1.58)
         cat = 7;
      else if (fBrem < 0.2)
         cat = 4;
      else
         cat = 5;
   }

   return cat;
}
