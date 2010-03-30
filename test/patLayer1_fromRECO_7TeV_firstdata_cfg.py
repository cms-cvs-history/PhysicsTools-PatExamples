# This is an example PAT configuration showing the usage of PAT on minbias data

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('START3X_V25::All')

# turn off MC matching for the process
#removeMCMatching(process, ['All'])

# add pf met
from PhysicsTools.PatAlgos.tools.metTools import *
removeMCMatching(process, ['All'])
addPfMET(process, 'PF')



# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

# run ak5 gen jets
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run33xOnReRecoMC( process, "ak5GenJets")

# Add PF jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA        = False,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5','PF'),
                 doType1MET   = False,
                 doL1Cleaning = False,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )

# require physics declared
process.physDecl = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
)

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )


# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# switch on PAT trigger
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger( process )

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(15), 
                                           maxd0 = cms.double(2) 
                                           )

# Select jets
process.selectedPatJets.cut = cms.string('pt > 10 & abs(eta) < 3.0')
process.selectedPatJetsAK5PF.cut = cms.string('pt > 8 & abs(eta) < 3.0')


# Add the files 
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [
       'file:0CBD356D-E53B-DF11-AC6F-000423D9970C.root',
       'file:6E907338-E83B-DF11-9DB1-000423D98A44.root'
        ] );
process.source.fileNames = readFiles

# let it run

print
print "============== Warning =============="
print "technical trigger filter:    DISABLED"
print "physics declare bit filter:  DISABLED"
print "primary vertex filter:       DISABLED"

process.p = cms.Path(
#    process.hltLevel1GTSeed*
    process.scrapingVeto*
#    process.physDecl*
#    process.primaryVertexFilter*
    process.patDefaultSequence
    )

# rename output file
process.out.fileName = cms.untracked.string('reco_7TeV_firstdata_356_pat.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = 1000
process.options.wantSummary = True

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
process.out.outputCommands = patEventContentNoCleaning
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += [
        'keep recoPFCandidates_particleFlow_*_*'
        ]

