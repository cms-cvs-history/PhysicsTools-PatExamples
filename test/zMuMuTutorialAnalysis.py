import FWCore.ParameterSet.Config as cms

process = cms.Process("ZMuMuTutorialAnalysis")

process.TFileService=cms.Service(
    "TFileService",
    fileName=cms.string("zmumu_plot.root")
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring("file:testSkim_v2.root")
    )

process.zmumuAnalysis = cms.EDAnalyzer(
    "ZMuMuTutorialAnalyzer",
    src = cms.InputTag("dimuons"),
    isocut = cms.double(3.0),
    etacut = cms.double(2),
    ptcut = cms.double(20),
    minZmass = cms.double(20),
    maxZmass = cms.double(200),
    dRConeVeto = cms.double(0.015),
    ptThresholdVeto = cms.double(1.5),
    dRIsolationCone = cms.double(0.3)
    )

process.eventInfo = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(
    process.zmumuAnalysis
    )

process.endPath = cms.EndPath(
    process.eventInfo
)
