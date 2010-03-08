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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"

class WSelector : public EventSelector {
public:
  WSelector( edm::ParameterSet const & params ) :
    muonSrc_(params.getParameter<edm::InputTag>("muonSrc")),
    metSrc_ (params.getParameter<edm::InputTag>("metSrc")) 
  {
    double muonPtMin = params.getParameter<double>("muonPtMin");
    double metMin = params.getParameter<double>("metMin");
    push_back("Muon Pt", muonPtMin );
    push_back("MET", metMin );
    set("Muon Pt");
    set("MET");

    wMuon_ = 0;
    met_ = 0;
  }
  virtual ~WSelector() {}


  // Here is where the selection occurs
  virtual bool operator()( edm::EventBase const & event, std::strbitset & ret){

    ret.set(false);

    // Handle to the muon collection
    edm::Handle<std::vector<pat::Muon> > muons;    
    // Handle to the MET collection
    edm::Handle<std::vector<pat::MET> > met;
    
    // Get the objects from the event
    bool gotMuons = event.getByLabel(muonSrc_, muons);
    bool gotMET = event.getByLabel(metSrc_, met);

    // get the MET, require to be > minimum
    if ( gotMET ) {
      met_ = &met->at(0);
      if ( met_->pt() > cut("MET",   double()) || ignoreCut("MET") ) 
	passCut(ret, "MET");
    }

    // get the highest pt muon, require to have pt > minimum
    if ( gotMuons ) {
      if ( !ignoreCut("Muon Pt") )  {
	if ( muons->size() > 0 ) {
	  wMuon_ = &muons->at(0);
	  if ( wMuon_->pt() > cut("Muon Pt", double()) || ignoreCut("Muon Pt") ) 
	    passCut(ret, "Muon Pt");
	}
      } else {
	passCut( ret, "Muon Pt");
      }
    }

    setIgnored(ret);
    return (bool)ret;
    
  }

  pat::Muon const & wMuon() const { return *wMuon_;}
  pat::MET  const & met()   const { return *met_;}

protected:
  edm::InputTag muonSrc_;
  edm::InputTag metSrc_;

  pat::Muon const * wMuon_;
  pat::MET const *  met_;
};

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

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // Get the python configuration
  PythonProcessDesc builder(argv[1]);
  edm::ParameterSet const& fwliteParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("FWLiteParams");
  
  // Now get the W selector
  edm::ParameterSet wSelectorParams = fwliteParameters.getParameter<edm::ParameterSet>("wSelectorParams");
  WSelector wSelector( wSelectorParams );
  std::strbitset wSelectorReturns = wSelector.getBitTemplate();
  

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService("analyzePatBasics.root");
  TFileDirectory theDir = fs.mkdir("analyzeBasicPat");
  TH1F* muonPt_  = theDir.make<TH1F>("muonPt", "pt",    100,  0.,300.);
  TH1F* muonEta_ = theDir.make<TH1F>("muonEta","eta",   100, -3.,  3.);
  TH1F* muonPhi_ = theDir.make<TH1F>("muonPhi","phi",   100, -5.,  5.);  
  
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

    
    if ( wSelector(event, wSelectorReturns ) ) {
      

      pat::Muon const & wMuon = wSelector.wMuon();

      muonPt_ ->Fill( wMuon.pt()  );
      muonEta_->Fill( wMuon.eta() );
      muonPhi_->Fill( wMuon.phi() );
      
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
