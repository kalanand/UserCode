import FWCore.ParameterSet.Config as cms 
process = cms.Process("Demo") 
process.load("FWCore.MessageService.MessageLogger_cfi") 
process.load("PhysicsTools.TagAndProbe.Electron_TagProbeEDMAnalysis_cfi") 
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring('file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_1.root', 'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_2.root', 'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_3.root', 'file:/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_4.root') 
) 
process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) 
) 
process.p = cms.Path(process.demo) 
process.MessageLogger.destinations = ['cout', 'cerr'] 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000 
process.demo.TagProbeType = 1 
process.demo.FitFileName = 'root_electroneff_WenuOn_eta_et.root' 
