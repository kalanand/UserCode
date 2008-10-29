import FWCore.ParameterSet.Config as cms

selectElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    cut = cms.string('pt>20.0 & (abs(eta)<1.4442 | (abs(eta)>1.56 & abs(eta)<2.5))')
)

selectElectronCands = cms.EDProducer("PixelMatchGsfElectronShallowCloneProducer",
    src = cms.InputTag("selectElectrons")
)

ZToEE = cms.EDFilter("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToEE'),
    roles = cms.vstring('e1', 
        'e2'),
    decay = cms.string('selectElectrons@+ selectElectrons@-')
)

bestZToEE = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToEE")
)

ZeePath = cms.Sequence(selectElectrons+selectElectronCands+ZToEE+bestZToEE)

