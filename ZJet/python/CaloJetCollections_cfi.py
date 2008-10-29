import FWCore.ParameterSet.Config as cms

ic5CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("iterativeCone5CaloJets"),
    module_label = cms.string('ic5CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

ic7CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("iterativeCone7CaloJets"),
    module_label = cms.string('ic7CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

sc5CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("sisCone5CaloJets"),
    module_label = cms.string('sc5CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

sc7CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("sisCone7CaloJets"),
    module_label = cms.string('sc7CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp5CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("midPointCone5CaloJets"),
    module_label = cms.string('mp5CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp7CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("midPointCone7CaloJets"),
    module_label = cms.string('mp7CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

kt4CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("fastjet4CaloJets"),
    module_label = cms.string('kt4CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

kt6CaloJetsClean = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("fastjet6CaloJets"),
    module_label = cms.string('kt6CaloJetsClean'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

CaloJetPath = cms.Sequence(ic5CaloJetsClean)

