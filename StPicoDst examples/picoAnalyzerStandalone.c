/**
 * \brief Example of how to read a file (list of files) using StPicoEvent class in a standalone mode
 *
 * picoAnalyzer.C is an example of reading STAR picoDst format in a standalone mode
 * on your laptop or local computer farm.
 * Prerequisites:
 * - StPicoEvent directory with the library (libStPicoDst.so) compiled
 * - CERN ROOT package
 * - g++ >= 4.8
 * - Makefile
 *
 * First, the program must be compiled with the Makefile, with simple command in the bash shell:
 * make
 *
 * Then the executable file picoAnalyzerStandalone will be created. The current version of the program
 * expects 3 arguments: ./picoAnalyzerStandalone inputFile outputFile
 * The first one is the program name, the second one is the name of the inputfile that
 * maybe either the picoDst file itself, in a format dummyname.picoDst.root or a list of
 * such files called dummyname.list or dummyname.lis. The outputFile assumes the some_output_name.root.
 *
 * \author Grigory Nigmatkulov
 * \date August 6, 2019
 * \email nigmatkulov@gmail.com
 */

// C++ headers
#include <iostream>

// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

// PicoDst headers
#include "StPicoDstReader.h"
#include "StPicoDst.h"
#include "StPicoEvent.h"
#include "StPicoTrack.h"
#include "StPicoBTofPidTraits.h"

//_________________
int main(int argc, char *argv[])
{

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
  R__LOAD_LIBRARY(libStPicoDst);
#else
  gSystem->Load("../libs/libStPicoDst.so");
#endif

  std::cout << "Hi! Lets do some physics, Master!" << std::endl;

  const char *fileName;
  const char *outputFileName;

  switch (argc)
  {
  case 3:
    fileName = argv[1];
    outputFileName = argv[2];
    break;
  default:
    std::cout << "Usage: picoAnalyzerStandalone inputFileName outputFileName.root" << std::endl;
    return -1;
  }

  StPicoDstReader *picoReader = new StPicoDstReader(fileName);
  picoReader->Init();

  // This is a way if you want to spead up I/O
  picoReader->SetStatus("*", 0);
  picoReader->SetStatus("Event*", 1);
  picoReader->SetStatus("Track*", 1);
  picoReader->SetStatus("BTofPidTraits*", 1);

  Long64_t eventsInTree = picoReader->tree()->GetEntries();
  std::cout << "eventsInTree: " << eventsInTree << std::endl;
  Long64_t events2read = picoReader->chain()->GetEntries();

  TFile *outputFile = new TFile(outputFileName, "recreate");

  // Histogramming
  // Event
  TH2F *hist_VtxX_VtxY = new TH2F("hist_VtxX_VtxY",
                                  "Vertex coordinates;X;Y",
                                  200, -6, 6, 200, -6, 6);
  TH1F *hist_VtxZ = new TH1F("hist_VtxZ",
                             "Vertex coordinate;Z;Entries",
                             200, -20, 20);
  TH1F *hist_Eta = new TH1F("hist_Eta",
                            "Pseudorapidity distribution; Eta; Entries",
                            100, -3, 3);
  TH2F *hist_dEdx = new TH2F("hist_dEdx",
                             "Energy loss;p/q GeV/c;dE/dx, KeV/cm",
                             400, -2, 2, 400, 0, 50);
  TH1F *hist_mult = new TH1F("hist_mult",
                             "Multiplisity distribution",
                             80, 0, 80);
  TH2F *hist_reverse_beta = new TH2F("hist_reverse_beta",
                                     "1/beta distribution;p/q, GeV/c;1/beta, 1/c",
                                     400, -3, 3, 400, 0.95, 4);

  // Loop over events
  for (Long64_t iEvent = 0; iEvent < events2read; iEvent++)
  {

    if (iEvent % 100 == 99)
      std::cout << "Working on event #[" << (iEvent + 1)
                << "/" << events2read << "]" << std::endl;

    if (!(picoReader->readPicoEvent(iEvent)))
    {
      std::cout << "Something went wrong, Master! Nothing to analyze..."
                << std::endl;
      break;
    }

    // Retrieve picoDst
    StPicoDst *picoDst = picoReader->picoDst();

    // Retrieve event information
    StPicoEvent *event = picoDst->event();
    if (!event)
    {
      std::cout << "Something went wrong, Master! Event is hiding from me..."
                << std::endl;
      break;
    }

    // Filling multiplisity
    hist_mult->Fill(event->refMult());

    // Filling Vertex X vs Vertex Y hist
    hist_VtxX_VtxY->Fill(event->primaryVertex().X(), event->primaryVertex().Y());

    // Filling Vertex Z hist
    hist_VtxZ->Fill(event->primaryVertex().Z());

    // Track loop
    Int_t nTracks = picoDst->numberOfTracks();
    for (Int_t iTrack = 0; iTrack < nTracks; iTrack++)
    {
      StPicoTrack *picoTrack = picoDst->track(iTrack);
      if (!picoTrack)
        continue;

      // Filling pseudorapidity and dE/dx hists
      if (picoTrack->isPrimary())
      {
        hist_Eta->Fill(picoTrack->pMom().Eta());

        hist_dEdx->Fill(picoTrack->pPtot() / picoTrack->charge(), picoTrack->dEdx());

        // Check if track has TOF signal
        if (picoTrack->isTofTrack())
        {
          // Retrieve corresponding trait
          StPicoBTofPidTraits *trait = picoDst->btofPidTraits(picoTrack->bTofPidTraitsIndex());
          if (!trait)
            continue;

          // Fill 1/beta
          hist_reverse_beta->Fill(picoTrack->pPtot() / picoTrack->charge(), 1. / trait->btofBeta());
        }
      }
    }
  }

  picoReader->Finish();
  outputFile->Write();
  outputFile->Close();

  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
            << std::endl;
}
