/*   A macro for making a histogram of Jet Pt with cuts
This is a basic way to cut out jets of a certain Pt and Eta using an if statement
This example creates a histogram of Jet Pt, using Jets with Pt above 30 and ETA above -2.1 and below 2.1
*/
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"


#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "PhysicsTools/PatExamples/interface/WPlusJetsEventSelector.h"
#endif

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

using namespace std;

int main ( int argc, char ** argv )
{

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();  
  
  cout << "About to allocate functors" << endl;

  // Tight muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdTight     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  // Tight electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  // Tight jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::TIGHT) );

  
  // Loose muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdLoose     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  muonIdLoose->set( "Chi2",    false);
  muonIdLoose->set( "D0",      false);
  muonIdLoose->set( "NHits",   false);
  muonIdLoose->set( "ECalIso", false);
  muonIdLoose->set( "HCalIso", false);
  muonIdLoose->set( "RelIso", 0.2 );

  // Loose electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdLoose     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08) );
  electronIdLoose->set( "D0",  false);
  electronIdLoose->set( "RelIso", 0.2 );
  // Loose jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdLoose      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::LOOSE) );

  cout << "Making event selector" << endl;
  boost::shared_ptr<WPlusJetsEventSelector> 
    wPlusJets( new WPlusJetsEventSelector( 
     muonIdTight,
     electronIdTight,
     jetIdTight,
     muonIdLoose,
     electronIdLoose,
     jetIdLoose,
     4,   // minJets
     true, // mu + jets
     false, // e + jets
     20,  // tight mu pt
     2.1, // tight mu eta
     30,  // tight ele pt
     2.4, // tight ele eta
     10,  // loose mu pt
     2.5, // loose mu eta
     15,  // loose ele pt
     2.5, // loose ele eta
     30,  // jet pt
     2.4  // jet eta
     ) 
  );
  

  TFile  * file = new TFile("vplusjets.root");
  TH1D * hist_jetPt = new TH1D("hist_jetPt", "Jet p_{T}", 20, 0, 100 );
  fwlite::Event ev(file);

  int count = 0;
  //loop through each event
  for( ev.toBegin();
         ! ev.atEnd();
       ++ev, ++count) {

    fwlite::Handle<std::vector<pat::Jet> > allJets;
    allJets.getByLabel(ev,"cleanLayer1Jets");
    if (!allJets.isValid() ) continue;

    fwlite::Handle<std::vector<pat::MET> > allMETs;
    allMETs.getByLabel(ev,"layer1METs");
    if (!allMETs.isValid() ) continue;

    fwlite::Handle<std::vector<pat::Muon> > allMuons;
    allMuons.getByLabel(ev,"cleanLayer1Muons");
    if (!allMuons.isValid() ) continue;

    fwlite::Handle<std::vector<pat::Electron> > allElectrons;
    allElectrons.getByLabel(ev,"cleanLayer1Electrons");
    if (!allElectrons.isValid() ) continue;

    fwlite::Handle<pat::TriggerEvent> triggerEvent;
    triggerEvent.getByLabel(ev, "patTriggerEvent");
    if (!triggerEvent.isValid() ) continue;

    pat::PatSummaryEvent summary;
    summary.jets      = *allJets;
    summary.METs      = *allMETs;
    summary.electrons = *allElectrons;
    summary.muons     = *allMuons;
    summary.triggerEvent   = &(*triggerEvent);

 
    std::strbitset ret = wPlusJets->getBitTemplate();


    bool passed = (*wPlusJets)(summary, ret);
//     vector<pat::Electron> const & electrons = wPlusJets->selectedElectrons();
//     vector<pat::Muon>     const & muons     = wPlusJets->selectedMuons();

//     vector<pat::Jet> const & jets = wPlusJets->selectedJets();

//     if ( passed ) {
//       for ( vector<pat::Jet>::const_iterator jetsBegin = jets.begin(),
// 	      jetsEnd = jets.end(), ijet = jetsBegin; 
// 	    ijet != jetsEnd; ++ijet) {
// 	//	cout << "Looking at each jet, pt,eta = " << ijet->pt() << ", " << ijet->eta() << endl;
// 	hist_jetPt->Fill( ijet->pt() );	  
//       } //end Jet loop   
//     } // end if passes event selection


  } //end event loop
  
  cout << "Printing" << endl;
  wPlusJets->print(std::cout);

  cout << "We're done!" << endl;

  file->Close();
  delete file;
  return 0;
}
