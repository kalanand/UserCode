import FWCore.ParameterSet.Config as cms

from JetMETCorrections.ZJet.CaloJetCollections_cfi import *
caloJetCollectionClone = cms.EDProducer("CaloJetShallowCloneProducer",
    src = cms.InputTag("ic5CaloJetsClean")
)

caloJetSele = cms.EDFilter("PtMinCandSelector",
    src = cms.InputTag("caloJetCollectionClone"),
    ptMin = cms.double(10.0)
)

tagJet = cms.EDFilter("CandJetFlavourIdentifier",
    jets = cms.InputTag("caloJetSele"),
    debug = cms.bool(False),
    coneSizeToAssociate = cms.double(0.3),
    vetoFlavour = cms.vstring(),
    physicsDefinition = cms.bool(True)
)

TagJetPath = cms.Sequence(caloJetCollectionClone*caloJetSele*tagJet)

