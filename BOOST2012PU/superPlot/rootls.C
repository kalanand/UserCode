void rootls(TString file)
{
  TFile *file0 = new TFile(file.Data());
  file0->ls();
}
