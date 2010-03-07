# This is an example PAT configuration showing the usage of PAT on full sim samples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# ----------------------------------------------------
# EXERCISE 1: embedding of calo towers in jets
# ----------------------------------------------------
## add calo towers to the event content
#process.out.outputCommands += ["keep *_towerMaker_*_*"]

## drop calo towers but embed those, which belong to
## jets
#process.patJets.embedCaloTowers = True


# ----------------------------------------------------
# EXERCISE 2: embedding of tracks in electrons
# ----------------------------------------------------
## add general tracks to the event content
#process.out.outputCommands += ["keep *_electronGsfTracks_*_*", "keep *_gsfElectron*_*_*"]

## drop general tracks but embed the track, which belongs
## to the electrons
#process.patElectrons.embedGsfTrack = False

# let it run
process.p = cms.Path(
    process.patDefaultSequence
    )

# ----------------------------------------------------
# You might want to change some of these default
# parameters
# ----------------------------------------------------
#process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#process.source.fileNames = [
#'/store/relval/CMSSW_3_1_1/RelValCosmics/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/7625DA7D-E36B-DE11-865A-000423D174FE.root'
#                            ]         ##  (e.g. 'file:AOD.root')
#process.maxEvents.input = ...         ##  (e.g. -1 to run on all events)
#process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#process.out.fileName = ...            ##  (e.g. 'myTuple.root')
#process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)
