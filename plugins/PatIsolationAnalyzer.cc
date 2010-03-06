#include <map>
#include <string>

#include "TH1.h"
#include "TH2.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

/**
   \class   PatIsolationAnalyzer PatIsolationAnalyzer.h "PhysicsTools/PatAlgos/plugins/PatIsolationAnalyzer.h"

   \brief   module to analyze different kinds of pat::METs in the context of a more complex exercise (detailed below).

   *Exercise 2:*

   (a)
   Make yourself familiar with the use of the addMuonUserIsolation tool from PAT. Find out where to find the 
   corresponding python file in the PatAlgos package where the tool is defined and have a look whether you 
   understand what the tool is doing. Change the standard PAT configuration file to produce a pat tuple, which 
   includes Tracker, Ecal and Hcal information for a user-defined isolation of muons and the corresponding 
   isoDeposits. 

   (b) 
   Make sure you understand how this user-defined isolation is accessed from the pat::Muon in contrast to the 
   standard isolation types as returned by the PAT member functions trackIso(), ecalIso() and hcalIso(). The 
   technical difference is that the standard isolation from PAT referes to information that is already stored 
   in the reco::Candidate and to the official recommendation by the corresponding POG. 

   (c)
   With the standard ttbar input sample you used during the morning session of Module 1 compare the following 
   different kinds of user-defined isolation for muons in 2-dimensional correlation plots: 
   - standard tracker isolation from PAT versus user-defined tracker isolation.
   - standard ECAL isolation from PAT versus user-defined ECAL isolation.
   - standard HCAL isolation from PAT versus user-defined HCAL isolation.
   - standard tracker isolation from PAT versus user-defined ECAL isolation.
   - standard tracker isolation from PAT versus user-defined HCAL isolation.
   (Have a look for the explanation in b to understand what is meant by standard Tracker/Ecal/Hcal isolation 
   from PAT.) Then switch between the different methods how to combine the energy in the vicinity of the muon, 
   when configuring the user-defined isolation, and check, which is the best method to use. For Tracker isolation 
   you should try the following changes: 
   - change deltaR cone from 0.3 to 0.4.
   - change the mode from sum to max.
   - change the mode from sum to sum2.
   Note that you might need to replace the userIsolation PSet by a self-defined PSet. Using the standard tool 
   will not help you here. You will need to do the configration of the userIsolations during the production 
   of your patTuple.  
   
   (d)
   With the standard ttbar input sample you used during the morning session of Module 1 make a plot of the 
   energy/pt flow in the vicinity of the muon from isoDeposits. As an extra you might want to distinguish 
   between different decay channels of the generated ttbar pair, to see the difference between isolated and 
   non-isolated muons in the energy/pt flow. 
   
   (e)
   Extend the user-defined isolation of c by your own type isolation for muons making use of the energy 
   desposits in the HO or the energy deposited in the detector by jets (an unusual way of formulating a 
   minimal distance between the closest jet and the muon) and extend the plots of c correspondingly. 


   *Solution  :*

   (c)
   We choose the muon to check the isolation variables and a simple EDAnalyzer, which only needs one para-
   meter (edm::InputTag muons). User-defined isolations for Tracker/Ecal/Hcal are expected. Other kinds of 
   user-defined isolations are optional. Note that the user-defined isolations must be configured beforehand 
   during the patTuple production steps. You can use a standard tool for the configuration of pre-defined 
   userIsolations for Tracker, Ecal and Hcal. You can also use an own configuration to add even more 
   individually defined isolation variables. This will be necessary for the exercises. Note that you can 
   NOT concatenate these tools, as the edm::ParameterSet for userIsolation must be replaced as a whole. 

   (d)
   We use the isoDeposits to fill the energy/pt flow as a function of the distance R from the muon. The 
   energy is plotted differentially, in rings of deltaR. At the end of the run each of the plots is 
   normalized to the number of muons, thus representing the mean energy per muon in each ring of deltaR 
   around the muon. In an extension you could split the sample for isolated muons from W decays and non 
   isolated muons within jets to see the difference in enegy/pt flow. For the sake of simplicity this 
   step is omitted here.

   (e)
   For this exercise we added a new tool for a completely user-defined userIsolation PSet, which includes 
   the pre-defined userIsolations for Tracker/Ecal/Hcal (with slightly modified values for deltaR, when 
   compared to the POG recommendation), and five additional definitions of isolation variables. 
*/

