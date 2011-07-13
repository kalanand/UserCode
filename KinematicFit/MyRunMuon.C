{
  gSystem->Load("libPhysicsToolsKinFitter.so");
  gROOT->ProcessLine(".include ../../../");
  gROOT->ProcessLine(".L Resolution.cc++");
  gROOT->ProcessLine(".L kanamuon.C++");
  gROOT->ProcessLine("kanamuon runover");
  gROOT->ProcessLine("runover.myana(0)");
}
