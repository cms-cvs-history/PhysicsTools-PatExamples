import FWCore.ParameterSet.Config as cms

## setup the analyzer module
from PhysicsTools.PatExamples.PatJetAnalyzer_cfi import *

## setup the jet energy corrections for the reco jet
from JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_cff import *
ak5CaloJetsL2   = L2L3CorJetAK5Calo.clone(correctors = ["L2RelativeJetCorrectorAK5Calo"])
ak5CaloJetsL2L3 = L2L3CorJetAK5Calo.clone(correctors = ["L2L3JetCorrectorAK5Calo"])

###
## setup the configuration for Exercise 1 (c)
###

compareRaw = comparePatJets.clone(corrLevel="raw", comp="ak5CaloJets")
compareL2  = comparePatJets.clone(corrLevel="rel", comp="ak5CaloJetsL2")
compareL3  = comparePatJets.clone(corrLevel="abs", comp="ak5CaloJetsL2L3")


## sequence for Exercise 1(c)
comparePatAndReco = cms.Sequence(
    ak5CaloJetsL2   *
    ak5CaloJetsL2L3 *
    compareRaw +
    compareL2  +
    compareL3
)
   
###
## setup the configuration for Exercise 1 (d)
###

calibRaw = analyzePatJets.clone(corrLevel="raw")
calibL2  = analyzePatJets.clone(corrLevel="rel")
calibL3  = analyzePatJets.clone(corrLevel="abs")
calibL5  = analyzePatJets.clone(corrLevel="had:uds")
calibL7  = analyzePatJets.clone(corrLevel="part:uds")

## sequence for Exercise 1(d)
doJetResponse = cms.Sequence(
    ak5CaloJetsL2   *
    ak5CaloJetsL2L3 *
    calibRaw +
    calibL2  +
    calibL3  +
    calibL5  +
    calibL7
)

###
## setup the configuration for Exercise 3
###

compareJPT    = comparePatJets.clone(comp="cleanPatJetsAK5JPT")
comparePflow  = comparePatJets.clone(comp="cleanPatJetsAK5PF" )

## sequence for Exercise 3
compareCaloAndOthers = cms.Sequence(
    compareJPT  +
    comparePflow
)
