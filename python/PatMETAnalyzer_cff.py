import FWCore.ParameterSet.Config as cms

## setup the analyzer module
from PhysicsTools.PatExamples.PatMETAnalyzer_cfi import *

###
## setup the configuration for Exercise 3
###

comparePF = analyzePatMETs.clone(comp="patMETsPF")
compareTC = analyzePatMETs.clone(comp="patMETsTC")

## sequence for Exercise 3
doPatMETsComparison = cms.Sequence(
    comparePF +
    compareTC
)
