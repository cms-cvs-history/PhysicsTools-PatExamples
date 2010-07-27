#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"

#include "DataFormats/FWLite/interface/LuminosityBlock.h"
#include "DataFormats/FWLite/interface/Run.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "TSystem.h"
#include "TFile.h"


int main(int argc, char ** argv) {



  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

   TFile file("reco_7TeV_380_pat.root");

   fwlite::Event ev(&file);
   fwlite::Handle<LumiSummary> summary;

   std::cout << "----------- Accessing by event ----------------" << std::endl;


   // We can get run and luminosity blocks from events as well as associated products
   // All this works for ChainEvent and MultiChainEvent as well, not just on one  
   for(ev.toBegin(); !ev.atEnd(); ++ev) {
      // We can get the Luminosity block ID from the event
      std::cout << " Luminosity ID " << ev.getLuminosityBlock().id() << std::endl;
      // or the Run ID from the event
      std::cout <<" Run ID " << ev.getRun().id()<< std::endl;
      // We can get the Run ID from the luminosity block we got from the event
      std::cout << "Run via lumi " << ev.getLuminosityBlock().getRun().id() << std::endl;
      // We can get the integrated luminosity (or any luminosity product) from the event
      summary.getByLabel(ev.getLuminosityBlock(),"lumiProducer");
   }


   std::cout << "----------- Accessing by lumi block ----------------" << std::endl;

   double lumi_tot = 0.0;
   // We can also loop over luminosity blocks themselves, same as events
   fwlite::LuminosityBlock ls(&file);
   for(ls.toBegin(); !ls.atEnd(); ++ls) {
      summary.getByLabel(ls,"lumiProducer");
      std::cout  << ls.id() << " Inst.  Luminosity = " << summary->avgInsRecLumi() << std::endl;
      // And get the associated run from this lumi
      std::cout << "Run from lumi " << ls.getRun().id() << std::endl;
      lumi_tot += summary->avgInsRecLumi();
   }

   std::cout << "----------------------------------------------------" << std::endl;
   std::cout << "Total luminosity from lumi sections = " << lumi_tot << std::endl;
   std::cout << "----------------------------------------------------" << std::endl;

   std::cout << "----------- Accessing by run ----------------" << std::endl;

   // And of course do the same for runs.
   fwlite::Run r(&file);
   for(r.toBegin(); !r.atEnd(); ++r) {
      std::cout << "Run " << r.id() <<std::endl;
   }

   return 0;
}
