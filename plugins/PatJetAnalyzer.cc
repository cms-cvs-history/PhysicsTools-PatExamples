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

/// maximal number of bins used for the jet
/// response plots
static const unsigned int MAXBIN=8;
/// binning used fro the jet response plots 
/// (NOTE BINS must have a length of MAXBIN
/// +1)
static const float BINS[]={0., 10., 20., 40., 60., 80., 100., 125., 150.};

/**
   \class   PatJetAnalyzer PatJetAnalyzer.h "PhysicsTools/PatAlgos/plugins/PatJetAnalyzer.h"

   \brief   module to analyze pat::Jets in the contect of a more complex exercise (detailed below).

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

   there is nothing to be done here. due to the abstract structure of the analyzer. Everything can 
   be done on the configuration file level. For the parts of the Exercise that concern MET we create
   a new very simple EDAnalyzer. So we keep the transparency of the modules and do not mix up 
   different tasks on different collections. 
*/

class PatJetAnalyzer : public edm::EDAnalyzer {

public:
  /// default contructor
  explicit PatJetAnalyzer(const edm::ParameterSet& cfg);
  /// default destructor
  ~PatJetAnalyzer(){};
  
private:
  /// everything that needs to be done before the event loop
  virtual void beginJob();
  /// everything that needs to be done during the even loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
  /// everything that needs to be done after the event loop
  virtual void endJob();
  /// deduce correction level for pat::Jet; label is 
  /// expected to be of type 'corrLevel:flavorType'
  std::string corrLevel() { return corrLevel_.substr(0, corrLevel_.find(':')); };  
  /// deduce potential flavor type for pat::Jet; label
  /// is expected to be of type 'corrLevel:flavorType' 
  std::string corrFlavor() { return corrLevel_.substr(corrLevel_.find(':')+1); }; 

private:  
  /// simple map to contain all histograms; 
  /// histograms are booked in the beginJob() 
  /// method (for 1-dim histograms)
  std::map<std::string,TH1F*> hist1D_; 
  /// same for (for 2-dim histograms)
  std::map<std::string,TH2F*> hist2D_; 
  /// correction level for pat jet
  std::string corrLevel_;
  /// pat jets
  edm::InputTag jets_;
  /// jets for comparison
  edm::InputTag comp_;
};

#include "DataFormats/PatCandidates/interface/Jet.h"

PatJetAnalyzer::PatJetAnalyzer(const edm::ParameterSet& cfg) : hist1D_(), hist2D_(),
  corrLevel_(cfg.getParameter<std::string>("corrLevel")),
  jets_(cfg.getParameter<edm::InputTag>("jets"))
{
  if(cfg.existsAs<edm::InputTag>("comp")){
    // can be omitted in the cfi file
    comp_=cfg.getParameter<edm::InputTag>("comp");
  }
}

void
PatJetAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  edm::Handle<edm::View<pat::Jet> > jets;
  event.getByLabel(jets_,jets);

  unsigned int mult =0;
  for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
    hist1D_["pt"]->Fill(jet->correctedJet(corrLevel(), corrFlavor()).pt());
    if(jet->correctedJet(corrLevel(), corrFlavor()).pt()>20){ ++mult; }

    if( jet->genParton() && abs(jet->genParton()->pdgId())<6 ){
      double resp=( jet->pt()-jet->genParton()->pt() )/jet->genParton()->pt();
      for(unsigned int idx=0; idx<MAXBIN; ++idx){
	if(BINS[idx]<=jet->genParton()->pt() && jet->genParton()->pt()<BINS[idx+1]){
	  char buffer [10]; sprintf (buffer, "resp_%i", idx);
	  hist1D_[buffer]->Fill( resp );
	}
      }
    }
  }
  hist1D_["mult"]->Fill(mult);

  if(!comp_.label().empty()){
    edm::Handle<edm::View<reco::Jet> > comp;
    event.getByLabel(comp_,comp);
    
    mult=0;
    unsigned int idx=0;
    for(edm::View<reco::Jet>::const_iterator jet=comp->begin(); jet!=comp->end(); ++jet, ++idx){
      hist1D_["ptComp"]->Fill(jet->pt());
      if(jet->pt()>20){ ++mult; }

      if(idx<jets->size()){
	hist2D_["ptCorr" ]->Fill((*jets)[idx].pt (), (*comp)[idx].pt ());
	hist2D_["etaCorr"]->Fill((*jets)[idx].eta(), (*comp)[idx].eta());
	hist2D_["phiCorr"]->Fill((*jets)[idx].phi(), (*comp)[idx].phi());
      }
    }
    hist1D_["multComp"]->Fill(mult);
  }
}

void 
PatJetAnalyzer::beginJob()
{
  // register TFileService
  edm::Service<TFileService> fs;

  for(unsigned int idx=0; idx<MAXBIN; ++idx){
    char buffer [10]; sprintf (buffer, "resp_%i", idx);
    hist1D_[buffer]=fs->make<TH1F>(buffer, "(pt_{rec}-pt_{gen})/pt_{rec}",  80, 10., 10.);
  }  
  hist1D_["mult" ]   =fs->make<TH1F>("mult"    , "N_{>20}(jet)" ,   10, 0.,  10.);
  hist1D_["pt"   ]   =fs->make<TH1F>("pt"      , "pt_{all}(jet)",  150, 0., 300.);
  if(comp_.label().empty()) return;

  hist1D_["multComp"]=fs->make<TH1F>("multComp", "N_{>20}(jet)" ,   10, 0.,  10.);
  hist1D_["ptComp"  ]=fs->make<TH1F>("ptComp"  , "pt_{all}(jet)",  150, 0., 300.);

  hist2D_["ptCorr"  ]=fs->make<TH2F>("ptCorr"  , "pt_{all}" ,  150,     0.,   300.,  150,     0.,   300.);
  hist2D_["etaCorr" ]=fs->make<TH2F>("etaCorr" , "eta_{all}",   50,     0.,     5.,   50,     0.,     5.);
  hist2D_["phiCorr" ]=fs->make<TH2F>("phiCorr" , "phi_{all}",   60,  -3.14,   3.14,   60,  -3.14,   3.14);
}

void 
PatJetAnalyzer::endJob() 
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PatJetAnalyzer);
