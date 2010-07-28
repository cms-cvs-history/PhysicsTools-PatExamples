import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.FWLiteParams = cms.PSet(
    inputFile   = cms.string('file:patTuple.root'),
    outputFile  = cms.string('analyzePatBasics.root'),
    muons = cms.InputTag('cleanPatMuons')
)

