#include <map>
#include <string>

#include "TH1D.h"
#include "TH2D.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"


class PatTriggerAnalyzer : public edm::EDAnalyzer {

 public:
  /// default constructor
  explicit PatTriggerAnalyzer( const edm::ParameterSet & iConfig );
  /// default destructor
  ~PatTriggerAnalyzer();

 private:
  /// everythin that needs to be done before the event loop
  virtual void beginJob();
  /// everythin that needs to be done during the event loop
  virtual void analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup );
  /// everythin that needs to be done after the event loop
  virtual void endJob();

  /// input for patTrigger
  edm::InputTag trigger_;
  /// input for patTriggerEvent
  edm::InputTag triggerEvent_;
  /// input for muons
  edm::InputTag muons_;
  /// binning for turn-on curve
  unsigned nBins_;
  double   binWidth_;
  /// minimal id for meanPt plot
  unsigned minID_;
  /// maximal id for meanPt plot
  unsigned maxID_;

  /// histogram management
  std::map< std::string, TH1D* > histos1D_;
  std::map< std::string, TH2D* > histos2D_;

  /// internals for meanPt histogram calculation
  std::map< unsigned, unsigned > sumN_;
  std::map< unsigned, double >   sumPt_;
};


#include "TMath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


using namespace pat;


PatTriggerAnalyzer::PatTriggerAnalyzer( const edm::ParameterSet & iConfig ) :
  // pat::Trigger
  trigger_( iConfig.getParameter< edm::InputTag >( "trigger" ) ),
  // pat::TriggerEvent
  triggerEvent_( iConfig.getParameter< edm::InputTag >( "triggerEvent" ) ),
  // muon input collection
  muons_( iConfig.getParameter< edm::InputTag >( "muons" ) ),
  // binning for turn-on curve
  nBins_( iConfig.getParameter< unsigned >( "nBins" ) ),
  binWidth_( iConfig.getParameter< double >( "binWidth" ) ),
  // minimal id for of all trigger objects
  minID_( iConfig.getParameter< unsigned >( "minID" ) ),
  // maximal id for of all trigger objects
  maxID_( iConfig.getParameter< unsigned >( "maxID" ) ),
  histos1D_(), histos2D_()
{
}

PatTriggerAnalyzer::~PatTriggerAnalyzer()
{
}

void PatTriggerAnalyzer::beginJob()
{
  edm::Service< TFileService > fileService;

  // pt correlation plot
  histos2D_[ "ptTrigCand" ] = fileService->make< TH2D >( "ptTrigCand", "Object vs. candidate p_{T} (GeV)", 60, 0., 300., 60, 0., 300. );
  histos2D_[ "ptTrigCand" ]->SetXTitle( "candidate p_{T} (GeV)" );
  histos2D_[ "ptTrigCand" ]->SetYTitle( "object p_{T} (GeV)" );
  // eta correlation plot
  histos2D_[ "etaTrigCand" ] = fileService->make< TH2D >( "etaTrigCand", "Object vs. candidate #eta", 50, -2.5, 2.5, 50, -2.5, 2.5 );
  histos2D_[ "etaTrigCand" ]->SetXTitle( "candidate #eta" );
  histos2D_[ "etaTrigCand" ]->SetYTitle( "object #eta" );
  // phi correlation plot
  histos2D_[ "phiTrigCand" ] = fileService->make< TH2D >( "phiTrigCand", "Object vs. candidate #phi", 60, -TMath::Pi(), TMath::Pi(), 60, -TMath::Pi(), TMath::Pi() );
  histos2D_[ "phiTrigCand" ]->SetXTitle( "candidate #phi" );
  histos2D_[ "phiTrigCand" ]->SetYTitle( "object #phi" );
  // candidate counter for turn-on curve
  histos1D_[ "countCand" ] = fileService->make< TH1D >( "countCand", "p_{T} (GeV) of candidate", nBins_, 20., 20. + nBins_ * binWidth_ );
  histos1D_[ "countCand" ]->SetXTitle( "candidate p_{T} (GeV)" );
  histos1D_[ "countCand" ]->SetYTitle( "# of candidates" );
  // turn-on curve
  histos1D_[ "turnOn" ] = fileService->make< TH1D >( "turnOn", "p_{T} (GeV) of candidate", nBins_, 20., 20. + nBins_ * binWidth_ );
  histos1D_[ "turnOn" ]->SetXTitle( "candidate p_{T} (GeV)" );
  histos1D_[ "turnOn" ]->SetYTitle( "efficiency" );
  // mean pt for all trigger objects
  histos1D_[ "ptMean" ] = fileService->make< TH1D >( "ptMean", "Mean p_{T} (GeV) per trigger object type", maxID_ - minID_ + 1, minID_ - 0.5, maxID_ + 0.5);
  histos1D_[ "ptMean" ]->SetXTitle( "trigger object type" );
  histos1D_[ "ptMean" ]->SetYTitle( "mean p_{T} (GeV)" );

  // initialize counters for mean pt calculation
  for( unsigned id = minID_; id <= maxID_; ++id ) {
    sumN_ [ id ] = 0;
    sumPt_[ id ] = 0.;
  }
}

void PatTriggerAnalyzer::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup )
{
  // PAT trigger event
  edm::Handle< TriggerEvent > triggerEvent;
  iEvent.getByLabel( triggerEvent_, triggerEvent );

  // PAT object collection
  edm::Handle< MuonCollection > muons;
  iEvent.getByLabel( muons_, muons );

  /*
    kinematics comparison
  */

  // loop over muon references (PAT muons have been used in the matcher in task 3)
  for( size_t iMuon = 0; iMuon < muons->size(); ++iMuon ) {
  }

  /*
    turn-on curve
  */

  // loop over muon references again
  for( size_t iMuon = 0; iMuon < muons->size(); ++iMuon ) {
    // fill the counting histogram...
    histos1D_[ "countCand" ]->Fill( muons->at( iMuon ).pt() );
  }

  // get the trigger objects corresponding to the used matching (HLT muons)
  const TriggerObjectRefVector trigRefs( triggerEvent->objects( trigger::TriggerMuon ) );
  // loop over selected trigger objects
  for ( TriggerObjectRefVector::const_iterator iTrig = trigRefs.begin(); iTrig != trigRefs.end(); ++iTrig ) {
  }

  /*
    mean pt
  */

  // loop over all trigger match objects from minID to maxID; have
  // a look to DataFormats/HLTReco/interface/TriggerTypeDefs.h to
  // know more about the available trrigger object id's
  for ( unsigned id=minID_; id<=maxID_; ++id ) {
  }
}

void PatTriggerAnalyzer::endJob()
{
  /*
    turn-on curve
  */

  // normalise bins for turn-on based with counter

  /*
    mean pt
  */

  // normalize the entries for the mean pt plot
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( PatTriggerAnalyzer );
