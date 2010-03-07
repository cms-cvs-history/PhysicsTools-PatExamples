import FWCore.ParameterSet.Config as cms

myMuonMatch = cms.EDFilter("MCMatcher",     # cut on deltaR, deltaPt/Pt; pick best by deltaR
    src     = cms.InputTag("muons"),        # RECO objects to match
    matched = cms.InputTag("genParticles"), # mc-truth particle collection
    mcPdgId     = cms.vint32(0),            # one or more PDG ID (13 = muon); absolute values (see below)
    checkCharge = cms.bool(True),           # True = require RECO and MC objects to have the same charge
    mcStatus = cms.vint32(0),               # PYTHIA status code (1 = stable, 2 = shower, 3 = hard scattering)
    maxDeltaR = cms.double(0.),             # Minimum deltaR for the match
    maxDPtRel = cms.double(0.),             # Minimum deltaPt/Pt for the match
    resolveAmbiguities = cms.bool(True),    # Forbid two RECO objects to match to the same GEN object
    resolveByMatchQuality = cms.bool(True)  # False = just match input in order; True = pick lowest deltaR pair first
)
myJetGenJetMatch = cms.EDFilter("GenJetMatcher", # cut on deltaR, deltaPt/Pt; pick best by deltaR
    src      = cms.InputTag("ak5CaloJets"), # RECO jets (any View<Jet> is ok)
    matched  = cms.InputTag("ak5GenJets"),  # GEN jets  (must be GenJetCollection)
    mcPdgId  = cms.vint32(0),               # n/a
    mcStatus = cms.vint32(0),               # n/a
    checkCharge = cms.bool(True),           # n/a
    maxDeltaR = cms.double(0.),             # Minimum deltaR for the match
    maxDPtRel = cms.double(0.),             # Minimum deltaPt/Pt for the match
    resolveAmbiguities = cms.bool(True),    # Forbid two RECO objects to match to the same GEN object
    resolveByMatchQuality = cms.bool(False) # False = just match input in order; True = pick lowest deltaR pair first
)
