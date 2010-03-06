# This is an example PAT configuration showing the usage of PAT on full sim samples

# starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# add muon tools
#from PhysicsTools.PatAlgos.tools.muonTools import *
# add all pre-defined userIsolations
#addMuonUserIsolation(process, ['All'])

# add muon tools
from PhysicsTools.PatExamples.muonTools import *
addMyUserIsolation(process)

# let it run
process.p = cms.Path(
  process.patDefaultSequence
)

# ----------------------------------------------------
# You might want to change some of these default
# parameters
# ----------------------------------------------------
process.maxEvents.input = -1
process.options.wantSummary = False
