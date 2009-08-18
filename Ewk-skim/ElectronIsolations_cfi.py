import FWCore.ParameterSet.Config as cms
from Geometry.CMSCommonData.cmsIdealGeometryXML_cff import *
from Geometry.CaloEventSetup.CaloGeometry_cff import *
##### Egamma isolations: not stored by default in RECO in 3.2.X
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositTk_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositEcalFromHits_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositHcalFromTowers_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositHcalFromHits_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoFromDepsModules_cff import *


eleIsoDeposits = cms.Sequence(
    eleIsoDepositTk +
    eleIsoDepositEcalFromHits + 
    eleIsoDepositHcalFromTowers +
    eleIsoDepositHcalDepth1FromTowers +
    eleIsoDepositHcalDepth2FromTowers +
    eleIsoDepositHcalFromHits
)

eleIsoFromDeposits = cms.Sequence( 
    eleIsoFromDepsTk * 
    eleIsoFromDepsEcalFromHits * 
    eleIsoFromDepsHcalFromTowers *
    eleIsoFromDepsHcalDepth1FromTowers *
    eleIsoFromDepsHcalDepth2FromTowers *
    eleIsoFromDepsHcalFromHits 
)

#Standard electron isolation sequence
eleIsolationSequence = cms.Sequence( 
    eleIsoDeposits * 
    eleIsoFromDeposits
)
