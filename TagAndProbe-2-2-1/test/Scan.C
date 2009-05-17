{
TFile f("/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_1.root");
TTree* t = (TTree*) f.Get("Events");
t->SetScanField(0);
t->Scan("floats_TPEdm_TPTagetDet_ElectronEff.obj[10]:floats_TPEdm_TPTagetaDet_ElectronEff.obj[10]:floats_TPEdm_TPProbeetDet_ElectronEff.obj[10]:floats_TPEdm_TPProbeetaDet_ElectronEff.obj[10]", "floats_TPEdm_TPProbeetDet_ElectronEff.obj[10]>20 && ( abs(floats_TPEdm_TPProbeetaDet_ElectronEff.obj[10])<1.4442 || (abs(floats_TPEdm_TPProbeetaDet_ElectronEff.obj[10])>1.560 && abs(floats_TPEdm_TPProbeetaDet_ElectronEff.obj[10])<2.5))","",1200);
}
