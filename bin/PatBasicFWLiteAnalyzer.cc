#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // book a set of histograms
  TH1F* muonPt_  = new TH1F("muonPt", "pt",    100,  0.,300.);
  TH1F* muonEta_ = new TH1F("muonEta","eta",   100, -3.,  3.);
  TH1F* muonPhi_ = new TH1F("muonPhi","phi",   100, -5.,  5.);  
  
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open( "file:PATLayer1_Output.fromAOD_full.root" );

  // ----------------------------------------------------------------------
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  // loop the events
  unsigned int iEvent=0;
  fwlite::Event ev(inFile);
  for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
    edm::EventBase const & event = ev;

    // break loop after end of file is reached 
    // or after 1000 events have been processed
    if( iEvent==1000 ) break;
    
    // simple event counter
    if(iEvent>0 && iEvent%1==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // Handle to the muon collection
    edm::Handle<std::vector<pat::Muon> > muons;
    edm::InputTag muonLabel("cleanPatMuons");
    event.getByLabel(muonLabel, muons);
    
    // loop muon collection and fill histograms
    for(unsigned i=0; i<muons->size(); ++i){
      muonPt_ ->Fill( (*muons)[i].pt()  );
      muonEta_->Fill( (*muons)[i].eta() );
      muonPhi_->Fill( (*muons)[i].phi() );
    }
  }  
  // close input file
  inFile->Close();

  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * open the output file 
  //  * write the histograms to the output file
  //  * close the output file
  // ----------------------------------------------------------------------
  
  //open output file
  TFile outFile( "analyzePatBasics.root", "recreate" );
  outFile.mkdir("analyzeBasicPat");
  outFile.cd("analyzeBasicPat");
  muonPt_ ->Write( );
  muonEta_->Write( );
  muonPhi_->Write( );
  outFile.Close();
  
  // ----------------------------------------------------------------------
  // Fourth Part: 
  //
  //  * never forgett to free the memory of the histograms
  // ----------------------------------------------------------------------

  // free allocated space
  delete muonPt_;
  delete muonEta_;
  delete muonPhi_;
  
  // that's it!
  return 0;
}
