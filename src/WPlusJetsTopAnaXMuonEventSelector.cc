
#include "PhysicsTools/PatExamples/interface/WPlusJetsTopAnaXMuonEventSelector.h"

#include <iostream>

using namespace std;

WPlusJetsTopAnaXMuonEventSelector::WPlusJetsTopAnaXMuonEventSelector( 
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
  push_back( "Inclusive"         );
  push_back( "Trigger + 1 GlMu"  );
  push_back( "1 Tight Mu"        );
  push_back( ">= 4 Jets"         , minJets_ );
  push_back( "No Loose Muons"    );
  push_back( "No Loose Electrons");

  // all on by default
  set( "Inclusive"         );
  set( "Trigger + 1 GlMu"  );
  set( "1 Tight Mu"        );
  set( ">= 4 Jets"         );
  set( "No Loose Muons"    );
  set( "No Loose Electrons");

}

bool WPlusJetsTopAnaXMuonEventSelector::operator() ( pat::PatSummaryEvent const & t, std::strbitset & ret)
{
  selectedJets_.clear();
  selectedMuons_.clear();
  selectedElectrons_.clear();
  looseMuons_.clear();
  looseElectrons_.clear();
  selectedMETs_.clear();

  passCut( ret, "Inclusive");

  int nGlobalMuons = 0;
  for ( std::vector<pat::Muon>::const_iterator muonBegin = t.muons.begin(),
	  muonEnd = t.muons.end(), imuon = muonBegin;
	imuon != muonEnd; ++imuon ) {
    if ( imuon->isGlobalMuon() ) {

      nGlobalMuons++;
      // Tight cuts
      std::strbitset iret = muonIdTight_->getBitTemplate();
      if ( imuon->pt() > muPtMin_ && fabs(imuon->eta()) < muEtaMax_ && 
	   (*muonIdTight_)(*imuon, iret) ) {
	selectedMuons_.push_back( *imuon );
      } 
      else {
	// Loose cuts
	std::strbitset iret2 = muonIdLoose_->getBitTemplate();
	if ( imuon->pt() > muPtMinLoose_ && fabs(imuon->eta()) < muEtaMaxLoose_ && 
	     (*muonIdLoose_)(*imuon, iret2) ) {
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
      std::strbitset iret2 = electronIdLoose_->getBitTemplate();
      if ( ielectron->pt() > elePtMinLoose_ && fabs(ielectron->eta()) < eleEtaMaxLoose_ && 
	   (*electronIdLoose_)(*ielectron, iret2) ) {
	looseElectrons_.push_back( *ielectron );
      }
    }
  }

  for ( std::vector<pat::Jet>::const_iterator jetBegin = t.jets.begin(),
	  jetEnd = t.jets.end(), ijet = jetBegin;
	ijet != jetEnd; ++ijet ) {
    std::strbitset iret = jetIdTight_->getBitTemplate();
    if ( ijet->pt() > jetPtMin_ &&  fabs(ijet->eta()) < jetEtaMax_  ) {
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

    if ( ignoreCut("Trigger + 1 GlMu") || 
	 (passTrig && nGlobalMuons >= 1) ) {
      passCut(ret, "Trigger + 1 GlMu");

//       cout << "Passed trigger" << endl;

//       for ( std::vector<pat::Muon>::const_iterator muonBegin = t.muons.begin(),
// 	      muonEnd = t.muons.end(), imuon = muonBegin;
// 	    imuon != muonEnd; ++imuon ) {
// 	if ( imuon->isGlobalMuon() ) {

// 	  double hcalIso = imuon->hcalIso();
// 	  double ecalIso = imuon->ecalIso();
// 	  double trkIso  = imuon->trackIso();
// 	  double pt      = imuon->pt() ;
      
// 	  double relIso = (ecalIso + hcalIso + trkIso) / pt;


// 	  char buff[1000];
// 	  sprintf(buff,
// 		  "mujets:passStep1:%8.4g:%8.4g:%8d:%8.4g:%8.4g:%8.4g:%8.4g:%8.4g:%8.4g:%8.4g",
// 		  imuon->pt(),
// 		  imuon->eta(),
// 		  imuon->numberOfValidHits(),
// 		  imuon->dB(),
// 		  imuon->normChi2(),
// 		  imuon->isolationR03().emVetoEt,
// 		  imuon->isolationR03().hadVetoEt,
// 		  trkIso,
// 		  ecalIso + hcalIso,
// 		  relIso);
// 	  cout << buff << endl;
// 	}

//       }
      
      if ( ignoreCut("1 Tight Mu") || 
	   ( selectedMuons_.size()  == 1 ) ){
	passCut( ret, "1 Tight Mu");

// 	cout << "mujets:passStep2:Number of jets = " << selectedJets_.size() << endl;

	if ( ignoreCut(">= 4 Jets") ||
	     static_cast<int>(selectedJets_.size()) >=  this->cut(">= 4 Jets", int()) ){
	  passCut(ret,">= 4 Jets");

	  
	  if ( ignoreCut("No Loose Muons") ||
	       looseMuons_.size() == 0 ){
	    passCut(ret,"No Loose Muons");
	    

	    
	    if ( ignoreCut("No Loose Electrons") ||
		 looseElectrons_.size() + selectedElectrons_.size() == 0 ){
	      passCut(ret,"No Loose Electrons");
	      
	    } // end if no loose electrons
	    
	  } // end if no loose muons
      
	} // end if 1 tight jet 
	
      } // end if == 1 lepton
      
    } // end if trigger and 1 global lepton
    
  } // end if event triggered
  
  return (bool)ret;
}
