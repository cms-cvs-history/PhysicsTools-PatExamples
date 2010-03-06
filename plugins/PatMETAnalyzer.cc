#include <map>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

/**
   \class   PatMETAnalyzer PatMETAnalyzer.h "PhysicsTools/PatAlgos/plugins/PatMETAnalyzer.h"

   \brief   module to analyze different kinds of pat::METs in the context of a more complex exercise (detailed below).

   *Exercise 3:*

   (a)
   Make yourself familiar with the use of the addJetCollection tool and the tools to add additional 
   MET collection the the PAT event content. Find out where to find the corresponding python file in 
   the PatAlgos package where the tools are defined and have a look whether you understand what the 
   tools are doing. Change the standard PAT configuration file to produce a pat tuple, which includes 
   ak5 particle flow jets and ak5 JPT jets, as well as particle flow MET and (track corrected) tcMET 
   in addition to the standard jet and MET collections. Attention: Make sure beforehand which features 
   of the jet collections you need and, which make sense at all (this is NOT taken over by the tool 
   for you).

   (b) 
   With the standard ttbar input sample you used during the morning session of Module 1 extend 
   Exercise 1 to do the following: Compare calorimeter jets (corrected up to L3Absolute) with 
   particle flow jets and JPT jets in 2 dimensional plots of pt, eta and phi of all jets. Compare 
   the resolution plots for calorimeter MET, particle flow MET and (track corrected) tcMET in one 
   histogram.


   *Solution  :*

   (b)
   For the sake of simplicity we do not mixup the jet and the MET part of the exercise, but keep 
   the jet part for the PatJetAnalyzer module. For the MET part of the exercise we implementated 
   a simple EDAnalyzer, which takes the following parameters:
    - mets      : input for the pat MET collection (edm::InputTag).
    - comp      : input for an alternative MET collection for comparisons (edm::InputTag).
*/

class PatMETAnalyzer : public edm::EDAnalyzer {

public:
  /// default contructor
  explicit PatMETAnalyzer(const edm::ParameterSet& cfg);
  /// default destructor
  ~PatMETAnalyzer(){};
  
private:
  /// everything that needs to be done before the event loop
  virtual void beginJob();
  /// everything that needs to be done during the even loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
  /// everything that needs to be done after the event loop
  virtual void endJob();

private:  
  /// simple map to contain all histograms; 
  /// histograms are booked in the beginJob() 
  /// method (for 1-dim histograms)
  std::map<std::string,TH1F*> hist1D_; 
  /// same for (for 2-dim histograms)
  std::map<std::string,TH2F*> hist2D_; 
  /// pat METs
  edm::InputTag mets_;
  /// METs for comparison
  edm::InputTag comp_;
};

#include "DataFormats/PatCandidates/interface/MET.h"

PatMETAnalyzer::PatMETAnalyzer(const edm::ParameterSet& cfg) : hist1D_(), hist2D_(),
  mets_(cfg.getParameter<edm::InputTag>("mets")),
  comp_(cfg.getParameter<edm::InputTag>("comp"))
{
}

void
PatMETAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  edm::Handle<edm::View<pat::MET> > mets;
  event.getByLabel(mets_,mets);

  edm::Handle<edm::View<pat::MET> > comp;
  event.getByLabel(comp_,comp);

  if( mets->begin()!=mets->end() ){
    hist1D_["etMETs"]->Fill(mets->begin()->et());
  }
  if( comp->begin()!=comp->end() ){
    hist1D_["etComp"]->Fill(comp->begin()->et());
  }
  if( mets->begin()!=mets->end() ){
    if( mets->begin()->genMET() ){
      hist2D_["corrMETs"]->Fill( mets->begin()->genMET()->et(), mets->begin()->et() );
    }
    if( comp->begin()->genMET() ){
      hist2D_["corrComp"]->Fill( comp->begin()->genMET()->et(), comp->begin()->et() );
    }
    if( comp->begin()!=comp->end() ){
      hist1D_["res"]->Fill( (mets->begin()->et()-comp->begin()->et())/mets->begin()->et() );
    }
  }
}

void 
PatMETAnalyzer::beginJob()
{
  // register TFileService
  edm::Service<TFileService> fs;

  hist1D_["etMETs"  ]=fs->make<TH1F>("etMETs" , "MET(std)" , 50,  0.,  150.);
  hist1D_["etComp"  ]=fs->make<TH1F>("etComp" , "MET(comp)", 50,  0.,  150.);
  hist1D_["res"     ]=fs->make<TH1F>("res" , "(MET(std)-MET(comp))/MET(std)",   40, -5.,    5.);
  hist2D_["corrMETs"]=fs->make<TH2F>("corrMETs", "MET(std) vs MET(comp)",  50,  0.,  150.,  50,  0.,  150.);
  hist2D_["corrComp"]=fs->make<TH2F>("corrComp", "MET(std) vs MET(comp)",  50,  0.,  150.,  50,  0.,  150.);
}

void 
PatMETAnalyzer::endJob() 
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PatMETAnalyzer);
