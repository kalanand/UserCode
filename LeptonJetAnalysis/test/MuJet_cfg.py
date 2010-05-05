import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')



#   seed and muons selection ##########

process.seed = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string('isTrackerMuon && pt > 10 && abs(eta)<2.4')
)


process.mymuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),                               
    cut = cms.string('isTrackerMuon && pt > 1 && abs(eta)<2.4')
)


#
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0004/544F6FC2-A52B-DF11-8C2F-0018F3D09706.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0004/40E547FE-2C2C-DF11-84B4-0026189438FE.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/6A3D9E72-A42B-DF11-94EA-00304867BFB2.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/1E608FCE-A32B-DF11-BC7B-001731A2876F.root',
'/store/relval/CMSSW_3_5_4/RelValWM/GEN-SIM-RECO/START3X_V24-v1/0003/0674C1E0-A42B-DF11-A01C-001A92811716.root') )


process.MuJet = cms.EDFilter("MuJet",      
    SeedMuon   = cms.InputTag("seed"),
    TrackMuons = cms.InputTag("mymuons"),
    OutputFile = cms.string('demo.root')                        
)



process.p = cms.Path( process.seed + process.mymuons + process.MuJet
                      )






