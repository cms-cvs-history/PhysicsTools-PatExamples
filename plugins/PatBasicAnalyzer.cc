#include <map>
#include <string>

#include "TH1.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

/**
   \class   PatBasicAnalyzer patBasicAnalyzer.h "PhysicsTools/PatExamples/plugins/PatBasicAnalyzer.h"

   \brief   Plugin to demonstrate how to read pat::Candidate collections from file

   The class demonstrates how to access the most important pat::Candidatre collections 
   from file within the full framework of the edm model. These collections are jets, 
   photons, electrons, muons, taus and MET. For MET the value is plotted. Otherwise
   the multiplicity (for jets the multiplicity above a certain pt cut) is plotted. 

   The module also contains a few commented sections, which may be uncommented during 
   tutorials to demonstrate the following features of PAT: 

    - Section 1: embedding event information for the example of jets. Recommended use 
      together with test/patTuple_embedding_cfg.py in the test directory (EXERCISE 1).

    - Section 2: embedding event information for the example of electrons. Recommen-
      ded use together with test/patTuple_embedding_cfg.py in the test directory
      (EXERCISE 2).

    - Section 3: read user defined isolation variables. Recommended use together with
      test/patTuple_contents_cf.py (EXERCISE 5).

    - Section 4: analyze the cleanLayer1Jets and check these for overlapping objects
      (EXERCISE 3 of Module 3 -- Object Cross Cleaning).

   You can find out more about these exercises on the web page for the December PAT
   tutorial indicated below:
 
   http://indico.cern.ch/conferenceOtherViews.py?view=standard&confId=73168
*/


class PatBasicAnalyzer : public edm::EDAnalyzer {

public:
  /// default contructor (reads in the parameters the configuration file)
  explicit PatBasicAnalyzer(const edm::ParameterSet&);
  /// default destructor
  ~PatBasicAnalyzer();
  
private:
  /// everything that needs to be done before the event loop
  virtual void beginJob() ;
  /// everything that needs to be done during the event loop
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  /// everything that needs to be done after the event loop
  virtual void endJob() ;
  
  /// simple map to contain all histograms; 
  /// histograms are booked in the beginJob() 
  /// method
  std::map<std::string,TH1F*> histContainer_; 
  /// plot number of towers per jet (this is an extra plot, when 
  /// demonstrating the effect of embedding event information)
  TH1F* jetTowers_;
  /// Number of hist on the reconstructed track (this is an 
  /// extra plot, when demonstrating the effect of embedding 
  /// event information)
  TH1F* elecNHits_;

  /// input tag for photons  
  edm::InputTag photonSrc_;
  /// input tag for electrons  
  edm::InputTag elecSrc_;
  /// input tag for muons  
  edm::InputTag muonSrc_;
  /// input tag for taus  
  edm::InputTag tauSrc_;
  /// input tag for jets  
  edm::InputTag jetSrc_;
  /// input tag for MET  
  edm::InputTag metSrc_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

/// default contructor (reads in the parameters the configuration file)
PatBasicAnalyzer::PatBasicAnalyzer(const edm::ParameterSet& iConfig):
  histContainer_(),
  photonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc")),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  tauSrc_(iConfig.getUntrackedParameter<edm::InputTag>("tauSrc" )),
  jetSrc_(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc" )),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc" ))
{
}

/// default destructor
PatBasicAnalyzer::~PatBasicAnalyzer()
{
}