class PatIsolationAnalyzer : public edm::EDAnalyzer {

public:
  /// default contructor
  explicit PatIsolationAnalyzer(const edm::ParameterSet& cfg);
  /// default destructor
  ~PatIsolationAnalyzer(){};
  
private:
  /// everything that needs to be done before the event loop
  virtual void beginJob();
  /// everything that needs to be done during the even loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
  /// everything that needs to be done after the event loop
  virtual void endJob();
  /// fill histogram with userIsolation or give LogInfo in
  /// case it was not filled during production
  void fill(const char* hist, double referenceValue, double comparisonValue);
  /// fill the energy flow histogram from a given deposit
  void energyFlow(TH1* hist, const pat::IsoDeposit* deposit);

private:  
  /// simple map to contain all histograms; 
  /// histograms are booked in the beginJob() 
  /// method (for 1-dim histograms)
  std::map<std::string,TH1F*> hist1D_; 
  /// same for (for 2-dim histograms)
  std::map<std::string,TH2F*> hist2D_; 
  /// pat muon
  edm::InputTag muons_;
};

#include "FWCore/MessageLogger/interface/MessageLogger.h"

PatIsolationAnalyzer::PatIsolationAnalyzer(const edm::ParameterSet& cfg) : hist1D_(), hist2D_(),
  muons_(cfg.getParameter<edm::InputTag>("muons"))
{
}

void
PatIsolationAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  edm::Handle<edm::View<pat::Muon> > muons;
  event.getByLabel(muons_,muons);

  // fill correlation histograms; if userIsolation(...) was not filled during the 
  // patTuple production it will return -1.0; this is caught in the fill function 
  // and a corresponding LogInfo is issued.
  for(edm::View<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon){
    fill("corrTrackEcal", muon->trackIso(), muon->ecalIso());
    fill("corrEcalHcal" , muon->ecalIso (), muon->hcalIso());
    fill("corrTrackUser", muon->trackIso(), muon->userIsolation(pat::TrackIso));
    fill("corrEcalUser" , muon->ecalIso (), muon->userIsolation(pat::EcalIso ));
    fill("corrHcalUser" , muon->hcalIso (), muon->userIsolation(pat::HcalIso ));

    // we need this histogram for later normalization of the eflow histograms to 
    // entries per muon.
    hist1D_["norm"]->Fill(0.5);
    
    // fill energy flow plots for each muon; these need to be normalized to entries
    // per muon later!
    energyFlow(hist1D_["eflowTrack"], muon->trackIsoDeposit());
    energyFlow(hist1D_["eflowEcal" ], muon->ecalIsoDeposit ());
    energyFlow(hist1D_["eflowHcal" ], muon->hcalIsoDeposit ());

    // fill a complete user-defined isolation (if it exists)
    if(muon->userIsolation(pat::User1Iso)!=-1.0){ fill("corrCaloUser1" , muon->trackIso(),      muon->userIsolation(pat::User1Iso) ); }
    if(muon->userIsolation(pat::User2Iso)!=-1.0){ fill("corrCaloUser2" , muon->trackIso(),      muon->userIsolation(pat::User2Iso) ); }
    if(muon->userIsolation(pat::User3Iso)!=-1.0){ fill("corrCaloUser3" , muon->trackIso(), sqrt(muon->userIsolation(pat::User3Iso))); }
    if(muon->userIsolation(pat::User4Iso)!=-1.0){ hist1D_["user4Iso"]->Fill(muon->userIsolation(pat::User4Iso) ); }
    if(muon->userIsolation(pat::User5Iso)!=-1.0){ hist1D_["user5Iso"]->Fill(muon->userIsolation(pat::User5Iso) ); }
  }
}

