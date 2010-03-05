import FWCore.ParameterSet.Config as cms

analyzePatIsolation = cms.EDAnalyzer("PatIsolationAnalyzer",
  ## input for pat Muons
  muons = cms.InputTag("cleanPatMuons"),
)                               
