# This is an example PAT configuration showing the usage of PAT on full sim samples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# ----------------------------------------------------
# switch off new tau features introduced in 33X to
# restore 31X defaults new feaures:
# - shrinkingConeTaus instead of fixedCone ones
# - TaNC discriminants attached for shrinkingConeTaus
# - default preselection on cleaningLayer1
# ----------------------------------------------------
from PhysicsTools.PatAlgos.tools.tauTools import *
switchTo31Xdefaults(process)

# ----------------------------------------------------
# EXERCISE 1: create a patTuple which contains both 
#             selectedLayer1Candidates & cleanLayer1
#             Candidates & modify the cleanLayer1Jet
#             collection to discard jets overlaping
#             with isolated electrons
# ----------------------------------------------------
## add the selectedLayer1Candidates to the event content
#process.out.outputCommands+= ['keep *_selectedLayer1*_*_*']

## modfy the cleanLayer1Jets to discard overlapping jets
#process.cleanLayer1Jets.checkOverlaps.tkIsoElectrons.requireNoOverlaps=True

## let it run
#process.p = cms.Path(
#    process.patDefaultSequence
#    )

# ----------------------------------------------------
# EXERCISE 2: clone the cleanLayer1Jets module and 
#             modify it such that jets, which over- 
#             lap with electrons or photons with pt
#             larger than 20Gev are discarded from 
#             the collection
# ----------------------------------------------------
## modfy the cleanLayer1Jets to discard overlapping jets
process.myCleanLayer1Jets = process.cleanLayer1Jets.clone()
process.myCleanLayer1Jets.checkOverlaps.electrons.preselection='pt>20'
process.myCleanLayer1Jets.checkOverlaps.electrons.requireNoOverlaps=True
process.myCleanLayer1Jets.checkOverlaps.photons.preselection='pt>20'
process.myCleanLayer1Jets.checkOverlaps.photons.requireNoOverlaps=True

## add the our new jet collection to the event content
process.out.outputCommands+= ['keep *_myCleanLayer1Jets_*_*']

## let it run
process.p = cms.Path(
    process.patDefaultSequence *
    process.myCleanLayer1Jets
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
