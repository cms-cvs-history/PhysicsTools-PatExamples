import FWCore.ParameterSet.Config as cms

process = cms.Process("Exercise3")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:PATLayer1_Output.fromAOD_full.root'
  )
)

process.MessageLogger = cms.Service("MessageLogger")
process.load("PhysicsTools/PatExamples/PatMETAnalyzer_cff")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('analyzePatMETs.root')
                                   )

## do Exercise 3
process.p = cms.Path(process.comparePatMETs)

