{
  gSystem->Load("libPhysicsToolsKinFitter.so");
  gROOT->ProcessLine(".include ../../../");
  gROOT->ProcessLine(".L Resolution.cc++");
  gROOT->ProcessLine(".L kanaelec.C++");
  gROOT->ProcessLine("kanaelec runover");
  gROOT->ProcessLine("runover.myana(0)");
}
