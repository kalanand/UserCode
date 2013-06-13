#ifndef JetAnalysis_H
#define JetAnalysis_H

#include <TROOT.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include "TString.h"

#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <fstream>

#include "Event.h"
#include "TLorentzVector.h"
#include <fastjet/PseudoJet.hh>
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include<fastjet/Selector.hh>
#include<fastjet/tools/JetMedianBackgroundEstimator.hh>
#include<fastjet/tools/Subtractor.hh>
#include<fastjet/contrib/GenericSubtractor.hh>

#include<fastjet/contrib/Nsubjettiness.hh>
#include<fastjet/contrib/Njettiness.hh>
#include<fastjet/contrib/NjettinessPlugin.hh>


namespace {

void createHistogramsOneAlgo(std::string algo, 
			     std::map<TString, TH1*>& m_Hist1D, 
			     std::map<TString, TProfile*>& m_Hist2D)
{
  const int nJetMassBins(1000);
  const double JetMassMin(0.);
  const double JetMassMax(10000.);

  const int nJetPtBins(1000);
  const double JetPtMin(0.);
  const double JetPtMax(10000);

  TString name = (algo+ "_numJets").c_str();
  m_Hist1D[name] = new TH1D(name,"",20, -0.5, 19.5);
  name = (algo+ "_jetmass").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetMassBins, JetMassMin, JetMassMax);
  name = (algo+ "_jetpt").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetPtBins, JetPtMin, JetPtMax); 
  name = (algo+ "_jetarea").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 10.);
  name = (algo+ "_tau2tau1").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 1.);
  name = (algo+ "_tau3tau2").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 1.);
  name = (algo+ "_jetmass_tr").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetMassBins, JetMassMin, JetMassMax); 
  name = (algo+ "_jetpt_tr").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetPtBins, JetPtMin, JetPtMax); 
  name = (algo+ "_jetarea_tr").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 10.);
  name = (algo+ "_jetmass_ft").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetMassBins, JetMassMin, JetMassMax); 
  name = (algo+ "_jetpt_ft").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetPtBins, JetPtMin, JetPtMax); 
  name = (algo+ "_jetarea_ft").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 10.); 		 
  name = (algo+ "_jetmass_pr").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetMassBins, JetMassMin, JetMassMax);
  name = (algo+ "_jetpt_pr").c_str();
  m_Hist1D[name] = new TH1D(name,"",nJetPtBins, JetPtMin, JetPtMax); 
  name = (algo+ "_jetarea_pr").c_str();
  m_Hist1D[name] = new TH1D(name,"", 100, 0., 10.); 
  name = (algo+ "_massdrop_pr").c_str();
  m_Hist1D[name] = new TH1D(name,"", 200, 0., 2.);	


  name = (algo+ "_jetmass_jetpt").c_str();
  m_Hist2D[name] = new TProfile(name,"Profile of mass versus pt", 
				nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  name = (algo+ "_jetmass_tr_jetpt_tr").c_str();
  m_Hist2D[name] = new TProfile(name,"", 
				nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  name = (algo+ "_jetmass_ft_jetpt_ft").c_str();
  m_Hist2D[name] = new TProfile(name,"", 
				nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  name = (algo+ "_jetmass_pr_jetpt_pr").c_str();
  m_Hist2D[name] = new TProfile(name,"", 
				nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  name = (algo+ "_tau2tau1_jetpt").c_str();
  m_Hist2D[name] = new TProfile(name,"Profile of mass versus pt", 
				nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);
}




void createAllAlgos(std::map<TString, TH1*>& m_Hist1D, 
		    std::map<TString, TProfile*>& m_Hist2D, 
		    TString optStr="")

{
  const char* mychar = (const char*) (optStr+TString("ak5"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("ak6"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("ak7"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("ak8"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("ak10"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);

  mychar = (const char*) (optStr+TString("ca8"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("ca10"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);


  mychar = (const char*) (optStr+TString("kt5"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("kt6"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("kt7"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("kt8"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
  mychar = (const char*) (optStr+TString("kt10"));
  createHistogramsOneAlgo(std::string(mychar), m_Hist1D, m_Hist2D);
}



void createHistogramsAllAlgos(std::map<TString, TH1*>& m_Hist1D, 
			      std::map<TString, TProfile*>& m_Hist2D)

{
createAllAlgos(m_Hist1D, m_Hist2D);
}




void createSignalPlusPUHistogramsAllAlgos(std::map<TString, TH1*>& m_Hist1D, 
			      std::map<TString, TProfile*>& m_Hist2D)

{
  createAllAlgos(m_Hist1D, m_Hist2D, "all_");
}



void createSignalHistogramsAllAlgos(std::map<TString, TH1*>& m_Hist1D, 
			      std::map<TString, TProfile*>& m_Hist2D)

{
  createAllAlgos(m_Hist1D, m_Hist2D, "signal_");
}



void createPUHistogramsAllAlgos(std::map<TString, TH1*>& m_Hist1D, 
			      std::map<TString, TProfile*>& m_Hist2D)

{
  createAllAlgos(m_Hist1D, m_Hist2D, "pu_");
}



///////////////////////////////////////////////////////////////////////////////
void fillHist1D(std::map<TString, TH1*>& m_Hist1D, 
		TString histName,const Double_t& value, 
		const Double_t& wt=1.0) {

  std::map<TString, TH1*>::iterator hid=m_Hist1D.find(histName);
  if (hid==m_Hist1D.end()){
    std::cout << "%fillHist1D -- Could not find histogram with name: " << 
      histName << std::endl;
  }
  else{
    hid->second->Fill(value,wt);
  }
}
///////////////////////////////////////////////////////////////////////////////
  


///////////////////////////////////////////////////////////////////////////////
void fillHist2D(std::map<TString, TProfile*>& m_Hist2D, 
		TString histName,const Double_t& value1, const Double_t& value2,
		const Double_t& wt=1.0) {

  std::map<TString, TProfile*>::iterator hid=m_Hist2D.find(histName);

  if (hid==m_Hist2D.end()){
    std::cout << "%fillHist2D -- Could not find histogram with name: " << 
      histName << std::endl;
  }
  else{
    hid->second->Fill(value1,value2,wt);
  }
}
///////////////////////////////////////////////////////////////////////////////
  




float massDrop(fastjet::PseudoJet& transformedJet) {

  float massdrop = -1.0;
  
  //decompose into requested number of subjets:
  if (transformedJet.constituents().size() > 1){
    int nsubjetstokeep = 2;
    std::vector<fastjet::PseudoJet> subjets = 
      transformedJet.associated_cluster_sequence()->exclusive_subjets(transformedJet, nsubjetstokeep);    
                    
    TLorentzVector sj1( subjets.at(0).px(),subjets.at(0).py(),subjets.at(0).pz(),subjets.at(0).e());
    TLorentzVector sj2( subjets.at(1).px(),subjets.at(1).py(),subjets.at(1).pz(),subjets.at(1).e());   
    TLorentzVector fullj = sj1 + sj2;
                    
    if (subjets.at(0).m() >= subjets.at(1).m())
      massdrop = subjets.at(0).m()/transformedJet.m();                        
    else massdrop = subjets.at(1).m()/transformedJet.m();    
  }
  return massdrop;
}




void GetNsubjettiness(fastjet::PseudoJet& jet, 
		   double mJetRadius, float& tau2tau1, 
		   float& tau3tau2) {

  double beta = 1.0;// power for angular dependence, e.g. beta = 1 --> linear k-means, beta = 2 --> quadratic/classic k-means
  double R0 = mJetRadius; // Characteristic jet radius for normalization	      
  double Rcut = mJetRadius; // maximum R particles can be from axis to be included in jet	

  fastjet::contrib::Nsubjettiness nSub1KT(1, fastjet::contrib::Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau1 = nSub1KT(jet);
  fastjet::contrib::Nsubjettiness nSub2KT(2, fastjet::contrib::Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau2 = nSub2KT(jet);
  fastjet::contrib::Nsubjettiness nSub3KT(3, fastjet::contrib::Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau3 = nSub3KT(jet);
  fastjet::contrib::Nsubjettiness nSub4KT(4, fastjet::contrib::Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau4 = nSub4KT(jet);

  tau2tau1 = tau2/tau1;
  tau3tau2 = tau3/tau2;

/*   tau2tau1 = 1.; */
/*   tau3tau2 = 1.; */
}


/////////////////////////////////////////////////////////




void analyzeJetSubstructure(Event& pEvt, 
			    std::vector<fastjet::PseudoJet> pPart, 
			    fastjet::JetAlgorithm jetAlgo, 
			    double mJetRadius, 
			    bool PUsub, 
			    std::map<TString, TH1*>& m_Hist1D, 
			    std::map<TString, TProfile*>& m_Hist2D,
			    const char* optStr="") {

  TString numJets; 
  TString jetmass; 
  TString jetpt; 
  TString jetarea; 
  TString tau2tau1;
  TString tau3tau2;
  TString jetmass_tr; 
  TString jetpt_tr; 
  TString jetarea_tr;
  TString jetmass_ft; 
  TString jetpt_ft; 
  TString jetarea_ft;
  TString jetmass_pr; 
  TString jetpt_pr; 
  TString jetarea_pr; 
  TString massdrop_pr; 
  TString jetmass_jetpt;
  TString jetmass_tr_jetpt_tr;
  TString jetmass_ft_jetpt_ft; 
  TString jetmass_pr_jetpt_pr;
  TString tau2tau1_jetpt;

  const char* myAlgo;

  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.5)<0.001) 
    myAlgo = "ak5";
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.6)<0.001) 
    myAlgo = "ak6";
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.7)<0.001) 
    myAlgo = "ak7";
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.8)<0.001) 
    myAlgo = "ak8";
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-1.0)<0.001) 
    myAlgo = "ak10";


  if(jetAlgo == fastjet::cambridge_algorithm && fabs(mJetRadius-0.8)<0.001) 
    myAlgo = "ca8";
  if(jetAlgo == fastjet::cambridge_algorithm && fabs(mJetRadius-1.0)<0.001) 
    myAlgo = "ca10";


  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.5)<0.001) 
    myAlgo = "kt5";
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.6)<0.001) 
    myAlgo = "kt6";
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.7)<0.001) 
    myAlgo = "kt7";
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.8)<0.001) 
    myAlgo = "kt8";
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-1.0)<0.001) 
    myAlgo = "kt10";

  
  std::string prefix(std::string(optStr)+std::string(myAlgo));
  numJets                    = (prefix+"_numJets").c_str(); 
  jetmass                    = (prefix+"_jetmass").c_str(); 
  jetpt                      = (prefix+"_jetpt").c_str(); 
  jetarea                    = (prefix+"_jetarea").c_str(); 
  tau2tau1                   = (prefix+"_tau2tau1").c_str();
  tau3tau2                   = (prefix+"_tau3tau2").c_str();
  jetmass_tr                 = (prefix+"_jetmass_tr").c_str(); 
  jetpt_tr                   = (prefix+"_jetpt_tr").c_str(); 
  jetarea_tr                 = (prefix+"_jetarea_tr").c_str();
  jetmass_ft                 = (prefix+"_jetmass_ft").c_str(); 
  jetpt_ft                   = (prefix+"_jetpt_ft").c_str(); 
  jetarea_ft                 = (prefix+"_jetarea_ft").c_str();
  jetmass_pr                 = (prefix+"_jetmass_pr").c_str(); 
  jetpt_pr                   = (prefix+"_jetpt_pr").c_str(); 
  jetarea_pr                 = (prefix+"_jetarea_pr").c_str(); 
  massdrop_pr                = (prefix+"_massdrop_pr").c_str(); 
  jetmass_jetpt              = (prefix+"_jetmass_jetpt").c_str();
  jetmass_tr_jetpt_tr        = (prefix+"_jetmass_tr_jetpt_tr").c_str();
  jetmass_ft_jetpt_ft        = (prefix+"_jetmass_ft_jetpt_ft").c_str(); 
  jetmass_pr_jetpt_pr        = (prefix+"_jetmass_pr_jetpt_pr").c_str();
  tau2tau1_jetpt             = (prefix+"_tau2tau1_jetpt").c_str();


  // do re-clustering
  fastjet::JetDefinition jetDef(jetAlgo, mJetRadius);

  int activeAreaRepeats = 1;
  double ghostArea = 0.01;
  double ghostEtaMax = 3.0;
  double jetPtMin = 100.0;


  fastjet::GhostedAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
  fjActiveArea.set_fj2_placement(true);
  fastjet::AreaDefinition fjAreaDefinition( fastjet::active_area_explicit_ghosts, fjActiveArea );
  fastjet::ClusterSequenceArea thisClustering(pPart, jetDef, fjAreaDefinition);
    

  // ------- pileup subtraction stuff --------------
  fastjet::JetDefinition jet_def_for_rho(fastjet::kt_algorithm, 0.4);
  fastjet::Selector rho_range = fastjet::SelectorAbsRapMax(ghostEtaMax);
  // the estimation of the background itself
  // fastjet::JetMedianBackgroundEstimator bge_rho(rho_range, jet_def_for_rho, fjAreaDefinition);
  fastjet::ClusterSequenceArea puClustering(pPart, jet_def_for_rho, fjAreaDefinition);
  fastjet::JetMedianBackgroundEstimator bge_rho(rho_range, puClustering);

  // for Pythia8(4C) minbias events, we've studied the rapidity 
  // dependence so we can easily correct for it by applying
  // (Gregory Soyez has a reference for that if needed)
  fastjet::BackgroundRescalingYPolynomial rescaling(1.05114, 0, -0.0236079, 0, 2.22773e-05);
  bge_rho.set_rescaling_class(&rescaling);
  fastjet::Subtractor subtractor(&bge_rho);
  std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(jetPtMin));
  if(PUsub) out_jets = subtractor(out_jets);


  // define groomers
  fastjet::Filter trimmer( fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.03)));
  fastjet::Filter filter( fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.3), fastjet::SelectorNHardest(3)));
  fastjet::Pruner pruner(fastjet::cambridge_algorithm, 0.1, 0.5);
    
  std::vector<fastjet::Transformer const *> transformers;
  transformers.push_back(&trimmer);
  transformers.push_back(&filter);
  transformers.push_back(&pruner);
    

  // Defining Nsubjettiness parameters
  double beta = 1.0; // power for angular dependence, e.g. beta = 1 --> linear k-means, beta = 2 --> quadratic/classic k-means
  double R0 = mJetRadius; // Characteristic jet radius for normalization	      
  double Rcut = mJetRadius; // maximum R particles can be from axis to be included in jet	      
    
