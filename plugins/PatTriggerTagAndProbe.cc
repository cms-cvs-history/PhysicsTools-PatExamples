#include "TMath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "PhysicsTools/PatExamples/plugins/PatTriggerTagAndProbe.h"


PatTriggerTagAndProbe::PatTriggerTagAndProbe( const edm::ParameterSet & iConfig ) :
  // pat::Trigger
  trigger_( iConfig.getParameter< edm::InputTag >( "trigger" ) ),
  // pat::TriggerEvent
  triggerEvent_( iConfig.getParameter< edm::InputTag >( "triggerEvent" ) ),
  // muon input collection
  muons_( iConfig.getParameter< edm::InputTag >( "muons" ) ),
  // muon match objects
  muonMatch_( iConfig.getParameter< std::string >( "muonMatch" ) ),
  // histogram management
  histos1D_()
{
}

PatTriggerTagAndProbe::~PatTriggerTagAndProbe()
{
}

void PatTriggerTagAndProbe::beginJob()
{
  edm::Service< TFileService > fileService;

  // mean pt for all trigger objects
  histos1D_[ "mass"    ] = fileService->make< TH1D >( "mass"  , "Mass_{Z} (GeV)",  45,   30., 120.);
  histos1D_[ "testPt"  ] = fileService->make< TH1D >( "testPt"  , "p_{T} (GeV)" ,  30,    0.,  30.);
  histos1D_[ "probePt" ] = fileService->make< TH1D >( "probePt" , "p_{T} (GeV)" ,  30,    0.,  30.);
  histos1D_[ "testEta" ] = fileService->make< TH1D >( "testEta" , "#eta"        ,  24,  -2.4,  2.4);
  histos1D_[ "probeEta"] = fileService->make< TH1D >( "probeEta", "#eta"        ,  24,  -2.4,  2.4);
}

void PatTriggerTagAndProbe::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup )
{
  // trigger event
  edm::Handle< pat::TriggerEvent > triggerEvent;
  iEvent.getByLabel( triggerEvent_, triggerEvent );
  // trigger paths from patTrigger
  edm::Handle< pat::TriggerPathCollection > triggerPaths;
  iEvent.getByLabel( trigger_, triggerPaths );
  // trigger filters from patTrigger
  edm::Handle< pat::TriggerFilterCollection > triggerFilters;
  iEvent.getByLabel( trigger_, triggerFilters );
  // trigger objects from patTrigger
  edm::Handle< pat::TriggerObjectCollection > triggerObjects;
  iEvent.getByLabel( trigger_, triggerObjects );
  // PAT object collection
  edm::Handle< pat::MuonCollection > muons;
  iEvent.getByLabel( muons_, muons );

  // PAT trigger helper for trigger matching information
  const pat::helper::TriggerMatchHelper matchHelper;

  // recieve the TriggerObjectMatch from the triggerEvent
  const pat::TriggerObjectMatch* triggerMatch( triggerEvent->triggerObjectMatchResult( muonMatch_ ) );
  // loop over muon references for the tag muon
  for( size_t idxTag=0; idxTag<muons->size(); ++idxTag){ 
    const reco::CandidateBaseRef candBaseRefTag( pat::MuonRef( muons, idxTag ) );
    const pat::TriggerObjectRef trigRefTag( matchHelper.triggerMatchObject( candBaseRefTag, triggerMatch, iEvent, *triggerEvent ) );
    if( trigRefTag.isAvailable() ){
      // loop over muon references for the probe/test muon
      for( size_t idxProbe=0; idxProbe<muons->size() && idxProbe!=idxTag; ++idxProbe){ 
	const reco::CandidateBaseRef candBaseRefProbe( pat::MuonRef( muons, idxProbe ) );
	histos1D_[ "mass" ]->Fill( (candBaseRefTag->p4()+candBaseRefProbe->p4()).mass() );
	if(((candBaseRefTag->p4()+candBaseRefProbe->p4()).mass()-90)<5){
	  const pat::TriggerObjectRef trigRefProbe( matchHelper.triggerMatchObject( candBaseRefProbe, triggerMatch, iEvent, *triggerEvent ) );
	  histos1D_[ "probePt"  ]->Fill( candBaseRefProbe->pt () );
	  histos1D_[ "probeEta" ]->Fill( candBaseRefProbe->eta() );
	  if( trigRefProbe.isAvailable() ){

	    std::cout << "TriggerObject ID : ";
	    for(unsigned int idx=0; idx<trigRefProbe->filterIds().size(); ++idx){
	      std::cout << " " << trigRefProbe->filterIds()[idx];
	    }
	    std::cout << std::endl;

	    histos1D_[ "testPt" ]->Fill( candBaseRefProbe->pt () );
	    histos1D_[ "testEta"]->Fill( candBaseRefProbe->eta() );
	  }
	}
      }
    }
  }
}

void PatTriggerTagAndProbe::endJob()
{
  // normalize the entries of the histograms
  histos1D_[ "testPt"  ]->Divide(histos1D_[ "probePt"  ]);
  histos1D_[ "testEta" ]->Divide(histos1D_[ "probeEta" ]);
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE( PatTriggerTagAndProbe );
