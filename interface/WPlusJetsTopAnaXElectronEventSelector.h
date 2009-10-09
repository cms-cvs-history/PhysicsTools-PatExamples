#ifndef PhysicsTools_PatExamples_interface_WPlusJetsTopAnaXElectronEventSelector_h
#define PhysicsTools_PatExamples_interface_WPlusJetsTopAnaXElectronEventSelector_h

#include "PhysicsTools/PatUtils/interface/PatEventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "PhysicsTools/PatUtils/interface/ElectronVPlusJetsIDSelectionFunctor.h"
#include "PhysicsTools/PatUtils/interface/MuonVPlusJetsIDSelectionFunctor.h"
#include "PhysicsTools/PatUtils/interface/JetIDSelectionFunctor.h"

class WPlusJetsTopAnaXElectronEventSelector : public pat::PatEventSelector {
 public:
  WPlusJetsTopAnaXElectronEventSelector( 
   boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor> & muonId,
   boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor> & electronIdTight,
   boost::shared_ptr<JetIDSelectionFunctor> & jetIdTight,
   boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor> & electronIdLoose,
   boost::shared_ptr<JetIDSelectionFunctor> & jetIdLoose,
   int minJets,
   bool muPlusJets,
   bool ePlusJets,
   double muPtMin       , double muEtaMax,
   double elePtMin      , double eleEtaMax,
   double muPtMinLoose  , double muEtaMaxLoose,
   double elePtMinLoose , double eleEtaMaxLoose,
   double jetPtMin      , double jetEtaMax,
   double dR
			  );
  
  virtual bool operator()( pat::PatSummaryEvent const & t, std::strbitset & ret);

  std::vector<pat::Jet>      const & selectedJets     () const { return selectedJets_;     } 
  std::vector<pat::Electron> const & selectedElectrons() const { return selectedElectrons_;}
  std::vector<pat::Muon>     const & selectedMuons    () const { return selectedMuons_;    }
  std::vector<pat::Electron> const & selectedlooseElectrons_() const { return looseElectrons_ ; }
 protected: 

  std::vector<pat::Jet>       selectedJets_;
  std::vector<pat::Muon>      selectedMuons_;
  std::vector<pat::Electron>  selectedElectrons_;
  std::vector<pat::Muon>      looseMuons_;
  std::vector<pat::Electron>  looseElectrons_;
  std::vector<pat::MET>       selectedMETs_;

  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonId_;
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight_;
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight_;
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdLoose_;
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdLoose_;

  int minJets_;
  bool muPlusJets_;
  bool ePlusJets_;

  double muPtMin_  ;
  double muEtaMax_ ;
  double elePtMin_ ;
  double eleEtaMax_;

  double muPtMinLoose_  ;
  double muEtaMaxLoose_ ;
  double elePtMinLoose_ ;
  double eleEtaMaxLoose_;

  double jetPtMin_ ;
  double jetEtaMax_;

  double dR_;

/*   TFile * outputPlots_; */
/*   TH1D * step1_muonPt_; */
/*   TH1D * step1_muonEta_; */
/*   TH1D * step1_muonNValid_; */
/*   TH1D * step1_muonDB_; */
/*   TH1D * step1_muonNormChi2_; */
/*   TH1D * step1_muonNormEmVeto_; */
/*   TH1D * step1_muonNormHadVeto_; */
/*   TH1D * step1_muonTrkIso_; */
/*   TH1D * step1_muonCalIso_; */
/*   TH1D * step1_muonRelIso_; */
  
};


#endif