//numJets->Fill( (int) (out_jets.size()));
fillHist1D(m_Hist1D, numJets, (int) (out_jets.size()) );

 const int NUM_JET_MAX = 2;

 for (unsigned j = 0; j < out_jets.size() && int(j)<NUM_JET_MAX; j++) {

   double dy = 10.0;

   if( ((int) (out_jets.size()))>1) 
     dy = fabs(out_jets.at(0).rapidity() - out_jets.at(1).rapidity());
   if(dy>1.0) break;
                
   fillHist1D(m_Hist1D, jetmass, out_jets.at(j).m());
   fillHist1D(m_Hist1D, jetpt, out_jets.at(j).pt());

//     jeteta[j] = out_jets.at(j).eta();
//     jetphi[j] = out_jets.at(j).phi();
//     jete[j]   = out_jets.at(j).energy();
 
    fillHist1D(m_Hist1D, jetarea, out_jets.at(j).area());


    float m_tau2tau1 = -1.0; 
    float m_tau3tau2 = -1.0; 
    GetNsubjettiness( out_jets.at(j), mJetRadius, m_tau2tau1, m_tau3tau2);

    fillHist1D(m_Hist1D, tau2tau1, m_tau2tau1);
    fillHist1D(m_Hist1D, tau3tau2, m_tau3tau2);
    fillHist2D(m_Hist2D, jetmass_jetpt, out_jets.at(j).pt(), out_jets.at(j).m());
    fillHist2D(m_Hist2D, tau2tau1_jetpt, out_jets.at(j).pt(), m_tau2tau1);


    // pruning, trimming, filtering  -------------
    int transctr = 0;
    for ( std::vector<fastjet::Transformer const *>::const_iterator 
	    itransf = transformers.begin(), itransfEnd = transformers.end(); 
	  itransf != itransfEnd; ++itransf ) {  
            
      fastjet::PseudoJet transformedJet = out_jets.at(j);
      transformedJet = (**itransf)(transformedJet);           
      if (transctr == 0){ // trimmed
	fillHist1D(m_Hist1D, jetmass_tr, transformedJet.m());
	fillHist1D(m_Hist1D, jetpt_tr, transformedJet.pt());
	fillHist1D(m_Hist1D, jetarea_tr, transformedJet.area());
	fillHist2D(m_Hist2D, jetmass_tr_jetpt_tr, transformedJet.pt(), transformedJet.m());
      }
      else if (transctr == 1){ // filtered
	fillHist1D(m_Hist1D, jetmass_ft, transformedJet.m());
	fillHist1D(m_Hist1D, jetpt_ft, transformedJet.pt());
	fillHist1D(m_Hist1D, jetarea_ft, transformedJet.area());
	fillHist2D(m_Hist2D, jetmass_ft_jetpt_ft, transformedJet.pt(), transformedJet.m());
      }
      else if (transctr == 2){ // pruned
	fillHist1D(m_Hist1D, jetmass_pr, transformedJet.m());
	fillHist1D(m_Hist1D, jetpt_pr, transformedJet.pt());
	fillHist1D(m_Hist1D, jetarea_pr, transformedJet.area());
	fillHist1D(m_Hist1D, massdrop_pr, massDrop(transformedJet));
	fillHist2D(m_Hist2D, jetmass_pr_jetpt_pr, transformedJet.pt(), transformedJet.m());
      }                
      else{ std::cout << "error in number of transformers" << std::endl;}                    
      transctr++;
    } //  close itransf for loop   
        
  }//close jet loop

}

}

#endif
