import FWCore.ParameterSet.Config as cms

L3JetCorrectorIcone5 = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('CSA07_perfect_L3Absolute_iterativeCone5_Zee'),
    label = cms.string('L3AbsoluteJetCorrectorIcone5')
)

L3JetCorrectorIcone7 = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('CSA07_perfect_L3Absolute_iterativeCone7_Zee'),
    label = cms.string('L3AbsoluteJetCorrectorIcone7')
)

L3JetCorrectorMcone5 = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('CSA07_perfect_L3Absolute_midPointCone5_Zee'),
    label = cms.string('L3AbsoluteJetCorrectorMcone5')
)

L3JetCorrectorMcone7 = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('CSA07_perfect_L3Absolute_midPointCone7_Zee'),
    label = cms.string('L3AbsoluteJetCorrectorMcone7')
)

L3JetCorrectorFKt6 = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('CSA07_perfect_L3Absolute_fastjet6_Zee'),
    label = cms.string('L3AbsoluteJetCorrectorFKt6')
)

L3JetCorJetIcone5 = cms.EDProducer("JetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('L3AbsoluteJetCorrectorIcone5'),
    alias = cms.untracked.string('L3JetCorJetIcone5')
)

L3JetCorJetIcone7 = cms.EDProducer("JetCorrectionProducer",
    src = cms.InputTag("iterativeCone7CaloJets"),
    correctors = cms.vstring('L3AbsoluteJetCorrectorIcone7'),
    alias = cms.untracked.string('L3JetCorJetIcone7')
)

L3JetCorJetMcone5 = cms.EDProducer("JetCorrectionProducer",
    src = cms.InputTag("midPointCone5CaloJets"),
    correctors = cms.vstring('L3AbsoluteJetCorrectorMcone5'),
    alias = cms.untracked.string('L3JetCorJetMcone5')
)

L3JetCorJetMcone7 = cms.EDProducer("JetCorrectionProducer",
    src = cms.InputTag("midPointCone7CaloJets"),
    correctors = cms.vstring('L3AbsoluteJetCorrectorMcone7'),
    alias = cms.untracked.string('L3JetCorJetMcone7')
)

L3JetCorJetFKt6 = cms.EDProducer("JetCorrectionProducer",
    src = cms.InputTag("fastjet6CaloJets"),
    correctors = cms.vstring('L3AbsoluteJetCorrectorFKt6'),
    alias = cms.untracked.string('L3JetCorJetFKt6')
)


