import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
	default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
	PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.GlobalTag.globaltag = cms.string('IDEAL_V12::All')

# produce PAT Layer 1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
# switch old trigger matching off
from PhysicsTools.PatAlgos.tools.trigTools import switchOffTriggerMatchingOld
switchOffTriggerMatchingOld( process )

# source
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
#		'/store/mc/Summer08/TauolaTTbar/USER/IDEAL_V9_PAT_v2/0000/F4E0BF7B-42CD-DD11-94BB-00E08178C0F3.root',
#		'/store/mc/Fall08/TTJets-madgraph/GEN-SIM-RECO/IDEAL_V11_redigi_v10/0000/06FC3959-4DFC-DD11-B504-00E08178C091.root',
		'file:TTJets-madgraph-PAT.root'
	)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService",
	fileName = cms.string("analyzePatBJetTags.root")
)

process.analyzeBJetTags = cms.EDAnalyzer("PatBJetTagAnalyzer",
	# input collections
	jets = cms.InputTag("selectedLayer1Jets"),

	#jet cuts
	jetPtCut = cms.double(50),
	jetEtaCut = cms.double(2.4)
)

process.p = cms.Path(
#	process.patDefaultSequence *
	process.analyzeBJetTags
)
