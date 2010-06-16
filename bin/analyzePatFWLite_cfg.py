import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.FWLiteParams = cms.PSet(
  input   = cms.string('file:patTuple.root'),
  muonSrc = cms.InputTag('cleanPatMuons')
)

