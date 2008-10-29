import FWCore.ParameterSet.Config as cms

process = cms.Process("demo")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V9::All')

process.load("JetMETCorrections.MCJet.L3AbsoluteCorrection_CSA07_perfect_Zee_cff")

#   Z-->ee Collection ##########
process.load("JetMETCorrections.ZJet.ZeeCollections_cfi")

#   Jet Collections ############
process.load("JetMETCorrections.ZJet.CaloJetCollections_cfi")

process.load("JetMETCorrections.ZJet.GenJetCollections_cfi")

process.load("JetMETCorrections.ZJet.CorrectedCaloJetCollections_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/uscms_data/d1/kalanand/junk/Pythia_ZeeJets/CMSSW_2_1_9/src/Configuration/GenProduction/python/Vol-1/PYTHIA6_ZeeJetpt_80_120_10TeV_cff_py_py_RAW2DIGI_RECO.root')
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100



process.myPartons = cms.EDFilter("PartonSelector",
    withLeptons = cms.bool(False)
)

process.IC5byRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("iterativeCone5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)

process.IC5byValPhys = cms.EDFilter("JetFlavourIdentifier",
    srcByReference = cms.InputTag("IC5byRef"),
    physicsDefinition = cms.bool(True)
)

process.myzjet = cms.EDFilter("ZJetAnalysis",
    #untracked VInputTag srcGen = {ic5GenJetsClean}
    srcCaloCor = cms.untracked.VInputTag(cms.InputTag("ic5CaloJetsCor")),
    srcCalo = cms.untracked.VInputTag(cms.InputTag("ic5CaloJetsClean")),
    electrons = cms.InputTag("selectElectrons"),
    tracks = cms.untracked.InputTag("generalTracks"),
    zee = cms.InputTag("ZToEE")
)

process.prefer("L3JetCorrectorIcone5")
process.TagJetPath = cms.Sequence(process.myPartons+process.IC5byRef+process.IC5byValPhys)
process.p = cms.Path(process.ZeePath*process.CaloJetPath*process.CorJetPath*process.myzjet)

