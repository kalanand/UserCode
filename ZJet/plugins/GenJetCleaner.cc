////////////////////////////////////////////////////////////////////////////////
//
// GenJetCleaner
// --------------
//
// 06/07/2008 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
// July 21, 2008  Kalanand Mishra, <kalanand@fnal.gov> 
//       Modified include modules to compile in CMSSW_1_6_11. 
////////////////////////////////////////////////////////////////////////////////


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
 
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"

#include <memory>
#include <vector>
#include <sstream>




////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////

class GenJetCleaner : public edm::EDProducer
{
public:
  // construction/destruction
  GenJetCleaner(const edm::ParameterSet& iConfig);
  virtual ~GenJetCleaner();
  
  // member functions
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
  void endJob();

private:  
  // member data
  edm::InputTag              srcJets_;
  std::vector<edm::InputTag> srcObjects_;
  double                     deltaRMin_;
  
  std::string  moduleLabel_;
  unsigned int nJetsTot_;
  unsigned int nJetsClean_;
};


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
GenJetCleaner::GenJetCleaner(const edm::ParameterSet& iConfig)
  : srcJets_    (iConfig.getParameter<edm::InputTag>         ("srcJets"))
  , srcObjects_ (iConfig.getParameter<vector<edm::InputTag> >("srcObjects"))
  , deltaRMin_  (iConfig.getParameter<double>                ("deltaRMin"))
  , moduleLabel_(iConfig.getParameter<string>                ("@module_label"))
  , nJetsTot_(0)
  , nJetsClean_(0)
{
  produces<reco::GenJetCollection>();
}


//______________________________________________________________________________
GenJetCleaner::~GenJetCleaner()
{
  
}



////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void GenJetCleaner::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{
  auto_ptr<reco::GenJetCollection>
    cleanJets(new reco::GenJetCollection);

  edm::Handle<reco::GenJetCollection> jets;
  iEvent.getByLabel(srcJets_,jets);
  
  bool* isClean = new bool[jets->size()];
  for (unsigned int iJet=0;iJet<jets->size();iJet++) isClean[iJet] = true;
  
  for (unsigned int iSrc=0;iSrc<srcObjects_.size();iSrc++) {
    edm::Handle<reco::CandidateView> objects;
    iEvent.getByLabel(srcObjects_[iSrc],objects);
    
    for (unsigned int iJet=0;iJet<jets->size();iJet++) {
      const reco::GenJet& jet = jets->at(iJet);
      for (unsigned int iObj=0;iObj<objects->size();iObj++) {
	const reco::Candidate& obj = objects->at(iObj);
	double deltaR = reco::deltaR(jet,obj);
	if (deltaR<deltaRMin_)  isClean[iJet] = false;
      }
    }
  }
  
  for (unsigned int iJet=0;iJet<jets->size();iJet++)
    if (isClean[iJet]) cleanJets->push_back(jets->at(iJet));
  
  nJetsTot_  +=jets->size();
  nJetsClean_+=cleanJets->size();

  delete [] isClean;  
  iEvent.put(cleanJets);
}


//______________________________________________________________________________
void GenJetCleaner::endJob()
{
  stringstream ss;
  ss<<"nJetsTot="<<nJetsTot_<<" nJetsClean="<<nJetsClean_
    <<" fJetsClean="<<100.*(nJetsClean_/(double)nJetsTot_)<<"%\n";
  cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"
      <<"\n"<<moduleLabel_<<"(GenJetCleaner) SUMMARY:\n"<<ss.str()
      <<"++++++++++++++++++++++++++++++++++++++++++++++++++"
      <<endl;
}


////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(GenJetCleaner);
