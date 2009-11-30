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
# EXERCISE 1: change the pat jet collection in the 
#             event content
# ----------------------------------------------------
#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJetCollection(process, 
#                    cms.InputTag('sisCone5CaloJets'),   
#                    doJTA            = True,            
#                    doBTagging       = True,            
#                    jetCorrLabel     = ('SC5','Calo'),  
#                    doType1MET       = True,            
#                    genJetCollection = cms.InputTag("sisCone5GenJets"),
#                    doJetID          = True,
#                    jetIdLabel       = "sc5"
#                    ) 

# ----------------------------------------------------
# EXERCISE 2: add more jet collections to the pat
#             event content
# ----------------------------------------------------
#from PhysicsTools.PatAlgos.tools.jetTools import *
#addJetCollection(process,cms.InputTag('ak7CaloJets'),
#                 'AK7',
#                 doJTA        = True,
#                 doBTagging   = False,
#                 jetCorrLabel = None,
#                 doType1MET   = True,
#                 doL1Cleaning = True,                 
#                 doL1Counters = False,
#                 genJetCollection=cms.InputTag("ak7GenJets"),
#                 doJetID      = True,
#                 jetIdLabel   = "ak7"
#                 )
#addJetCollection(process,cms.InputTag('iterativeCone5CaloJets'),
#                 'IC5',
#                 doJTA        = True,
#                 doBTagging   = True,
#                 jetCorrLabel = ('IC5', 'Calo'),
#                 doType1MET   = True,
#                 doL1Cleaning = True,                 
#                 doL1Counters = False,
#                 genJetCollection=cms.InputTag("iterativeCone5GenJets"),
#                 doJetID      = False
#                 )
#process.out.outputCommands += ["keep *_cleanLayer1Jets*_*_*",
#                               "keep *_selectedLayer1Jets*_*_*",                               
#                               "keep *_layer1METs*_*_*"
#                               ]

# ----------------------------------------------------
# EXERCISE 3: add different kinds of MET to the event
#             content
# ----------------------------------------------------
#from PhysicsTools.PatAlgos.tools.metTools import *
#addTcMET(process, 'TC')
#addPfMET(process, 'PF')
#process.out.outputCommands += ["keep *_layer1METs*_*_*"
#                               ]

# ----------------------------------------------------
# EXERCISE 4: switch to different standard ouputs of
#             the pat tuple
# ----------------------------------------------------
## switched from cleanLayer1Candidates to selectedLayer1Candidates
#from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
#removeCleaning(process)

## add AODExtras to the event content
#from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
#process.out.outputCommands+= patExtraAodEventContent 

# ----------------------------------------------------
# EXERCISE 5: add user defined isolation to a
#             pat::Candidate
# ----------------------------------------------------
## embed isoDeposits for later use in the isolation
## calculation in the analysis
#process.allLayer1Muons.isoDeposits = cms.PSet(
#    tracker = cms.InputTag("muIsoDepositTk"),
#    ecal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal"),
#    hcal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal"),
#    user    = cms.VInputTag(
#      cms.InputTag("muIsoDepositCalByAssociatorTowers","ho"), 
#      cms.InputTag("muIsoDepositJets")
#    )
#)

## define different types of isolation
#process.allLayer1Muons.userIsolation = cms.PSet(
#    tracker = cms.PSet(
#      ## from tracker with deltaR<0.2     
#      src = cms.InputTag("muIsoDepositTk"),
#      deltaR = cms.double(0.2),
#      mode = cms.string("sumRelative")
#    ),
#    hcal = cms.PSet(
#      ## from hcal isoDeposits with deltaR<0.5 
#      src = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal"),
#      deltaR = cms.double(0.5)
#    ),
#    ecal = cms.PSet(
#      ## from ecal isoDeposits with deltaR<0.5     
#      src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal"),
#      deltaR = cms.double(0.5)
#    ),
#    user = cms.VPSet(
#      cms.PSet(
#        ## from hcal outer isoDeposits with deltaR<0.5 
#        src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ho"),
#        deltaR = cms.double(0.5)
#      ), 
#      cms.PSet(
#        ## from isoDeposits from jets deltaR<0.5     
#        src = cms.InputTag("muIsoDepositJets"),
#        deltaR = cms.double(0.5)
#      )
#    )
#)

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
