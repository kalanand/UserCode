
## Example Tag-Probe analysis for muon tracking efficiency output

import FWCore.ParameterSet.Config as cms

process = cms.Process("Fit")

process.load('FWCore/MessageService/MessageLogger_cfi')

# Add your own files here
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("Track_eff_tpedm_ntuple_example.root")
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(${numEvents}) )    

process.RunFit = cms.EDAnalyzer("TagProbeEDMAnalysis",  

      ## Efficiency/Fitting variables
      CalculateEffSideBand = cms.untracked.bool( True ), ## Calculate and store effs using SB
      CalculateEffFitter   = cms.untracked.bool( True ), ## Calculate and store effs from Roofit
      CalculateEffTruth    = cms.untracked.bool( True ), ## Calculate and store true effs

      ## Set mode to read from files ...
      Mode = cms.untracked.string("Normal"),

      ## Root file to write eff histograms to
      FitFileName = cms.untracked.string( "fit_output.root" ),

      ## Variable Specifications for SB subtractions and Roofit

      ## Choose binned or unbinned fitter ...
      ## Note that the unbinned fit will not fit weighted data,
      ## if you wish to use weights, use the binned fit.
      UnbinnedFit          = cms.untracked.bool( False ),

      ## Request a 2D fit as well if desired ... will add 2D eff hists
      ## binned in Pt and Var2
      Do2DFit              = cms.untracked.bool( True ),

      ## Mass window for fitting
      NumBinsMass         = cms.untracked.int32( 60 ),
      MassLow             = cms.untracked.double( 70.0 ),
      MassHigh            = cms.untracked.double( 110.0 ),

      ## Variables and binning for the eff hists
      ## Valid variables names for the eff binning are:
      ## "pt","p","px","py","pz","e","et","eta" and "phi"
      ## If omitted the defaults are var1 = pt and var2 = eta
      NameVar1             = cms.untracked.string( "pt" ),
      ## This way of declaring the bin will overide any other
      Var1BinBoundaries   = cms.untracked.vdouble( 10.0,30.0,40.0,50.0,100.0 ),

      NameVar2             = cms.untracked.string( "eta" ),
      Var2BinBoundaries   = cms.untracked.vdouble( -2.4,-1.2,0.0,1.2,2.4 ),

      ## Fitter variables - for the Roofit fitter
      ## If you want the variable to float in the fit fill
      ## three array elements {default, range_low, range_high}
      ## If the variable should be fixed, fill one element {value}
      ## Signal variables
      ZLineShape = cms.untracked.PSet(
	ZMean        = cms.untracked.vdouble( 91.1876, 91.0, 91.4 ),
	ZWidth       = cms.untracked.vdouble( 2.495,0.5,50.0 ),
	ZSigma       = cms.untracked.vdouble( 0.75,0.1,5.0 ),
	ZWidthL      = cms.untracked.vdouble( 15.0,0.0,30.0 ),
	ZWidthR      = cms.untracked.vdouble( 4.0,0.05,15.0 ),
	ZBifurGaussFrac    = cms.untracked.vdouble( 0.10,0.0,1.0 )
      ),

      ## Other possible signal line shapes, uncomment to use                                 
##      CBLineShape = cms.untracked.PSet(
##	CBMean        = cms.untracked.vdouble( 9.4603,9.0,10.0 ),
##	CBSigma       = cms.untracked.vdouble( 0.75,0.01,5.0 ),
##	CBAlpha       = cms.untracked.vdouble( 8.0,0.0,30.0 ),
##	CBN           = cms.untracked.vdouble( 1.0,0.0,30.0 )
##      ),

##      GaussLineShape = cms.untracked.PSet(
##	GaussMean        = cms.untracked.vdouble( 9.4603,9.0,10.0 ),
##	GaussSigma       = cms.untracked.vdouble( 0.75,0.01,5.0 )
##      ),

      ## Background variables
      CMSBkgLineShape = cms.untracked.PSet(
	CMSBkgAlpha           = cms.untracked.vdouble( 124, 0, 1000 ),
	CMSBkgBeta            = cms.untracked.vdouble( -0.028,-1.0,1.0 ),
	CMSBkgPeak            = cms.untracked.vdouble( 91.1876 ),
	CMSBkgGamma           = cms.untracked.vdouble( 0.0379,0.0,0.5 )
      ),

      ## Efficiency variables
      Efficiency        = cms.untracked.vdouble( 0.99,0.5,1.0 ),    
      NumSignal         = cms.untracked.vdouble( 4000.0,-10.0,30000.0 ),    
      NumBkgPass        = cms.untracked.vdouble( 4000.0,0.0,10000.0 ),    
      NumBkgFail        = cms.untracked.vdouble( 1000.0,-10.0,7000.0 ),    

      ## Variables for sideband subtraction
      SBSPeak            = cms.untracked.double( 9.4603 ),   ## Mass peak
      SBSStanDev         = cms.untracked.double( 2 )        ## SD from peak for subtraction

)

process.p = cms.Path( process.RunFit ) 
