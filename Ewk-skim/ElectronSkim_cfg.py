import FWCore.ParameterSet.Config as cms
process = cms.Process("SKIM")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('ElectronIsolations_cfi')


#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
     '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3_preproduction_312-v1/0008/B4E5948E-1679-DE11-8721-002219205095.root',
       '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3_preproduction_312-v1/0006/B28F900A-0B79-DE11-B2D3-00D0680BF9A4.root',
       '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3_preproduction_312-v1/0006/26F4C28E-EA78-DE11-8CA3-0019B9E7CCC4.root',
       '/store/mc/Summer09/Zee/GEN-SIM-RECO/MC_31X_V3_preproduction_312-v1/0005/FE4ADA86-FC78-DE11-832A-001D09675288.root')
)

#############   Path       ###########################
##-------- Electrons Triggers --------------
process.HLTElectrons = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag  = cms.InputTag("TriggerResults","","HLT"),
    #HLTPaths           = cms.vstring("HLT_Ele15_SC10_LW_L1R"),
    HLTPaths           = cms.vstring("HLT_Ele15_SW_LooseTrackIso_L1R"),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(True)
)
process.skimPath = cms.Path(process.HLTElectrons)

#############   output module ########################
process.out = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('skimPath')), 
    fileName = cms.untracked.string('SkimElectrons.root')
)

process.p = cms.EndPath(process.out*process.eleIsolationSequence)


#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

