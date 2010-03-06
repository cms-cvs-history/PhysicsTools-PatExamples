import FWCore.ParameterSet.Config as cms

def addMyUserIsolation(process):
    ## embed all kinds of isoDeposits
    process.patMuons.isoDeposits = cms.PSet(
      tracker = cms.InputTag("muIsoDepositTk"),
      ecal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal"),
      hcal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal"),
      user    = cms.VInputTag(
                   cms.InputTag("muIsoDepositCalByAssociatorTowers","ho"), 
                   cms.InputTag("muIsoDepositJets")
                ),
    )
    ## define all kinds of userIsolation
    process.patMuons.userIsolation = cms.PSet(
      tracker = cms.PSet(
          src = cms.InputTag("muIsoDepositTk"),
          deltaR = cms.double(0.4)
      ),
      ecal = cms.PSet(
          src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal"),
          deltaR = cms.double(0.3),
         
      ),
      hcal = cms.PSet(
          src = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal"),
          deltaR = cms.double(0.3)
      ),
      user = cms.VPSet(
        cms.PSet(
          src = cms.InputTag("muIsoDepositTk"),
          deltaR = cms.double(0.3),
          mode = cms.string("max")
        ),         
        cms.PSet(        
          src = cms.InputTag("muIsoDepositTk"),
          deltaR = cms.double(0.3),
          mode = cms.string("sum")
        ), 
        cms.PSet(
          src = cms.InputTag("muIsoDepositTk"),
          deltaR = cms.double(0.3),
          mode = cms.string("sum2")
        ), 
        cms.PSet(
          src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ho"),
          deltaR = cms.double(0.5)
        ), 
        cms.PSet(
          src = cms.InputTag("muIsoDepositJets"),
          deltaR = cms.double(0.5)
        )
      )
    )

