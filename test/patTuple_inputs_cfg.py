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
# EXAMPLE 1: run with 31X samples 
# ----------------------------------------------------
## uncomment the following line to run on 31X RECO inputs
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run33xOn31xMC( process,
#               jetSrc = cms.InputTag("antikt5CaloJets"),
#               jetIdTag = "antikt5"
#               )
#process.source.fileNames = [
# '/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V2-v1/0006/0CC00E3B-5A78-DE11-A2AB-000423D94A04.root'
#]

## uncomment the following line to run on 31X AOD inputs
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run33xOn31xMC( process,
#               jetSrc = cms.InputTag("antikt5CaloJets"),
#               jetIdTag = "antikt5"
#               )
#restrictInputToAOD31X( process )
#process.source.fileNames = [
# '/store/mc/Summer09/TTbar/AODSIM/MC_31X_V3_156BxLumiPileUp_AODSIM-v1/0000/0CD4A3C1-328E-DE11-AD09-003048C7584C.root'
#]

# ----------------------------------------------------
# EXAMPLE 2: run with CRAFT09 data 
# ----------------------------------------------------
#from PhysicsTools.PatAlgos.tools.coreTools import *
#process.patDefaultSequence.remove(process.makeAllLayer1Jets)
#removeAllPATObjectsBut(process, ['Muons'])
#removeMCMatching(process, 'Muons')
#process.source.fileNames = [
# '/store/data/CRAFT09/Cosmics/RECO/multiCosmicMuon-CRAFT09_R_V4_CosmicsSeq_v1/0000/068303DC-80B8-DE11-9110-003048678F62.root'
#]

# ----------------------------------------------------
# EXAMPLE 3: run with collison data 
# ----------------------------------------------------
#from PhysicsTools.PatAlgos.tools.coreTools import *
#removeMCMatching(process, 'All')
#process.source.fileNames = [
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/F62B040F-6CD8-DE11-9007-001D09F24664.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/F4387297-74D8-DE11-996C-001D09F24F1F.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/EE7B1AC4-6CD8-DE11-97BB-0030487A1FEC.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/9264A28F-87D8-DE11-83A1-001D09F24763.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/7C7495C2-71D8-DE11-ACF2-001D09F248F8.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/4CAB3B6C-6BD8-DE11-845C-000423D9890C.root',
# '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/122/314/02E8544C-70D8-DE11-85CF-001617C3B66C.root'
#]
#process.source.firstRun = cms.untracked.uint32(122314)

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
