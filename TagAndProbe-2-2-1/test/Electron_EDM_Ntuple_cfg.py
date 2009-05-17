import FWCore.ParameterSet.Config as cms

process = cms.Process("ElectronEff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')
process.load("PhysicsTools.TagAndProbe.tag_probe_electron_cfi")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100



process.TPEdm = cms.EDProducer("TagProbeEDMNtuple",
    allProbeTruthMatchMapTags = cms.untracked.VInputTag(cms.InputTag("SuperClustersMatch"), cms.InputTag("GsfElectronsMatch"), cms.InputTag("IsolationMatch"), cms.InputTag("IdMatch"), cms.InputTag("HFSCMatch")),
    checkExactOverlap = cms.untracked.bool(False),
    triggerDelRMatch = cms.untracked.double(0.3),
    triggerDelPtRelMatch = cms.untracked.double(0.3),
    allProbeCandTags = cms.untracked.VInputTag(cms.InputTag("theSuperClusters"), cms.InputTag("theGsfElectrons"), cms.InputTag("theIsolation"), cms.InputTag("theId"), cms.InputTag("theHFSuperClusters")),
    # Truth Matching tags
    passProbeTruthMatchMapTags = cms.untracked.VInputTag(cms.InputTag("GsfElectronsMatch"), cms.InputTag("IsolationMatch"), cms.InputTag("IdMatch"), cms.InputTag("HLTMatch"), cms.InputTag("HFIDMatch")),
    # Tag & Probe Electron Candidate Collections
    tagCandTags = cms.untracked.VInputTag(cms.InputTag("theHLT"), cms.InputTag("theHLT"), cms.InputTag("theHLT"), cms.InputTag("theHLT"), cms.InputTag("theHLT")),
    # Tag & Probe Muon Association Map 
    tagProbeMapTags = cms.untracked.VInputTag(cms.InputTag("tpMapSuperClusters"), cms.InputTag("tpMapGsfElectrons"), cms.InputTag("tpMapIsolation"), cms.InputTag("tpMapId"), cms.InputTag("tpMapHFSuperClusters")),
    # Type of tag-probe candidates, use "Muon" or "Electron"
    # For the moment this only affects the kind of particle
    # used for storing MC truth information.
    tagProbeType = cms.untracked.string('Electron'),
    # Truth Map Tags
    tagTruthMatchMapTags = cms.untracked.VInputTag(cms.InputTag("HLTMatch"), cms.InputTag("HLTMatch"), cms.InputTag("HLTMatch"), cms.InputTag("HLTMatch"), cms.InputTag("HLTMatch")),
    # Store some generic information about the event
    # in case we want it
    mcParticles = cms.untracked.vint32(23, 11, 22),
    trackTags = cms.untracked.VInputTag(cms.InputTag("generalTracks")),
    # Pass Probe Electron Candidate Collections
    passProbeCandTags = cms.untracked.VInputTag(cms.InputTag("theGsfElectrons"), cms.InputTag("theIsolation"), cms.InputTag("theId"), cms.InputTag("theHLT"), cms.InputTag("HFElectronID")),
    verticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),
    mcParents = cms.untracked.vint32(0, 0, 0),
    BestProbeCriteria = cms.untracked.vstring("Random6","OneProbe","OneProbe","OneProbe","Random2"),
    BestProbeInvMass  = cms.untracked.vdouble(91.1876,91.1876,91.1876,91.1876,91.1876)
)

process.outpath = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_TPEdm_*_*'),
    fileName = cms.untracked.string('tp_EDM_ntuple_Summer08_IDEAL_V9.root')
)

process.p1 = cms.Path(process.lepton_cands+process.TPEdm)
process.the_end = cms.EndPath(process.outpath)
