{

TFile f1("analysis_zjet.root");
TTree* t = (TTree*)f1.Get("ZJet");

TH1D h1("h1","",100,0.,2.5);
TH1D h2("h2","",100,0.,2.5);
h1.Sumw2();
h2.Sumw2();
h2.SetLineStyle(2);


float JetRecoPt[10],JetGenPt[10];
float Z_Pt;
bool isGoodZ;

t->SetBranchAddress("JetGenPt",  JetGenPt);
t->SetBranchAddress("JetRecoPt",  JetRecoPt);
t->SetBranchAddress("Z_Pt",  &Z_Pt);
t->SetBranchAddress("isGoodZ",  &isGoodZ);


for (Int_t entry(0); entry<t->GetEntries(); entry++) {
  t->GetEntry(entry);
  if(entry%5000==0) std::cout<<"************ Event # "<< entry <<std::endl;

  float leadGenJetPt=0.0, secondGenJetPt=0.0; 
  float leadRecoJetPt=0.0, secondRecoJetPt=0.0;
  int indexGen=-1, indexReco=-1;

  for (int i=0; i<8; i++) {
    if(JetGenPt[i]>leadGenJetPt) { leadGenJetPt = JetGenPt[i]; indexGen=i; }
    if(JetRecoPt[i]>leadRecoJetPt){ leadRecoJetPt = JetGenPt[i]; indexReco=i; }
  }



  if(Z_Pt<0.5 || Z_Pt>250.) continue;
  if(!isGoodZ) continue;
  if(leadRecoJetPt<5.) continue;

//   if(leadRecoJetPt/Z_Pt < 0.2) {
//     std::cout<< "Leading RecoJet Pt = " << leadRecoJetPt <<std::endl;
//     std::cout<< "Leading GenJet Pt = " << leadGenJetPt <<std::endl;
//     std::cout<< "Z Pt = " << Z_Pt <<std::endl;
//   }


    h1.Fill(leadGenJetPt/Z_Pt);
    h2.Fill(leadRecoJetPt/Z_Pt);
}

TCanvas *canvas = new TCanvas("canvas","allevents",880,600);
h1.Draw();
h2.Draw("same");
// delete canvas;

}
