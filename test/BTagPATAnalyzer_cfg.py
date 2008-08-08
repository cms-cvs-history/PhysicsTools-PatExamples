#

import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatUtils.bJetOperatingPointsParameters_cfi import *

process = cms.Process("BTagPATAnalyzer")

#process.load("Analyzers.BTagPAT.selectedLooseBJets_cff")

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:PATLayer1_Output.fromAOD_full_ttbar.root')
    fileNames = cms.untracked.vstring('/store/relval/2008/7/21/RelVal-RelValTTbar-1216579481-IDEAL_V5-2nd/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/CMSSW_2_1_0_pre9-RelVal-1216579481-IDEAL_V5-2nd-unmerged/0000/00BCD825-6E57-DD11-8C1F-000423D98EA8.root')
)


process.MessageLogger = cms.Service("MessageLogger")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V5::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


# PAT Layer 1
process.load("PhysicsTools.PatAlgos.patLayer0_cff") # need to load this
process.load("PhysicsTools.PatAlgos.patLayer1_cff") # even if we run only layer 1


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('btagpatanalyzerpy.root')
)
 
# request a summary at the end of the file
process.options = cms.untracked.PSet(
     wantSummary = cms.untracked.bool(True)
)


process.BTagPATAnalyzerTC2 = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(True),
        tagger = cms.untracked.string('TC2'),
        purity = cms.string('Loose'),
        discriminator = cms.string('trackCountingHighEffBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(30.0),
        mindiscriminatorcut = cms.untracked.double(-10.0)
    )
)


process.BTagPATAnalyzerTC3 = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('TC3'),
        purity = cms.string('Loose'),
        discriminator = cms.string('trackCountingHighPurBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(30.0),
        mindiscriminatorcut = cms.untracked.double(-10.0)
    )
)

process.BTagPATAnalyzerTP = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('TP'),
        purity = cms.string('Loose'),
        discriminator = cms.string('jetProbabilityBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(2.6),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerBTP = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('BTP'),
        purity = cms.string('Loose'),
        discriminator = cms.string('jetBProbabilityBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(8.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)
process.BTagPATAnalyzerSSV = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('SSV'),
        purity = cms.string('Loose'),
        discriminator = cms.string('simpleSecondaryVertexBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(8.0),
        mindiscriminatorcut = cms.untracked.double(0.0)
    )
)

process.BTagPATAnalyzerCSV = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
   jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('CSV'),
        purity = cms.string('Loose'),
        discriminator = cms.string('combinedSecondaryVertexBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerMSV = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('MSV'),
        purity = cms.string('Loose'),
        discriminator = cms.string('combinedSecondaryVertexMVABJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerIPM = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('IPM'),
        purity = cms.string('Loose'),
        discriminator = cms.string('impactParameterMVABJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerSET = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('SET'),
        purity = cms.string('Loose'),
        discriminator = cms.string('softElectronBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerSMT = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('SMT'),
        purity = cms.string('Loose'),
        discriminator = cms.string('softMuonBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)

process.BTagPATAnalyzerSMNIPT = cms.EDAnalyzer("BTagPATAnalyzer",
    BJetOperatingPointsParameters,
    jetTag = cms.untracked.InputTag("selectedLayer1Jets"),
    BjetTag = cms.PSet(
        verbose = cms.untracked.bool(False),
        tagger = cms.untracked.string('SMNIPT'),
        purity = cms.string('Loose'),
        discriminator = cms.string('softMuonNoIPBJetTags'),
        maxdiscriminatorcut = cms.untracked.double(1.1),
        mindiscriminatorcut = cms.untracked.double(-0.1)
    )
)


process.p = cms.Path(

    process.patLayer0
    *process.patLayer1
    *process.BTagPATAnalyzerTC2
    *process.BTagPATAnalyzerTC3
    *process.BTagPATAnalyzerBTP
    *process.BTagPATAnalyzerSSV
    *process.BTagPATAnalyzerCSV
    *process.BTagPATAnalyzerMSV
    *process.BTagPATAnalyzerIPM
    *process.BTagPATAnalyzerSET
    *process.BTagPATAnalyzerSMT
    *process.BTagPATAnalyzerSMNIPT
    *process.BTagPATAnalyzerTP

 
)