void 
PatIsolationAnalyzer::beginJob()
{
  // register TFileService
  edm::Service<TFileService> fs;

  // book 1-dim histograms 
  hist1D_["norm"         ]=fs->make<TH1F>("norm"         , "norm"                   ,   1,  0.,  1.);
  hist1D_["eflowTrack"   ]=fs->make<TH1F>("eflowTrack"   , "Energy Flow (Tracker)"  ,  20,  0.,  1.);
  hist1D_["eflowEcal"    ]=fs->make<TH1F>("eflowEcal"    , "Energy Flow (ECAL)"     ,  20,  0.,  1.);
  hist1D_["eflowHcal"    ]=fs->make<TH1F>("eflowHcal"    , "Energy Flow (HCAL)"     ,  20,  0.,  1.);
  hist1D_["user4Iso"     ]=fs->make<TH1F>("user4Iso"     , "Isolation (User 4)"     ,  20,  0., 20.);
  hist1D_["user5Iso"     ]=fs->make<TH1F>("user5Iso"     , "Isolation (User 5)"     ,  25,  0., 50.);

  // book 2-dim histograms 
  hist2D_["corrTrackEcal"]=fs->make<TH2F>("corrTrackEcal", "Isolation track vs ecal",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrEcalHcal" ]=fs->make<TH2F>("corrEcalHcal" , "Isolation ecal  vs hcal",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrTrackUser"]=fs->make<TH2F>("corrTrackUser", "Isolation track vs user",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrEcalUser" ]=fs->make<TH2F>("corrEcalUser" , "Isolation ecal  vs user",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrHcalUser" ]=fs->make<TH2F>("corrHcalUser" , "Isolation hcal  vs user",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrCaloUser1"]=fs->make<TH2F>("corrCaloUser1", "Isolation calo vs user1",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrCaloUser2"]=fs->make<TH2F>("corrCaloUser2", "Isolation calo vs user2",  50,  0.,  5.,  50,  0.,  5.);
  hist2D_["corrCaloUser3"]=fs->make<TH2F>("corrCaloUser3", "Isolation calo vs user3",  50,  0.,  5.,  50,  0.,  5.);
}

void
PatIsolationAnalyzer::endJob(){
  hist1D_["eflowTrack"]->Scale(1./hist1D_["norm"]->Integral());  
  hist1D_["eflowEcal" ]->Scale(1./hist1D_["norm"]->Integral());  
  hist1D_["eflowHcal" ]->Scale(1./hist1D_["norm"]->Integral());  
}

void
PatIsolationAnalyzer::fill(const char* hist, double referenceValue, double comparisonValue)
{
  if(referenceValue==-1.0 || comparisonValue==-1.0){
    edm::LogInfo("IsolationConfig") << "userIsolation was not filled for chosen key(s) during patTuple production.\n";
  }
  else{
    hist2D_[hist]->Fill( referenceValue, comparisonValue);
  }
}

void
PatIsolationAnalyzer::energyFlow(TH1* hist, const pat::IsoDeposit* deposit)
{
  if(!deposit){
    edm::LogInfo("IsolationConfig") << "trackIsoDeposit was not filled durint patTuple production.\n";
  }
  if(hist && deposit){
    for(int bin=1; bin<=hist->GetNbinsX(); ++bin){
      double lowerEdge = hist->GetBinLowEdge(bin);
      double upperEdge = hist->GetBinLowEdge(bin)+hist->GetBinWidth(bin);
      // restrict the differential energy deposit to less than 10GeV
      // in order not to have too high weight in the plot
      if(deposit->depositWithin(upperEdge) - deposit->depositWithin(lowerEdge) < 10)
	hist->Fill(hist->GetBinCenter(bin), deposit->depositWithin(upperEdge) - deposit->depositWithin(lowerEdge));
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PatIsolationAnalyzer);
