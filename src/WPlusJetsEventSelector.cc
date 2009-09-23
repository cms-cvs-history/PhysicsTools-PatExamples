
#include "PhysicsTools/PatExamples/interface/WPlusJetsEventSelector.h"

#include <iostream>

using namespace std;

WPlusJetsEventSelector::WPlusJetsEventSelector( 
    boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor> & muonIdTight,
    boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor> & electronIdTight,
    boost::shared_ptr<JetIDSelectionFunctor> & jetIdTight,
    boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor> & muonIdLoose,
    boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor> & electronIdLoose,
    boost::shared_ptr<JetIDSelectionFunctor> & jetIdLoose,
    double muPtMin  ,
    double elePtMin ,
    double jetPtMin
						) :
  muonIdTight_(muonIdTight),
  electronIdTight_(electronIdTight),
  jetIdTight_(jetIdTight),
  muonIdLoose_(muonIdLoose),
  electronIdLoose_(electronIdLoose),
  jetIdLoose_(jetIdLoose),
  muPtMin_(muPtMin),
  elePtMin_(elePtMin),
  jetPtMin_(jetPtMin)
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Lepton"    );
  push_back( "== 1 Lepton"    );
  push_back( ">= 1 Tight Jet" );
  push_back( "MET > 20"       );
  push_back( "Z Veto"         );
  push_back( "Conversion Veto");
  push_back( "Cosmic Veto"    );
}

bool WPlusJetsEventSelector::operator() ( pat::PatSummaryEvent const & t, std::strbitset & ret)
{
  selectedJets_.clear();
  selectedMuons_.clear();
  selectedElectrons_.clear();
  selectedMETs_.clear();

  passCut( ret, "Inclusive");

  for ( std::vector<pat::Muon>::const_iterator muonBegin = t.muons.begin(),
	  muonEnd = t.muons.end(), imuon = muonBegin;
	imuon != muonEnd; ++imuon ) {
    std::strbitset iret = muonIdTight_->getBitTemplate();
    if ( imuon->pt() > muPtMin_ && (*muonIdTight_)(*imuon, iret) ) {
      selectedMuons_.push_back( *imuon );
    }
  }

  for ( std::vector<pat::Electron>::const_iterator electronBegin = t.electrons.begin(),
	  electronEnd = t.electrons.end(), ielectron = electronBegin;
	ielectron != electronEnd; ++ielectron ) {
    std::strbitset iret = electronIdTight_->getBitTemplate();
    if ( ielectron->pt() > elePtMin_ && (*electronIdTight_)(*ielectron, iret) ) {
      selectedElectrons_.push_back( *ielectron );
    }
  }

  for ( std::vector<pat::Jet>::const_iterator jetBegin = t.jets.begin(),
	  jetEnd = t.jets.end(), ijet = jetBegin;
	ijet != jetEnd; ++ijet ) {
    std::strbitset iret = jetIdTight_->getBitTemplate();
    if ( ijet->pt() > jetPtMin_ && (*jetIdTight_)(*ijet, iret) ) {
      selectedJets_.push_back( *ijet );
    }
  }

  if ( true ) passCut(ret, "Trigger");

  if ( (*this)[">= 1 Lepton"] || 
       ( selectedMuons_.size() > 0 || selectedElectrons_.size() > 0 ) ){
    passCut( ret, ">= 1 Lepton");
  }

  if ( (*this)["== 1 Lepton"] || 
       ( selectedMuons_.size() + selectedElectrons_.size() == 1 ) ){
    passCut(ret, "== 1 Lepton");
  }

  if ( (*this)[">= 1 Tight Jet"] ||
       selectedJets_.size() > 0 ){
    passCut(ret,">= 1 Tight Jet");
  }

  bool metCut = true;
  if ( (*this)["MET > 20"] ||
       metCut ) {
    passCut( ret, "MET > 20" );
  }

  bool zVeto = true;
  if ( selectedMuons_.size() == 2 ) {
  }
  if ( selectedElectrons_.size() == 2 ) {
  }
  if ( (*this)["Z Veto"] ||
       zVeto ){
    passCut(ret, "Z Veto");
  }
  
  bool conversionVeto = true;
  if ( (*this)["Conversion Veto"] ||
       conversionVeto ) {
    passCut(ret,"Conversion Veto");
  }


  bool cosmicVeto = true;
  if ( (*this)["Cosmic Veto"] ||
       cosmicVeto ) {
    passCut(ret,"Cosmic Veto");
  }

  return (bool)ret;
}
