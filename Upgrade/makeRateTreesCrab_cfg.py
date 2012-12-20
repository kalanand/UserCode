import FWCore.ParameterSet.Config as cms


process = cms.Process("L1UCTRates")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_H_V28::All'
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring('dcache:/pnfs/cms/WAX/11/store/data/Run2012C/SingleMu/RAW-RECO/MuTau-24Aug2012-v1/00000/102DDE54-B7EF-E111-8FBB-00259073E406.root')
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("uct_rate_tree.root")
)

# Load emulation and RECO sequences
process.load("L1Trigger.UCT2015.emulation_cfi")


# Define the tree producers
process.tauL1Rate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("l1extraParticles", "Tau")),
    isUCT = cms.bool(False),
)
process.rlxTauUCTRate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("UCT2015Producer", "RelaxedTauUnpacked")),
    isUCT = cms.bool(True),
)
process.isoTauUCTRate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("UCT2015Producer", "IsolatedTauUnpacked")),
    isUCT = cms.bool(True),

)
process.isoEGL1Rate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("l1extraParticles", "Isolated")),
    isUCT = cms.bool(False),
)
process.isoEGUCTRate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("UCT2015Producer", "IsolatedEGUnpacked")),
    isUCT = cms.bool(True),
)
process.rlxEGL1Rate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(
        cms.InputTag("l1extraParticles", "NonIsolated"),
        cms.InputTag("l1extraParticles", "Isolated")
    ),
    isUCT = cms.bool(False),
)
process.rlxEGUCTRate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("UCT2015Producer", "RelaxedEGUnpacked")),
    isUCT = cms.bool(True),
)
process.jetL1Rate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(
        # Combine central and forward
        cms.InputTag("l1extraParticles", "Central"),
        cms.InputTag("l1extraParticles", "Forward"),
    ),
    isUCT = cms.bool(False),
)
process.jetUCTRate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("UCT2015Producer", "JetUnpacked")),
    isUCT = cms.bool(True),
)

process.sumsL1Rates = cms.EDAnalyzer(
    "SumsRateTree",
    l1MHTSrc = cms.InputTag("l1extraParticles", "MHT"),
    l1METSrc = cms.InputTag("l1extraParticles", "MET"),
    # fixme
    l1SHTSrc = cms.InputTag("l1extraParticles", "MHT"),
    l1SETSrc = cms.InputTag("l1extraParticles", "MET"),
)

process.sumsUCTRates = cms.EDAnalyzer(
    "SumsRateTree",
    l1MHTSrc = cms.InputTag("UCT2015Producer", "MHTUnpacked"),
    l1METSrc = cms.InputTag("UCT2015Producer", "METUnpacked"),
    # fixme
    l1SHTSrc = cms.InputTag("UCT2015Producer", "MHTUnpacked"),
    l1SETSrc = cms.InputTag("UCT2015Producer", "METUnpacked"),
)



# Define the muon tree producer
process.muonL1Rate = cms.EDAnalyzer(
    "RateTree",
    src = cms.VInputTag(cms.InputTag("l1extraParticles")),
    isUCT = cms.bool(False),
)


process.p1 = cms.Path(
    process.emulationSequence *
    process.tauL1Rate *
    process.rlxTauUCTRate *
    process.isoTauUCTRate *
    process.isoEGL1Rate *
    process.isoEGUCTRate *
    process.rlxEGL1Rate *
    process.rlxEGUCTRate *
    process.jetL1Rate *
    process.jetUCTRate *
    #process.sumsL1Rates *
    #process.sumsUCTRates *
    process.muonL1Rate
)

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

