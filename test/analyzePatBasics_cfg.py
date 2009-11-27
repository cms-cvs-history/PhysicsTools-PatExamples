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

## define EDAnalyzer module
process.analyzeBasicPat = cms.EDFilter("PatBasicAnalyzer",
  photonSrc   = cms.untracked.InputTag("cleanLayer1Photons"),
  electronSrc = cms.untracked.InputTag("cleanLayer1Electrons"),
  muonSrc     = cms.untracked.InputTag("cleanLayer1Muons"),                                             
  tauSrc      = cms.untracked.InputTag("cleanLayer1Taus"),
  jetSrc      = cms.untracked.InputTag("cleanLayer1Jets"),
  metSrc      = cms.untracked.InputTag("layer1METs")
)

## process path
process.p = cms.Path(process.analyzeBasicPat)

