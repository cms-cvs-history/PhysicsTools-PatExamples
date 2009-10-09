
#include "PhysicsTools/PatExamples/interface/WPlusJetsTopAnaXElectronEventSelector.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

using namespace std;

WPlusJetsTopAnaXElectronEventSelector::WPlusJetsTopAnaXElectronEventSelector( 
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
    ) :
  muonId_(muonId),
  electronIdTight_(electronIdTight),
  jetIdTight_(jetIdTight),
  electronIdLoose_(electronIdLoose),
  jetIdLoose_(jetIdLoose),
  minJets_(minJets),
  muPlusJets_(muPlusJets),
  ePlusJets_(ePlusJets),
  muPtMin_(muPtMin), muEtaMax_(muEtaMax),
  elePtMin_(elePtMin), eleEtaMax_(eleEtaMax),
  muPtMinLoose_(muPtMinLoose), muEtaMaxLoose_(muEtaMaxLoose),
  elePtMinLoose_(elePtMinLoose), eleEtaMaxLoose_(eleEtaMaxLoose),
  jetPtMin_(jetPtMin), jetEtaMax_(jetEtaMax),
  dR_(dR)
{
  // make the bitset
  push_back( "Inclusive"         );
  push_back( "Trigger + at least 1 e"  );
  push_back( "1 Tight e"        );
  push_back( "No Muons"        );
  push_back( "Remove Jets"    );
  push_back( ">= 4 Jets");

  // all on by default
  set( "Inclusive"         );
  set( "Trigger + at least 1 e"  );
  set( "1 Tight e"        );
  set( "No Muons"         );
  set( "Remove Jets"    );
  set( ">= 4 Jets");

}

bool WPlusJetsTopAnaXElectronEventSelector::operator() ( pat::PatSummaryEvent const & t, std::strbitset & ret)
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
	if( imuon->isGlobalMuon() ) {
      		std::strbitset iret = muonId_->getBitTemplate();
      		if ( imuon->pt() > muPtMin_ && fabs(imuon->eta()) < muEtaMax_ && 
	   	(*muonId_)(*imuon, iret) ) {
		   selectedMuons_.push_back( *imuon );
		}
      	} // end if globle muon
  }
  int nElectrons = 0;

  for ( std::vector<pat::Electron>::const_iterator electronBegin = t.electrons.begin(),
	  electronEnd = t.electrons.end(), ielectron = electronBegin;
	ielectron != electronEnd; ++ielectron ) {
    // Tight cuts
    if( t.electrons.size() != 0 )  nElectrons++; 
    std::strbitset iret = electronIdTight_->getBitTemplate();
    if ( ielectron->et() > elePtMin_ && fabs(ielectron->eta()) < eleEtaMax_ && 
	 (*electronIdTight_)(*ielectron, iret) && 
	 ielectron->electronID( "eidRobustTight" ) > 0 ) {
      selectedElectrons_.push_back( *ielectron );
    }
    
    // Loose cuts
    std::strbitset iret2 = electronIdLoose_->getBitTemplate();
    if ( ielectron->pt() > elePtMin_ && fabs(ielectron->eta()) < eleEtaMax_ && 
	   (*electronIdLoose_)(*ielectron, iret2) &&
	   ielectron->electronID( "eidRobustTight" ) > 0 ) {
	looseElectrons_.push_back( *ielectron );
     
    }
  }

  for ( std::vector<pat::Jet>::const_iterator jetBegin = t.jets.begin(),
	  jetEnd = t.jets.end(), ijet = jetBegin;
	ijet != jetEnd; ++ijet ) {
    std::strbitset iret = jetIdTight_->getBitTemplate();
    if ( ijet->pt() > jetPtMin_ &&  fabs(ijet->eta()) < jetEtaMax_  ) {
      //Remove some jets
      bool indeltaR = false;
      for( std::vector<pat::Electron>::const_iterator electronBegin = looseElectrons_.begin(),
         electronEnd = looseElectrons_.end(), ielectron = electronBegin;
	 ielectron != electronEnd; ++ielectron ) 
	    if( reco::deltaR( ielectron->eta(), ielectron->phi(), ijet->eta(), ijet->phi() ) < dR_ ) 
	    	indeltaR = true;
      if( !indeltaR )  selectedJets_.push_back( *ijet );
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

    if ( ignoreCut("Trigger + at least 1 e") || 
	 (passTrig && nElectrons >= 1) ) {
      passCut(ret, "Trigger + at least 1 e");

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
      
      if ( ignoreCut("1 Tight e") || 
	   ( selectedElectrons_.size()  == 1 ) ){
	passCut( ret, "1 Tight e");

// 	cout << "mujets:passStep2:Number of jets = " << selectedJets_.size() << endl;

	if ( ignoreCut("No Muons") ||
	    selectedMuons_.size() == 0 ){
	  passCut(ret,"No Muons");

	  
	  passCut(ret,"Remove Jets");
	    

	    
	    if ( ignoreCut(">= 4 Jets") ||
		 selectedJets_.size() >= 4 ){
	      passCut(ret,">= 4 Jets");
	      
	    } // end if >= 4 jets
	    
	  } // end if no muon
      
	} // end if one tight electron
	
    } // end if trigger and 1 electron
    
  } // end if event triggered
  
  return (bool)ret;
}
