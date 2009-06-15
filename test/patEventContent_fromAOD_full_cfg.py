import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root')
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP_V7::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
#process.content = cms.EDAnalyzer("EventContentAnalyzer")
# Switch off old trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOffTriggerMatchingOld
switchOffTriggerMatchingOld( process )

process.p = cms.Path(
    process.patDefaultSequence  
)

# Output module configuration
from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('PATLayer1_Output.fromAOD_full.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output
    outputCommands = cms.untracked.vstring('drop *')
)
process.out.outputCommands += patEventContent

# --------------------------------------------------------------------------
#
# Part1:
#
# uncomment the following lines to
# re-configure the event content
#
# --------------------------------------------------------------------------
# process.out.outputCommands += patTriggerEventContent
# process.out.outputCommands += patExtraAodEventContent
# process.out.outputCommands += patEventContentTriggerMatch
# process.out.outputCommands += patEventContentNoLayer1Cleaning
# process.out.outputCommands += ["keep *_selectedLayer1Jets*_*_*", ## add these lines when 
#                                "keep *_layer1METs*_*_*"]         ## adding additional jets
# process.out.outputCommands += ["drop *_towerMaker_*_*" ]         ## add this line to drop
                                                                   ## calo towers from event

# --------------------------------------------------------------------------
#
# Part2:
#
# uncomment the following lines to
# re-configure the pat::Jet content
#
# --------------------------------------------------------------------------
# switch embedding of calo towers on
# process.allLayer1Jets.embedCaloTowers = True
# switch embedding of btag information off
# process.allLayer1Jets.addBTagInfo     = False


process.outpath = cms.EndPath(process.out)

