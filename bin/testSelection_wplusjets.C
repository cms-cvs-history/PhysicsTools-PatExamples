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
#include "DataFormats/PatCandidates/interface/Jet.h"
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

  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdTight     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::TIGHT) );


  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdLoose     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdLoose     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdLoose      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::LOOSE) );

  cout << "Making event selector" << endl;
  boost::shared_ptr<WPlusJetsEventSelector> 
    wPlusJets( new WPlusJetsEventSelector( 
     edm::InputTag("cleanLayer1Muons"),
     edm::InputTag("cleanLayer1Electrons"),
     edm::InputTag("cleanLayer1Jets"),
     edm::InputTag("layer1METs"),
     muonIdTight,
     electronIdTight,
     jetIdTight,
     muonIdLoose,
     electronIdLoose,
     jetIdLoose
     ) 
  );
  

  TFile  * file = new TFile("PATLayer1_Output.fromAOD_full.root");
  TH1D * hist_jetPt = new TH1D("hist_jetPt", "Jet p_{T}", 20, 0, 100 );
  fwlite::Event ev(file);

  int count = 0;
  //loop through each event
  for( ev.toBegin();
         ! ev.atEnd();
       ++ev, ++count) {

 
    std::strbitset ret = wPlusJets->getBitTemplate();
    if ( (*wPlusJets)(ev, ret) ) {
      
      vector<pat::Jet> const & jets = wPlusJets->selectedJets();
      for ( vector<pat::Jet>::const_iterator jetsBegin = jets.begin(),
	      jetsEnd = jets.end(), ijet = jetsBegin; 
	    ijet != jetsEnd; ++ijet) {
	//	cout << "Looking at each jet, pt,eta = " << ijet->pt() << ", " << ijet->eta() << endl;
	hist_jetPt->Fill( ijet->pt() );	  
      } //end Jet loop   
    } // end if passes event selection
  } //end event loop
  
  cout << "Printing" << endl;
  wPlusJets->print(std::cout);

  cout << "We're done!" << endl;
  return 0;
}
