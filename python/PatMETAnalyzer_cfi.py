import FWCore.ParameterSet.Config as cms

analyzePatMETs = cms.EDAnalyzer("PatMETAnalyzer",
  ## input for pat MET
  mets = cms.InputTag("patMETs"),
  ## input for alternative MET collection
  comp = cms.InputTag("patMETsPF")                        
)                               
