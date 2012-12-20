import FWCore.ParameterSet.Config as cms

process = cms.Process("L1UCTEfficiency")

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
    fileNames = cms.untracked.vstring( 'dcache:/pnfs/cms/WAX/11/store/data/Run2012C/SingleMu/RAW-RECO/MuTau-24Aug2012-v1/00000/102DDE54-B7EF-E111-8FBB-00259073E406.root')
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("uct_efficiency_tree.root")
)

# Load emulation and RECO sequences
process.load("L1Trigger.UCT2015.emulation_cfi")
process.load("L1Trigger.UCT2015.recoObjects_cfi")

# Common branches to add to the ntuple
common_ntuple_branches = cms.PSet(
    index = cms.string("index"), # Index of reco object in the event
    nRecoObjects = cms.string("nTotalObjects"), # Number of reco objects in the event
    nPVs = cms.string("nPVs"), # number of reco'ed vertices in the event

    # Run, lumi, event number
    run = cms.string("id.run"),
    lumi = cms.string("id.luminosityBlock"),
    evt = cms.string("id.event"),

    recoPt = cms.string("reco.pt"),
    recoEta = cms.string("reco.eta"),
    recoPhi = cms.string("reco.phi"),

    # Whether there exists a L1/UCT object matched to reco
    l1Match = cms.string("l1Match"),
    l1gMatch = cms.string("l1gMatch"),

    l1Pt = cms.string("? l1Match ? l1.pt : 0"),
    l1Eta = cms.string("? l1Match ? l1.eta : 0"),
    l1Phi = cms.string("? l1Match ? l1.phi : 0"),


    # TODO add L1extra eta/phi indices

    l1DPhi = cms.string("? l1Match ? deltaPhi(l1.phi, reco.phi) : -1"),
    l1DR = cms.string("? l1Match ? deltaR(l1.eta, l1.phi, reco.eta, reco.phi) : -1"),

    l1gPt = cms.string("? l1gMatch ? l1g.pt : 0"),
    l1gEta = cms.string("? l1gMatch ? l1g.eta : 0"),
    l1gPhi = cms.string("? l1gMatch ? l1g.phi : 0"),

    # For tuning isolation and PU subtraction
    l1gPU = cms.string("? l1gMatch ? l1g.puLevel : -2"),
    l1gRegionEt = cms.string("? l1gMatch ? l1g.associatedRegionEt : -2"),
    l1gJetPt = cms.string("? l1gMatch ? l1g.associatedJetPt : -2"),
    l1gEllIso = cms.string("? l1gMatch ? l1g.ellIsolation : -2"),
    l1gTauVeto = cms.string("? l1gMatch ? l1g.tauVeto : -2"),
    l1gMIP = cms.string("? l1gMatch ? l1g.mipBit : -2"),

    l1gEtaCode = cms.vstring("? l1gMatch ? l1g.etaIndex : 0", "I"),
    l1gPhiCode = cms.vstring("? l1gMatch ? l1g.phiIndex : 0", "I"),

    l1gDPhi = cms.string("? l1gMatch ? deltaPhi(l1g.phi, reco.phi) : -1"),
    l1gDR = cms.string("? l1gMatch ? deltaR(l1g.eta, l1g.phi, reco.eta, reco.phi) : -1"),
)

# Keep track of electron isolation values
electron_branches = cms.PSet(
    dr03TkSumPt  = cms.string("reco.dr03TkSumPt"),
    dr03EcalRecHitSumEt  = cms.string("reco.dr03EcalRecHitSumEt"),
    dr03HcalTowerSumEt  = cms.string("reco.dr03HcalTowerSumEt"),
    dr03CombinedEt  = cms.string("reco.dr03TkSumPt + reco.dr03EcalRecHitSumEt + reco.dr03HcalTowerSumEt"),
)

# Keep track of information about the ECAL/HCAL composition of taus
tau_branches = cms.PSet(
    emFraction = cms.string("reco.emFraction"),
    decayMode = cms.string("reco.decayMode"),
    # EK - as far as I can tell, this does not use the lead track at all
    hcal = cms.string("reco.hcalTotOverPLead"),
)

