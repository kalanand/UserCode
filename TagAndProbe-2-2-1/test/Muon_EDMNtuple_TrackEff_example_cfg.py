

import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.load("SimGeneral/HepPDTESSource/pythiapdt_cfi")
process.load("PhysicsTools/TagAndProbe/tag_probe_muon_tracking_eff_cfi" )

process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
        #### PUT your files here!!!
      )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 100 ) )    
       
process.TPEdm = cms.EDProducer("TagProbeEDMNtuple",
    ## Type of tag-probe candidates, use "Muon" or "Electron"
    ## For the moment this only affects the kind of particle
    ## used for storing MC truth information.
    tagProbeType = cms.untracked.string( "Muon" ),

    ## Store some generic information about the event
    ## in case we want it
    mcParticles = cms.untracked.vint32( 23,13,22 ),
    mcParents   = cms.untracked.vint32( 0,0,0 ),
    
    ## Tag & Probe Muon Candidate Collections
    tagCandTags = cms.untracked.VInputTag( cms.InputTag("tagCands") ),
    passProbeCandTags = cms.untracked.VInputTag( cms.InputTag("StaTkMatched") ),
    allProbeCandTags  = cms.untracked.VInputTag( cms.InputTag("staProbeCands") ),

    ## Truth Map Tags
    tagTruthMatchMapTags       = cms.untracked.VInputTag( cms.InputTag("tagMuonMatch") ),
    passProbeTruthMatchMapTags = cms.untracked.VInputTag( cms.InputTag("passProbeMuonMatch") ),
    allProbeTruthMatchMapTags  = cms.untracked.VInputTag( cms.InputTag("allProbeMuonMatch") ),

    ## Tag & Probe Muon Association Map ... can have many, for
    ## as many kinds of probe/pass-probe combination as desired.
    ## (Then the above muon and cand arrays should be filled appropriately),
    tagProbeMapTags = cms.untracked.VInputTag( cms.InputTag("muonTagProbeMap") ),

    ## Trigger & Trigger Matching tags
    triggerEventTag = cms.untracked.InputTag( "hltTriggerSummaryAOD" ),
    hltL1Tag        = cms.untracked.InputTag( "hltSingleMuIsoLevel1Seed" ),
    hltTag          = cms.untracked.InputTag( "hltSingleMuIsoL3IsoFiltered" ),
    triggerDelRMatch     = cms.untracked.double( 0.15 ),
    triggerDelPtRelMatch = cms.untracked.double( 0.15 )
)

# Only keep events where a Tag-Probe pair was found (useful for background)
process.TPFilter = cms.EDFilter("TagProbeEDMFilter")

process.p1 = cms.Path( process.muon_cands*process.TPEdm*process.TPFilter )

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("Track_eff_tpedm_ntuple_example.root"),
    outputCommands = cms.untracked.vstring(
	  "drop *",
	  "keep *_TPEdm_*_*" 
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    )
)
    
process.the_end = cms.EndPath( process.out )