/// everything that needs to be done during the event loop
void
PatBasicAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get electron collection
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(elecSrc_,electrons);

  // get muon collection
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_,muons);

  // get tau collection  
  edm::Handle<edm::View<pat::Tau> > taus;
  iEvent.getByLabel(tauSrc_,taus);

  // get jet collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetSrc_,jets);

  // get met collection  
  edm::Handle<edm::View<pat::MET> > mets;
  iEvent.getByLabel(metSrc_,mets);
  
  // get photon collection  
  edm::Handle<edm::View<pat::Photon> > photons;
  iEvent.getByLabel(photonSrc_,photons);
    
  // loop over jets
  size_t nJets=0;
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    if(jet->pt()>50){
      ++nJets;
    }
  }
  histContainer_["jets"]->Fill(nJets);

  // do something similar for the other candidates
  histContainer_["photons"]->Fill(photons->size() );
  histContainer_["elecs" ]->Fill(electrons->size());
  histContainer_["muons"]->Fill(muons->size() );
  histContainer_["taus" ]->Fill(taus->size()  );
  histContainer_["met"  ]->Fill(mets->empty() ? 0 : (*mets)[0].et());

  // -----------------------------------------
  // Section 1: Show the effect of embedding 
  //            event information for jetss
  //
  // (uncomment these lines for fill the 
  // jetTowers_ histogram)
  // -----------------------------------------
  //for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
  //  jetTowers_->Fill(jet->getCaloConstituents().size());
  //}

  // -----------------------------------------
  // Section 2: Show the effect of embedding 
  //            event information for electrons
  //
  // (uncomment these lines for fill the 
  // elecNHit histogram)
  // -----------------------------------------
  //for(edm::View<pat::Electron>::const_iterator elec=electrons->begin(); elec!=electrons->end(); ++elec){
  //  elecNHits_->Fill(elec->gsfTrack()->numberOfLostHits());
  //}

  // -----------------------------------------
  // Section 3: Show how to access user-defined 
  //            isolation variables
  // -----------------------------------------
  //for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
  //  std::cout << "test isolation values: " << std::endl
  //	      << "POG  Track : " << muon->trackIso() << std::endl
  //	      << "user Track : " << muon->userIsolation(pat::TrackIso) << std::endl
  //	      << "user Ecal  : " << muon->userIsolation(pat::EcalIso ) << std::endl
  //	      << "user Hcal  : " << muon->userIsolation(pat::HcalIso ) << std::endl
  //	      << "user User1 : " << muon->userIsolation(pat::User1Iso) << std::endl
  //	      << "user User2 : " << muon->userIsolation(pat::User2Iso) << std::endl;
  //  }

  // -----------------------------------------
  // Section 3: Show how to access user-defined 
  //            isolation variables
  // -----------------------------------------
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    if(!jet->overlapLabels().empty()){
      std::cout << "checking cleanLayer1Jets for overlaps with: " << std::endl;
      for(std::vector<std::string>::const_iterator overlap=jet->overlapLabels().begin(); overlap!=jet->overlapLabels().end(); ++overlap){
	std::cout << *overlap << "  ::  " << jet->overlaps(*overlap).size() << std::endl;
      }
    }
  }
}

/// everything that needs to be done before the event loop
void 
PatBasicAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;
  
  // book histograms:
  histContainer_["photons"]=fs->make<TH1F>("photons", "photon multiplicity",   10, 0,  10);
  histContainer_["elecs"  ]=fs->make<TH1F>("elecs",   "electron multiplicity", 10, 0,  10);
  histContainer_["muons"  ]=fs->make<TH1F>("muons",   "muon multiplicity",     10, 0,  10);
  histContainer_["taus"   ]=fs->make<TH1F>("taus",    "tau multiplicity",      10, 0,  10);
  histContainer_["jets"   ]=fs->make<TH1F>("jets",    "jet multiplicity",      10, 0,  10);
  histContainer_["met"    ]=fs->make<TH1F>("met",     "missing E_{T}",         20, 0, 100);

  // these are booked for special demonstration reasons during the tutorial
  jetTowers_= fs->make<TH1F>("jetTowers", "towers per jet",   90, 0,  90); 
  elecNHits_= fs->make<TH1F>("elecNHits", "hits per track",   20, 0,  20); 
}

/// everything that needs to be done after the event loop
void 
PatBasicAnalyzer::endJob() 
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PatBasicAnalyzer);
