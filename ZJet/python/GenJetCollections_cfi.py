import FWCore.ParameterSet.Config as cms

ic5GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("iterativeCone5GenJets"),
    module_label = cms.string('ic5GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

ic7GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("iterativeCone7GenJets"),
    module_label = cms.string('ic7GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

sc5GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("sisCone5GenJets"),
    module_label = cms.string('sc5GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

sc7GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("sisCone7GenJets"),
    module_label = cms.string('sc7GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp5GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("midPointCone5GenJets"),
    module_label = cms.string('mp5GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp7GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("midPointCone7GenJets"),
    module_label = cms.string('mp7GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

kt4GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("fastjet4GenJets"),
    module_label = cms.string('kt4GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

kt6GenJetsClean = cms.EDFilter("GenJetCleaner",
    srcJets = cms.InputTag("fastjet6GenJets"),
    module_label = cms.string('kt6GenJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

GenJetPath = cms.Sequence(ic5GenJetsClean)

