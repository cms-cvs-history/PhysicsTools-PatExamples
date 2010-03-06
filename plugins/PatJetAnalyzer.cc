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

   *Exercise 1:*

   (a)
   Make yourself familiar with the jetCorrFactors module of PAT. Inspect it using one of 
   the configuration files you used during the morning session of Module 1 and python in 
   interactive mode (NB: python -i myfile_cfg.py). Find out where the corresponding cfi 
   file is located within the PatAlgos package and make sure you understand the use and 
   meaning of the parameters of the module.

   (b)
   Make sure you understand how to retrieve jets with transvers momentum (pt) at different 
   correction levels of the jet energy scale (JES) from a pat::Jet. 

   (c)
   With the standard ttbar input sample you used during the morning session, compare the pt 
   of all pat::Jets with the pt of all reco::Jets at the following correction levels of the 
   jet energy scale (JES): Raw, L2Relative, L3Absolute. Use the most actual corrections 
   for 7 TeV data. You may use the PatBasicExample to start from.

   (d)
   With the standard ttbar input sample you used during the morning session make a plot of 
   the jet pt response at the following correction levels of the JES: Raw, L2Relative, 
   L3Absolute, L5Flavor, L7Parton. Use the most actual corrections for 7 TeV data. Choose 
   the L5Flavor and L7Parton corrections to be determined from a ttbar sample instead of 
   a QCD dijet sample (which is the default configuration in the standard workflow of PAT). 
   For the response compare the pat::Jet to the matched generator jet or to a matched parton 
   of status 3. You may use the PatBasicExample to start from.

   As an extension you may distinguish between b-jets and light quark jets (based on the 
   pdgId of the matched status 3 parton) when plotting the jet response.


   *Solution  :*

   (c)
   We choose a simple implementation of an EDAnalyzer, which takes the following parameters: 
    - jets      : input for the pat jet collection (edm::InputTag).
    - comp      : input for an alternative jet collection for comparisons, which can be of 
                  type reco::Jet or pat::Jet (edm::InputTag).
    - corrLevel : string to determine the pat jet correction level.
   The corrLevel string is expected to be of the form corrType:flavorType. The parameter 
   comp is optional; it can be omitted in the configuration file if not needed. (This is 
   reflected by two definitions of the module in the corresponding cfi file.) We neglect a 
   complex parsing to check for allowed concrete substrings for the correction level or 
   correction flavor for the sake of simplicity; the user should take care to give a proper 
   input here. Illegal strings will lead to an edm::Exception of the jet correction service.    
   In a corresponding cff file this module will be cloned for each correction level required
   for the exercise. Note that you might have to check the input parameters the corrections 
   of the JES and for the matching to partons and change them in the configuration file for
   the production of your patTuple is required.

   (d)
   For the sake of simplicity we restrict ourselves to the example of partons. The partons 
   are restricted to quarks (with masses below the top quark) only. A variable binning to 
   fill the basic histograms and the number of bins are defined as static const's outside 
   the class definition. Both, parton and generator jet matching are already provided to 
   best CMS knowledge by the configuration of the pat::Jet (check the configuration of the 
   patJetPartonMatch and the patJetGenJetMatch module and the WorkBookMCTruthMatch TWiki 
   page for more details). We clone and re-use the module for each correction level required
   for the exercise.
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
