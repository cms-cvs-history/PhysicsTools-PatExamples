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
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

int main(int argc, char* argv[])
{
  // ----------------------------------------------------------------------
  // First Part:
  //
  //  * enable the AutoLibraryLoader
  //  * book the histograms of interest
  //  * open the input file
  // ----------------------------------------------------------------------

  if ( argc < 4 ) return 0;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
 
  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService(argv[2]);
  TFileDirectory theDir = fs.mkdir("analyzeBasicPat");
  TH1F* jetPt_  = theDir.make<TH1F>("jetPt", "pt",    100,  0.,300.);
  TH1F* jetEta_ = theDir.make<TH1F>("jetEta","eta",   100, -3.,  3.);
  TH1F* jetPhi_ = theDir.make<TH1F>("jetPhi","phi",   100, -5.,  5.);
  TH1F* disc_   = theDir.make<TH1F>("disc", "Discriminant", 100, 0.0, 10.0);
  TH1F* constituentPt_ = theDir.make<TH1F>("constituentPt", "Constituent pT", 100, 0, 300.0);
 
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open( argv[1] );

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

    // Handle to the jet collection
    edm::Handle<std::vector<pat::Jet> > jets;
    edm::InputTag jetLabel( argv[3] );
    event.getByLabel(jetLabel, jets);
   
    // loop jet collection and fill histograms
    for(unsigned i=0; i<jets->size(); ++i){
      jetPt_ ->Fill( (*jets)[i].pt()  );
      jetEta_->Fill( (*jets)[i].eta() );
      jetPhi_->Fill( (*jets)[i].phi() );
      reco::SecondaryVertexTagInfo const * svTagInfos = (*jets)[i].tagInfoSecondaryVertex("secondaryVertex");
      if ( svTagInfos != 0 ) {
    if ( svTagInfos->nVertices() > 0 )
      disc_->Fill( svTagInfos->flightDistance(0).value() );
      }
      std::vector<CaloTowerPtr> const & caloConstituents =  (*jets)[i].getCaloConstituents();
      for ( std::vector<CaloTowerPtr>::const_iterator ibegin = caloConstituents.begin(),
          iend = caloConstituents.end(),
          iconstituent = ibegin;
        iconstituent != iend; ++iconstituent ) {
    constituentPt_->Fill( (*iconstituent)->pt() );
      }
    }
  } 
  // close input file
  inFile->Close();

  // ----------------------------------------------------------------------
  // Third Part:
  //
  //  * never forget to free the memory of objects you created
  // ----------------------------------------------------------------------

  // in this example there is nothing to do
 
  // that's it!
  return 0;
}
