{
  //---------- Run this macro as: root -b -q run.C ------------//

  gROOT->ProcessLine(".L spDriver.C+");
  gROOT->ProcessLine("spDriver(\"compareAlgos.cfg\",\"comparevars.drv\", true)");

 gROOT->ProcessLine(".L spDriver.C+");
 gROOT->ProcessLine("spDriver(\"comparePUs.cfg\",\"comparepus.drv\", true)");



//----------------------- signal only, no pileup -------------
//gROOT->ProcessLine(".L spDriver.C+");
//gROOT->ProcessLine("spDriver(\"compareAlgos_sig.cfg\",\"comparevars_sig.drv\", true)");

}
