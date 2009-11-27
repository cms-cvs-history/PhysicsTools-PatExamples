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

## using the process.load function
#process.load("PhysicsTools/PatExamples/PatBasicAnalyzer_cfi")

## using import and making the module known to the process afterwards
from PhysicsTools.PatExamples.PatBasicAnalyzer_cfi import analyzeBasicPat
process.analyzeBasicPat = analyzeBasicPat

## cloning an existing module
process.analyzeBasicPat2= analyzeBasicPat.clone()

## process path
process.p = cms.Path(process.analyzeBasicPat * process.analyzeBasicPat2)

