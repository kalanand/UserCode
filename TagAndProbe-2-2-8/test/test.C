{
  // open the histo file and retrieve the info in the input root file     
  TFile* m_file = TFile::Open( inputFileName.c_str(), "READ");
  TList *keylist = m_file->GetListOfKeys();
  
  // Check if the built m_prefix is actually correct  
  TString info_tree_name = "";  
  TIter firstIter(keylist);
  TObject *obj;
  bool isFound = false;
  while (obj = firstIter()){
    TString keyname = obj->GetName();
    if (keyname.Contains(m_prefix.c_str())) isFound = true;
  }
  
  if (isFound == true){
    std::cout<< "Info tree " << m_prefix.c_str() << " found." <<std::endl;
  } else {
    std::cout<< "Info tree " << m_prefix.c_str() << " not found." <<std::endl;
  }
  
}






 
bool Offline::Init() {   

  TChain *t = new TChain( m_prefix.c_str() );
  t->Add( m_file->GetName() );
  
  std::vector<float> *EtaBins=0;
  std::vector<float> *EnergyBins=0;
  
  t->SetBranchAddress("NumberEtaBins",    &m_nEtaBins );
  t->SetBranchAddress("NumberEnergyBins", &m_nEnergyBins );     
  t->SetBranchAddress("EtaBins", &EtaBins );
  t->SetBranchAddress("EnergyBins", &EnergyBins );
  
  t->GetEntry(0);
  for (int iEta=0; iEta<m_nEtaBins+1; ++iEta){
    m_EtaBins.push_back( EtaBins->at(iEta) );           
  }     
  for (int iEne=0; iEne<m_nEnergyBins+1; ++iEne){
    m_EnergyBins.push_back( EnergyBins->at(iEne) );             
  }
  std::cout<<"\tNumber of eta bins : "<< m_EtaBins.size()-1 <<std::endl;
  std::cout<<"\tNumber of energy bins : "<< m_EnergyBins.size()-1 <<std::endl;
  
  delete t;
  return true;
  
}





TGraphAsymmErrors* Offline::plotEffVsEta( int bin){

  TGraphAsymmErrors *g = new TGraphAsymmErrors();

  TH2 *matched;
  TH2 *total;

  TString variableName = Form("%sNumberMatchedTrueJets", m_prefix.c_str() );
  matched = (TH2I*)m_file->Get( variableName.Data() );
  variableName = Form("%sNumberTrueJets", m_prefix.c_str() );
  total = (TH2I*)m_file->Get( variableName.Data() );

  TH1D *Nmatched = matched->ProjectionX("Nmatched", bin+1, bin+1);
  TH1D *Ntotal = total->ProjectionX("Ntotal", bin+1, bin+1);
  
  g->BayesDivide(Nmatched,Ntotal);
  g->SetTitle(m_title + axisTitle);
  g->GetYaxis()->SetRangeUser(0, 1.2);
  g->GetXaxis()->SetLimits(0, 4.5);   

 return g;
}






////// private function : compute the mean & resolution using a 
////// gaussian fit between +/- 2 sigma

int Offline::fitGauss( TH1F* hist, double *mean, double *reso ){        
  
  //return 0 if no entries are found in the histogram     
  if (hist->GetEntries()==0){
    mean[0] = 0;
    mean[1] = 0;
    reso[0] = 0;            
    reso[1] = 0;
    return -1;                                      
  }       
  TCanvas *c1 = new TCanvas();
  TF1 *g = new TF1("G", "gaus", -2, 2);            
  hist->Fit(g, "q");      
  double  mean0 = g->GetParameter(1);
  double sigma0 = g->GetParameter(2);
  
  g->SetParLimits(1, mean0-3*sigma0, mean0+3*sigma0 );   
  int status = hist->Fit(g, "q", "", mean0-2*sigma0, mean0+2*sigma0);      
  mean[0] = g->GetParameter(1);
  mean[1] = g->GetParError(1);    
  double sigma = g->GetParameter(2);            
  double dsigma = g->GetParError(2);                              
  
  reso[0] = 100*sigma/mean[0];
  reso[1] = reso[0]*sqrt( pow(mean[1]/mean[0], 2) + pow(dsigma/sigma, 2) );
  
  //mean[0] = hist->GetMean();
  //mean[1] = 0; 
  //reso[0] = hist->GetRMS()/hist->GetMean();
  //reso[1] = 0; 
  
  delete g;
  delete c1;     
  return status;            
}
