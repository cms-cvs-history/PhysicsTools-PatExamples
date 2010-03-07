# This is an example PAT configuration showing the usage of PAT on full sim samples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## produce patTuple that contains selectedPatCandidates and
## cleanPatCandidates to be able to compare both collections
##
## NOTE that you can produce the selectedPatCandidates in an
##      extra patTuple beforehand and run the pat cleaning
##      on top the selectedPatCandidates during a later step
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands+= patEventContentNoCleaning

## reconfigure the cleaning of jets according to the 
## requirements of Exercise 2
#process.cleanPatJets.checkOverlaps.tkIsoElectrons.requireNoOverlaps=True

## reconfigure the cleaning of jets according to the
## requirements of Exercise 3
#process.cleanPatJets.checkOverlaps.photons.preselection="pt>20"
#process.cleanPatJets.checkOverlaps.photons.requireNoOverlaps=True
#process.cleanPatJets.checkOverlaps.electrons.preselection="pt>20"
#process.cleanPatJets.checkOverlaps.electrons.requireNoOverlaps=True

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
