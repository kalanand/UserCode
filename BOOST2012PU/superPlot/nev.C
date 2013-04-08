
#include "TFile.h"
#include <stdio.h>
#include <iostream>

void nev(std::string file) {
    TFile* file0 = new TFile(file.c_str()) ;
    //    std::cout << "Events #Entries = " << Events->GetEntries() << std::endl ; 
    std::cout << "ZJet #Entries = " << ZJet->GetEntries() << std::endl ; 
}