# Define the tree producers
process.isoTauEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoTaus"),
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticles", "Tau")),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "IsolatedTauUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        tau_branches,
    )
)
# Define the tree producers
process.rlxTauEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoTaus"),
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticles", "Tau")),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "RelaxedTauUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        tau_branches,
    )
)

# Note that the input electron collection is not isolated, this needs to be done
# at the ntuple level.
process.isoEGEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoElecs"),
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticles", "Isolated")),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "IsolatedEGUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        electron_branches,
    )
)

process.rlxEGEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoElecs"),
    l1Src = cms.VInputTag(
        # These two collections
        cms.InputTag("l1extraParticles", "NonIsolated"),
        cms.InputTag("l1extraParticles", "Isolated"),
    ),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "RelaxedEGUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        electron_branches,
    )
)

# So we can compare relaxed UCT + ntuple isolation cuts versus stock L1 IsoEG
process.rlxUCTisoL1EGEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoElecs"),
    l1Src = cms.VInputTag(
        cms.InputTag("l1extraParticles", "Isolated"),
    ),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "RelaxedEGUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        electron_branches,
    )
)

process.jetEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoJets"),
    l1Src = cms.VInputTag(
        # Combine central jets + tau + forward jets
        cms.InputTag("l1extraParticles", "Central"),
        cms.InputTag("l1extraParticles", "Tau"),
        cms.InputTag("l1extraParticles", "Forward"),
    ),
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "JetUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
    )
)






# Keep track of muon isolation values
muon_branches = cms.PSet(
    dr03TkSumPt  = cms.string("reco.isolationR03().sumPt"),
    dr03EcalRecHitSumEt  = cms.string("reco.isolationR03().emEt"),
    dr03HcalTowerSumEt  = cms.string("reco.isolationR03().hadEt"),
    dr03CombinedEt  = cms.string("reco.isolationR03().sumPt + reco.isolationR03().emEt + reco.isolationR03().hadEt"),
    dr04PFIsoChargedHad  = cms.string("reco.pfIsolationR04().sumChargedHadronPt"),
    dr04PFIsoNeutralHad  = cms.string("reco.pfIsolationR04().sumNeutralHadronEt"),
    dr04PFIsoPhot  = cms.string("reco.pfIsolationR04().sumPhotonEt"),
    dr04PFIsoPUPt  = cms.string("reco.pfIsolationR04().sumPUPt"),
    dr04PFIsoCombined  = cms.string("reco.pfIsolationR04().sumChargedHadronPt+max(0.,reco.pfIsolationR04().sumNeutralHadronEt+ reco.pfIsolationR04().sumPhotonEt-0.5*reco.pfIsolationR04().sumPUPt)"),
    dr04PFIsoCombinedRel  = cms.string("(reco.pfIsolationR04().sumChargedHadronPt+max(0.,reco.pfIsolationR04().sumNeutralHadronEt+reco.pfIsolationR04().sumPhotonEt-0.5*reco.pfIsolationR04().sumPUPt))/reco.pt"),    
)


# Note that the input muon collection is not isolated, this needs to be done
# at the ntuple level.
process.recoMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("pt>20 && isGlobalMuon && isPFMuon && abs(eta)<2.4"
                     " && globalTrack().normalizedChi2<10"
                     " && globalTrack().hitPattern().numberOfValidMuonHits>0"
                     " && globalTrack().hitPattern().numberOfValidPixelHits>0"
                     " && numberOfMatchedStations>1"
                     " && globalTrack().hitPattern().trackerLayersWithMeasurement>5"
                     )
)
process.muonEfficiency = cms.EDAnalyzer(
    "EfficiencyTree",
    recoSrc = cms.VInputTag("recoMuons"),
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticles")),    
    l1GSrc = cms.VInputTag(cms.InputTag("UCT2015Producer", "JetUnpacked")),
    l1GPUSrc = cms.InputTag("UCT2015Producer", "PULevel"),
    # Max DR for RECO-trigger matching
    maxDR = cms.double(0.5),
    # Ntuple configuration
    ntuple = cms.PSet(
        common_ntuple_branches,
        muon_branches,
    )
)




