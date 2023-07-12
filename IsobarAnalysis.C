#define _VANILLA_ROOT_
// C++ headers
#include <iostream>
#include "build_histos.h"

// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"
#include "TStyle.h"
#include "TCanvas.h"

// PicoDst headers
#include "../StPicoEvent/StPicoDstReader.h"
#include "../StPicoEvent/StPicoDst.h"
#include "../StPicoEvent/StPicoEvent.h"
#include "../StPicoEvent/StPicoTrack.h"
#include "../StPicoEvent/StPicoBTofHit.h"
#include "../StPicoEvent/StPicoBTowHit.h"
#include "../StPicoEvent/StPicoEmcTrigger.h"
#include "../StPicoEvent/StPicoBTofPidTraits.h"
#include "../StPicoEvent/StPicoTrackCovMatrix.h"
//_________________
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
R__LOAD_LIBRARY(../StPicoEvent/libStPicoDst);
#else
gSystem->Load("../StPicoEvent/libStPicoDst.so");
#endif

const Char_t *defaultOutFile = "19.6GeV_Test.root";

int AnalysisModified(const Char_t *inFile = "/home/ubuntu/RootFiles/st_physics_*.picoDst.root", const Char_t *outFileName = defaultOutFile)
{

    StPicoDstReader *picoReader = new StPicoDstReader(inFile);
    picoReader->Init();

    // This is a way if you want to spead up I/O
    picoReader->SetStatus("*", 0);
    picoReader->SetStatus("Event*", 1);
    picoReader->SetStatus("Track*", 1);
    picoReader->SetStatus("BTofPidTraits*", 1);

    TFile *outputFile = new TFile(defaultOutFile, "recreate");

    // histogram building
    
    build();

    Long64_t events2read = picoReader->chain()->GetEntries();
    for (Long64_t iEvent = 0; iEvent < events2read; iEvent++)
    {

        Int_t multCounter = 0;

        if (iEvent % 1000 == 0)
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
    }
}