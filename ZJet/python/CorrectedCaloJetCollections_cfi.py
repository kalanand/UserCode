import FWCore.ParameterSet.Config as cms

ic5CaloJetsCor = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("L3JetCorJetIcone5"),
    module_label = cms.string('ic5CaloJetsCor'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

ic7CaloJetsCor = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("L3JetCorJetIcone7"),
    module_label = cms.string('ic7CaloJetsCor'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp5CaloJetsCor = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("L3JetCorJetMcone5"),
    module_label = cms.string('mp5CaloJetsCor'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

mp7CaloJetsCor = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("L3JetCorJetMcone7"),
    module_label = cms.string('mp7CaloJetsCor'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

kt6CaloJetsCor = cms.EDFilter("CaloJetCleaner",
    srcJets = cms.InputTag("L3JetCorJetFKt6"),
    module_label = cms.string('kt6CaloJetsCor'),
    srcObjects = cms.VInputTag(cms.InputTag("selectElectronCands")),
    deltaRMin = cms.double(0.3)
)

CorJetPath = cms.Sequence(cms.SequencePlaceholder("L3JetCorJetIcone5")*ic5CaloJetsCor)