process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.p1 = cms.Path(
    process.recoObjects *
    process.emulationSequence *
    #process.dump *
    process.isoTauEfficiency *
    process.rlxTauEfficiency *
    process.isoEGEfficiency *
    process.rlxEGEfficiency *
    process.rlxUCTisoL1EGEfficiency *
    process.jetEfficiency *
    process.recoMuons *
    process.muonEfficiency
)

################################################################################
###  Semileptonic ttbar skim for sums ###########################################
################################################################################

process.oneMuon = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("tightMuons"),
    minNumber = cms.uint32(1),
)

process.jetsPt30 = cms.EDFilter(
    "PFJetSelector",
    src = cms.InputTag("ak5PFJetsNOMuons"),
    filter = cms.bool(True),
    cut = cms.string("pt > 30")
)

process.atLeastThreeJets = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("jetsPt30"),
    minNumber = cms.uint32(3),
)

# Computing CaloMET significance
process.load("RecoMET.METProducers.CaloMETSignif_cfi")
process.metsignificance.noHF = True

# Computing RECO level Sum ET and Sum HT
process.load("L1Trigger.UCT2015.PFSumET_cfi")

process.l1SumsEfficiency = cms.EDAnalyzer(
    "SumsEfficiencyTree",
    l1MHTSrc = cms.InputTag("l1extraParticles", "MHT"),
    l1METSrc = cms.InputTag("l1extraParticles", "MET"),
    l1METSigSrc = cms.InputTag("UCT2015Producer", "METSIGUnpacked"),
    # fixme
    l1SHTSrc = cms.InputTag("l1extraParticles", "MHT"),
    l1SETSrc = cms.InputTag("l1extraParticles", "MET"),
    recoMHTSrc = cms.InputTag("htMetAK5"),
    recoMETSrc = cms.InputTag("metNoHF"), # calomet
    recoMETSigSrc = cms.InputTag("metsignificance"), # calomet
    recoSHTSrc = cms.InputTag("pfSumET", "sht"),
    recoSETSrc = cms.InputTag("pfSumET", "set"),
)
process.uctSumsEfficiency = cms.EDAnalyzer(
    "SumsEfficiencyTree",
    l1MHTSrc = cms.InputTag("UCT2015Producer", "MHTUnpacked"),
    l1METSrc = cms.InputTag("UCT2015Producer", "METUnpacked"),
    l1METSigSrc = cms.InputTag("UCT2015Producer", "METSIGUnpacked"),
    l1SHTSrc = cms.InputTag("UCT2015Producer", "SHTUnpacked"),
    l1SETSrc = cms.InputTag("UCT2015Producer", "SETUnpacked"),

    recoMHTSrc = cms.InputTag("htMetAK5"),
    recoMETSrc = cms.InputTag("metNoHF"), # calomet
    recoMETSigSrc = cms.InputTag("metsignificance"), # calomet
    recoSHTSrc = cms.InputTag("pfSumET", "sht"),
    recoSETSrc = cms.InputTag("pfSumET", "set"),
)

# Make a version of UCT without PU corrections.
process.UCT2015ProducerNoPU = process.UCT2015Producer.clone(
    puCorrect = False
)
process.uctSumsNoPUEfficiency = process.uctSumsEfficiency.clone(
    l1MHTSrc = cms.InputTag("UCT2015ProducerNoPU", "MHTUnpacked"),
    l1METSrc = cms.InputTag("UCT2015ProducerNoPU", "METUnpacked"),
    l1METSigSrc = cms.InputTag("UCT2015ProducerNoPU", "METSIGUnpacked"),
    l1SHTSrc = cms.InputTag("UCT2015ProducerNoPU", "SHTUnpacked"),
    l1SETSrc = cms.InputTag("UCT2015ProducerNoPU", "SETUnpacked"),
)

process.semileptonicTTBarPath = cms.Path(
    process.cleanJets *
    process.oneMuon *
    process.jetsPt30 *
    process.atLeastThreeJets *
    process.pfSumET *
    process.metsignificance *
    process.l1SumsEfficiency *
    process.uctSumsEfficiency *
    # w/o PU corrections
    process.UCT2015ProducerNoPU *
    process.uctSumsNoPUEfficiency
)

process.schedule = cms.Schedule(
    process.p1,
    process.semileptonicTTBarPath
)

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

