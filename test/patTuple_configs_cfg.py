# This is an example PAT configuration showing the usage of PAT on full sim samples

# starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# uncomment the following section for Exercise 3
from PhysicsTools.PatAlgos.tools.jetTools import *

# produce jpt corrected calo jets, which are not on AOD per default
process.load("PhysicsTools.PatAlgos.recoLayer0.jetPlusTrack_cff")
process.jpt = cms.Path(
    process.jptCaloJets
)

# add JPT jets
addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
  'AK5', 'JPT',
  doJTA        = True,
  doBTagging   = True,
  jetCorrLabel = None,
  doType1MET   = False,
  doL1Cleaning = True,
  doL1Counters = False,                 
  genJetCollection = cms.InputTag("ak5GenJets"),
  doJetID      = False
)

# add pflow jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
  'AK5', 'PF',
  doJTA        = True,
  doBTagging   = True,
  jetCorrLabel = ('AK5','PF'),
  doType1MET   = True,
  doL1Cleaning = True,                 
  doL1Counters = False,
  genJetCollection=cms.InputTag("ak5GenJets"),
  doJetID      = True,
  jetIdLabel   = "ak5"
)

from PhysicsTools.PatAlgos.tools.metTools import *
# add tc MET
addTcMET(process, 'TC')
# add pflow MET
addPfMET(process, 'PF')


# let it run
process.p = cms.Path(
  process.patDefaultSequence
)

# ----------------------------------------------------
# You might want to change some of these default
# parameters
# ----------------------------------------------------
process.maxEvents.input = -1
process.out.outputCommands+= [ 'keep *_ak5CaloJets_*_*' ]
process.options.wantSummary = False
