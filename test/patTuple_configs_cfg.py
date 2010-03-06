# This is an example PAT configuration showing the usage of PAT on full sim samples

# starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# do the proper replacements as required by the exercise
process.patJetCorrFactors.corrSample = 'Summer09_7TeV'
process.patJetCorrFactors.sampleType = 'ttbar'

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
