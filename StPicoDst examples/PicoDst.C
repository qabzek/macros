#define _VANILLA_ROOT_
//#define _USE_MATH_DEFINE

// C++ headers

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TMath.h"

// C++ headers
// PicoDst headers
#include "StPicoDstReader.h"
#include "StPicoDst.h"
#include "StPicoEvent.h"
#include "StPicoTrack.h"
#include "StPicoBTofHit.h"
#include "StPicoBTowHit.h"
#include "StPicoEmcTrigger.h"
#include "StPicoBTofPidTraits.h"
#include "StPicoTrackCovMatrix.h"

const Char_t *defaultOutFile = "27GeV.root";

using namespace std;

int PicoDst(const Char_t *inFile = "st_physics_19131004_raw_3000006.picoDst.root", const Char_t *outFileName = "outTest.root")
{
    StPicoDstReader *picoReader = new StPicoDstReader(inFile);
    picoReader->Init();
    TFile ofile(outFileName, "RECREATE");

    Double_t Vz;
    TH1D *hNoCutVertexZ;
    TH1D *hNoCutNFitPoints;

    hNoCutNFitPoints = new TH1D("hNoCutFitPoints", "Number of fit points destribution, no cuts", 50, 0., 50.);
    hNoCutVertexZ = new TH1D("hNoCutVertexZ", "Vertex Z destribution, no cuts", 100, -100., 100.);

    picoReader->SetStatus("*", 0);
    picoReader->SetStatus("Event*", 1);
    picoReader->SetStatus("Track*", 1);

    Long64_t events2read = picoReader->chain()->GetEntries();
    // if (events2read > 2000) events2read = 2000;
//-------------------------Event loop----------------------------------------------------------------
    for (Long64_t iEvent = 0; iEvent < events2read; iEvent++) 
    {
        cout << "Working on event #[" << (iEvent + 1) << "/" << events2read << "\r";
        Bool_t readEvent = picoReader->readPicoEvent(iEvent);
        StPicoDst *dst = picoReader->picoDst();
        StPicoEvent *event = dst->event();

        Vz = event->primaryVertex().Z();
        hNoCutVertexZ->Fill(event->primaryVertex().Z());
//-------------------------Track loop----------------------------------------------------------------
        for (Int_t iTrk = 0; iTrk < dst->numberOfTracks(); iTrk++)
        {

            StPicoTrack *picoTrack = dst->track(iTrk);
            if (!picoTrack)
                continue;
            if (!picoTrack->isPrimary())
                continue;

            hNoCutNFitPoints->Fill(picoTrack->nHitsFit());
        } // end iTrack
    }     // end iEvent

    hNoCutVertexZ->Write();
    hNoCutNFitPoints->Write();

    ofile.Close();
    // End of the program -----------------------------------------------------------------------------------------
    picoReader->Clear();
    picoReader->Finish();
    return 0;
}

int main() {

  R__LOAD_LIBRARY(libStPicoDst);
  R__LOAD_LIBRARY(libStRefMultCorr);
  PicoDst();
    return 0;
}
