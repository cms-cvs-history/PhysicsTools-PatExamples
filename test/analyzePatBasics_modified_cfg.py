import FWCore.ParameterSet.Config as cms

## define process with name 'Test'
process = cms.Process("Test")

## define input source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:PATLayer1_Output.fromAOD_full.root'
  )
)

## define MessageLoggerService
process.MessageLogger = cms.Service("MessageLogger")

## define TFileService for histogramming
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('analyzePatBasics.root')
                                   )

## using import and making the module known to the process afterwards
from PhysicsTools.PatExamples.PatBasicAnalyzer_cfi import analyzeBasicPat

# ----------------------------------------------------
# EXERCISE 1/2: (of Module 3)analyze patTuple which 
#             contains both cleanLayer1Jets and sel-
#             ectedLayer1Candidates or myCleanLayer1
#             Jets
# ----------------------------------------------------
## create a module to analyze the selectedLayer1Jets w/o discarded jets
#process.analyzeModifiedJets = analyzeBasicPat.clone(jetSrc='selectedLayer1Jets')
## create a module to analyze the myCleanLayer1Jets w/o discarded jets
process.analyzeModifiedJets = analyzeBasicPat.clone(jetSrc='myCleanLayer1Jets')

## create a module to analyze the cleanLayer1Jets with discarded jets
process.analyzeStandardJets = analyzeBasicPat.clone(jetSrc='cleanLayer1Jets')


## process path
process.p = cms.Path(process.analyzeStandardJets * process.analyzeModifiedJets)

