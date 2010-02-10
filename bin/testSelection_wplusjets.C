/* 
Example W+Jets selection
*/
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TROOT.h"


#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 

#include "PhysicsTools/PatExamples/interface/WPlusJetsEventSelector.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

using namespace std;

int main ( int argc, char ** argv )
{


   ////////////////////////////////
   // ////////////////////////// //
   // // Command Line Options // //
   // ////////////////////////// //
   ////////////////////////////////

   // Tell people what this analysis code does and setup default options.
   optutl::CommandLineParser parser ("W+Jets Example");

   ////////////////////////////////////////////////
   // Change any defaults or add any new command //
   //      line options you would like here.     //
   ////////////////////////////////////////////////
   parser.stringValue ("outputFile")    = "wjets"; // .root added automatically


   parser.addOption ("muPlusJets",   optutl::CommandLineParser::kBool,
		     "Run mu+Jets",
                     true );
   parser.addOption ("ePlusJets",   optutl::CommandLineParser::kBool,
		     "Run e+Jets",
                     true );
   parser.addOption ("minNJets",   optutl::CommandLineParser::kInteger,
		     "Min number of tight jets",
                     4 );
   parser.addOption ("tightMuMinPt",   optutl::CommandLineParser::kDouble,
		     "Min tight mu pt",
                     20.0 );
   parser.addOption ("tightMuMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max tight mu eta",
                     2.1 );
   parser.addOption ("tightEleMinPt",   optutl::CommandLineParser::kDouble,
		     "Min tight e pt",
		     30.0 );
   parser.addOption ("tightEleMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max tight e eta",
                     2.4 );
   parser.addOption ("looseMuMinPt",   optutl::CommandLineParser::kDouble,
		     "Min loose mu pt",
                     20.0 );
   parser.addOption ("looseMuMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max loose mu eta",
                     2.1 );
   parser.addOption ("looseEleMinPt",   optutl::CommandLineParser::kDouble,
		     "Min loose e pt",
		     30.0 );
   parser.addOption ("looseEleMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max loose e eta",
                     2.4 );
   parser.addOption ("jetMinPt",   optutl::CommandLineParser::kDouble,
		     "Min jet pt",
		     30.0 );
   parser.addOption ("jetMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max jet eta",
                     2.4 );
   

   // Parse the command line arguments
   parser.parseArguments (argc, argv);

   //////////////////////////////////
   // //////////////////////////// //
   // // Create Event Container // //
   // //////////////////////////// //
   //////////////////////////////////

   // This object 'event' is used both to get all information from the
   // event as well as to store histograms, etc.
   fwlite::EventContainer ev (parser);

   ////////////////////////////////////////
   // ////////////////////////////////// //
   // //         Begin Run            // //
   // // (e.g., book histograms, etc) // //
   // ////////////////////////////////// //
   ////////////////////////////////////////

   // Setup a style
   gROOT->SetStyle ("Plain");

  
  cout << "About to allocate functors" << endl;

  // Tight muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdTight     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  muonIdTight->set( "D0", 0.02 );
  // Tight electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  electronIdTight->set( "D0", 0.02 );

  // Tight jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::TIGHT) );

  
  // Loose muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdLoose     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  muonIdLoose->set( "Chi2",    false);
  muonIdLoose->set( "D0",      false);
  muonIdLoose->set( "NHits",   false);
  muonIdLoose->set( "ECalVeto",false);
  muonIdLoose->set( "HCalVeto",false);
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
  WPlusJetsEventSelector wPlusJets(
     edm::InputTag("cleanLayer1Muons"),
     edm::InputTag("cleanLayer1Electrons"),
     edm::InputTag("cleanLayer1Jets"),
     edm::InputTag("layer1METs"),
     edm::InputTag("triggerEvent"),
     muonIdTight,
     electronIdTight,
     jetIdTight,
     muonIdLoose,
     electronIdLoose,
     jetIdLoose,
     parser.integerValue ("minNJets")      ,
     parser.boolValue    ("muPlusJets")    ,
     parser.boolValue    ("ePlusJets")     ,
     parser.doubleValue  ("tightMuMinPt")  ,
     parser.doubleValue  ("tightMuMaxEta") ,
     parser.doubleValue  ("tightEleMinPt") ,
     parser.doubleValue  ("tightEleMaxEta"),
     parser.doubleValue  ("looseMuMinPt")  ,
     parser.doubleValue  ("looseMuMaxEta") ,
     parser.doubleValue  ("looseEleMinPt") ,
     parser.doubleValue  ("looseEleMaxEta"),
     parser.doubleValue  ("jetMinPt")      ,
     parser.doubleValue  ("jetMaxEta")
     );

  wPlusJets.set("Trigger", false);


  ev.add( new TH1F( "jetPt", "Jet p_{T};Jet p_{T} (GeV/c)", 30, 0, 300) );


  //loop through each event
  for( ev.toBegin();
         ! ev.atEnd();
       ++ev) {

    bool passed = wPlusJets(ev);
    // vector<pat::Electron> const & electrons = wPlusJets.selectedElectrons();
    // vector<pat::Muon>     const & muons     = wPlusJets.selectedMuons();
    vector<pat::Jet>      const & jets      = wPlusJets.cleanedJets();

    if ( passed ) {
      for ( vector<pat::Jet>::const_iterator jetsBegin = jets.begin(),
	      jetsEnd = jets.end(), ijet = jetsBegin; 
	    ijet != jetsEnd; ++ijet) {
	//	cout << "Looking at each jet, pt,eta = " << ijet->pt() << ", " << ijet->eta() << endl;
	ev.hist("jetPt")->Fill( ijet->pt() );	  
      } //end Jet loop   
    } // end if passes event selection
  } //end event loop
  
  cout << "Printing" << endl;
  wPlusJets.print(std::cout);

  cout << "We're done!" << endl;

  return 0;
}
