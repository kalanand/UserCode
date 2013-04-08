{
 gSystem->Load("libFWCoreFWLite.so");
 AutoLibraryLoader::enable();
 gSystem->Load("libDataFormatsFWLite.so");
 gROOT->SetStyle ("Plain");
 gSystem->Load("libRooFit") ;
 gROOT.GetInterpreter().AddIncludePath("$ROOFITSYS/include");
 using namespace RooFit ;
 cout << "loaded" << endl;
}

