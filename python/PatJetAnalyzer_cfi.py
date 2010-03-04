import FWCore.ParameterSet.Config as cms

analyzePatJets = cms.EDAnalyzer("PatJetAnalyzer",
  ## input for pat jets 
  jets = cms.InputTag("cleanPatJets"),                              
  ## correction level for pat jet in
  ## the format corrType:flavorType
  corrLevel = cms.string("abs")
)                               

comparePatJets = cms.EDAnalyzer("PatJetAnalyzer",
  ## input for pat jets 
  jets = cms.InputTag("cleanPatJets"),                              
  ## input for alternative jets collection
  comp = cms.InputTag("ak5CaloJets"),
  ## correction level for pat jet in
  ## the format corrType:flavorType
  corrLevel = cms.string("abs")
)                               
