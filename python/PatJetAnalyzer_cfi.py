import FWCore.ParameterSet.Config as cms

analyzePatJets = cms.EDAnalyzer("PatJetAnalyzer",
  ## input for pat jets 
  jets = cms.InputTag("cleanPatJets"),                              
  ## correction level for pat jet in
  ## the format corrType:flavorType
  corrLevel = cms.string("abs")
)                               

comparePatJets = cms.EDAnalyzer("PatJetAnalyzer",
  ## input for reco jets
  comp = cms.InputTag("ak5CaloJets"),
  ## input for pat jets 
  jets = cms.InputTag("cleanPatJets"),                              
  ## correction level for pat jet in
  ## the format corrType:flavorType
  corrLevel = cms.string("abs")
)                               
