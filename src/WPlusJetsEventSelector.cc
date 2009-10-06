
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
    int minJets,
    bool muPlusJets,
    bool ePlusJets,
    double muPtMin       , double muEtaMax,
    double elePtMin      , double eleEtaMax,
    double muPtMinLoose  , double muEtaMaxLoose,
    double elePtMinLoose , double eleEtaMaxLoose,
    double jetPtMin      , double jetEtaMax
    ) :
  muonIdTight_(muonIdTight),
  electronIdTight_(electronIdTight),
  jetIdTight_(jetIdTight),
  muonIdLoose_(muonIdLoose),
  electronIdLoose_(electronIdLoose),
  jetIdLoose_(jetIdLoose),
  minJets_(minJets),
  muPlusJets_(muPlusJets),
  ePlusJets_(ePlusJets),
  muPtMin_(muPtMin), muEtaMax_(muEtaMax),
  elePtMin_(elePtMin), eleEtaMax_(eleEtaMax),
  muPtMinLoose_(muPtMinLoose), muEtaMaxLoose_(muEtaMaxLoose),
  elePtMinLoose_(elePtMinLoose), eleEtaMaxLoose_(eleEtaMaxLoose),
  jetPtMin_(jetPtMin), jetEtaMax_(jetEtaMax)
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Lepton"    );
  push_back( "== 1 Lepton"    );
  push_back( "Tight Jet Cuts", minJets );
  push_back( "MET Cut"        );
  push_back( "Z Veto"         );
  push_back( "Conversion Veto");
  push_back( "Cosmic Veto"    );
}

bool WPlusJetsEventSelector::operator() ( pat::PatSummaryEvent const & t, std::strbitset & ret)
{
  selectedJets_.clear();
  selectedMuons_.clear();
  selectedElectrons_.clear();
  looseMuons_.clear();
  looseElectrons_.clear();
  selectedMETs_.clear();

  passCut( ret, "Inclusive");


  for ( std::vector<pat::Muon>::const_iterator muonBegin = t.muons.begin(),
	  muonEnd = t.muons.end(), imuon = muonBegin;
	imuon != muonEnd; ++imuon ) {
    if ( imuon->isGlobalMuon() ) {
      // Tight cuts
      std::strbitset iret = muonIdTight_->getBitTemplate();
      if ( imuon->pt() > muPtMin_ && fabs(imuon->eta()) < muEtaMax_ && 
	   (*muonIdTight_)(*imuon, iret) ) {
	selectedMuons_.push_back( *imuon );
      } else {
	// Loose cuts
	std::strbitset iret = muonIdLoose_->getBitTemplate();
	if ( imuon->pt() > muPtMinLoose_ && fabs(imuon->eta()) < muEtaMaxLoose_ && 
	     (*muonIdLoose_)(*imuon, iret) ) {
	  looseMuons_.push_back( *imuon );
	}
      }
    }
  }


  for ( std::vector<pat::Electron>::const_iterator electronBegin = t.electrons.begin(),
	  electronEnd = t.electrons.end(), ielectron = electronBegin;
	ielectron != electronEnd; ++ielectron ) {
    // Tight cuts
    std::strbitset iret = electronIdTight_->getBitTemplate();
    if ( ielectron->pt() > elePtMin_ && fabs(ielectron->eta()) < eleEtaMax_ && 
	 (*electronIdTight_)(*ielectron, iret) ) {
      selectedElectrons_.push_back( *ielectron );
    } else {
      // Loose cuts
      std::strbitset iret = electronIdLoose_->getBitTemplate();
      if ( ielectron->pt() > elePtMinLoose_ && fabs(ielectron->eta()) < eleEtaMaxLoose_ && 
	   (*electronIdLoose_)(*ielectron, iret) ) {
	looseElectrons_.push_back( *ielectron );
      }
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


  pat::TriggerEvent const * trig = t.triggerEvent;

  bool passTrig = false;

  if ( trig->wasRun() && trig->wasAccept() ) {

    pat::TriggerPath const * muPath = trig->path("HLT_Mu9");

    pat::TriggerPath const * elePath = trig->path("HLT_Ele15_LW_L1R");

    if ( muPlusJets_ && muPath != 0 && muPath->wasAccept() ) {
      passTrig = true;    
    }

    if ( ePlusJets_ && elePath != 0 && elePath->wasAccept() ) {
      passTrig = true;
    }

    if ( passTrig ) {
      passCut(ret, "Trigger");

      int nleptons = 0;
      if ( muPlusJets_ )
	nleptons += selectedMuons_.size();
      
      if ( ePlusJets_ ) 
	nleptons += selectedElectrons_.size();

      if ( (*this)[">= 1 Lepton"] || 
	   ( nleptons > 0 ) ){
	passCut( ret, ">= 1 Lepton");

	bool oneMuon = 
	  ( selectedMuons_.size() == 1 && 
	    looseMuons_.size() + selectedElectrons_.size() + looseElectrons_.size() == 0 
	    );
	bool oneElectron = 
	  ( selectedElectrons_.size() == 1 &&
	    selectedMuons_.size() + looseMuons_.size() == 0
	    );

	if ( (*this)["== 1 Lepton"] || 
	     ( (muPlusJets_ && oneMuon) ^ (ePlusJets_ && oneElectron )  )
	     ) {
	  passCut(ret, "== 1 Lepton");

	  if ( (*this)["Tight Jet Cuts"] ||
	       selectedJets_.size() >=  this->cut("Tight Jet Cuts", int()) ){
	    passCut(ret,"Tight Jet Cuts");
	  

	    bool metCut = true;
	    if ( (*this)["MET Cut"] ||
		 metCut ) {
	      passCut( ret, "MET Cut" );
	  

	      bool zVeto = true;
	      if ( selectedMuons_.size() == 2 ) {
	      }
	      if ( selectedElectrons_.size() == 2 ) {
	      }
	      if ( (*this)["Z Veto"] ||
		   zVeto ){
		passCut(ret, "Z Veto");
	    
  
		bool conversionVeto = true;
		if ( (*this)["Conversion Veto"] ||
		     conversionVeto ) {
		  passCut(ret,"Conversion Veto");
		


		  bool cosmicVeto = true;
		  if ( (*this)["Cosmic Veto"] ||
		       cosmicVeto ) {
		    passCut(ret,"Cosmic Veto");

		  
		  } // end if cosmic veto
		
		} // end if conversion veto

	      } // end if z veto

	    } // end if met cut
      
	  } // end if 1 tight jet 
	
	} // end if == 1 lepton

      } // end if >= 1 lepton
    
    } // end if trigger

  } // end if event triggered

  return (bool)ret;
}
