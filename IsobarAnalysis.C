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
#include "StPicoEvent/StPicoDstReader.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTofHit.h"
#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoEmcTrigger.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoTrackCovMatrix.h"
#include "StRefMultCorr/StRefMultCorr.h"
#include "StRefMultCorr/BadRun.h"
#include "StRefMultCorr/CentralityMaker.h"
#include "StRefMultCorr/Param.h"
//_________________
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
R__LOAD_LIBRARY(StPicoEvent/libStPicoDst);
#else
gSystem->Load("StPicoEvent/libStPicoDst.so");
#endif

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
R__LOAD_LIBRARY(StRefMultCorr/libStRefMultCorr);
#else
gSystem->Load("StPicoEvent/libStRefMultCorr.so");
#endif

const Char_t *defaultOutFile = "../1Myresults/isobar_RuRu_2018.root";

int IsobarAnalysis(const Char_t *inFile = "/home/ubuntu/isobar_files/production_isobar_2018/RuRu/st_physics_*.picoDst.root", const Char_t *outFileName = defaultOutFile)
{

    StPicoDstReader *picoReader = new StPicoDstReader(inFile);
    picoReader->Init();

    // This is a way if you want to spead up I/O
    picoReader->SetStatus("*", 0);
    picoReader->SetStatus("Event*", 1);
    picoReader->SetStatus("Track*", 1);
    picoReader->SetStatus("BTofHit*", 1);
    picoReader->SetStatus("BTofPidTraits*", 1);

    TFile *outputFile = new TFile(defaultOutFile, "recreate");

    // histogram building
    
    build();
    build_pt();
    TH1D *hist_CentCount = new TH1D("CentCount","CentCount",10,0,10);

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
        

            // refmult
            hist_mult[0]->Fill(event->refMult());
            // Vxy
            hist_Vxy[0]->Fill(event->primaryVertex().X(), event->primaryVertex().Y());
            // Vz
            hist_Vz[0]->Fill(event->primaryVertex().Z());

            

            // cut badruns PileUp and make centrality
            int cent9 = -999;
            double CentWeight;
            StRefMultCorr *refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
            refmultCorrUtil->init(event->runId());
            Bool_t isBadRun = refmultCorrUtil->isBadRun(event->runId());
            if (refmultCorrUtil->isPileUpEvent(event->refMult(), event->nBTOFMatch(), event->primaryVertex().Z()))
                continue;
            refmultCorrUtil->initEvent(event->refMult(), event->primaryVertex().Z(), event->ZDCx());
            cent9 = refmultCorrUtil->getCentralityBin9();
            CentWeight = refmultCorrUtil->getWeight();
            if (isBadRun)
                continue;
            if (cent9 < 0)
                continue;


            // same with cuts
            if  (!((event->primaryVertex().Z()) > -35 && (event->primaryVertex().Z()) < 25 &&
            TMath::Sqrt(pow(event->primaryVertex().X(), 2) + pow(event->primaryVertex().Y(), 2)) < 2))
            continue;
                // Vxy
                hist_Vxy[1]->Fill(event->primaryVertex().X(), event->primaryVertex().Y());
                // Vz
                hist_Vz[1]->Fill(event->primaryVertex().Z());
                hist_CentCount->Fill(cent9, CentWeight);

            Int_t nTracks = picoDst->numberOfTracks();
            for (Int_t iTrack = 0; iTrack < nTracks; iTrack++)
            {
                StPicoTrack *picoTrack = picoDst->track(iTrack);
                Float_t flFit, flPoss;
                flFit = static_cast<float>(picoTrack->nHitsFit());
                flPoss = static_cast<float>(picoTrack->nHitsPoss());
                if (!picoTrack)
                    continue;

                if (picoTrack->isPrimary())
                {
                    hist_Eta[0]->Fill(picoTrack->pMom().Eta());
                    // fill primary track hits
                    hist_pHits[0]->Fill(picoTrack->nHits());
                    hist_pHitsFit[0]->Fill(picoTrack->nHitsFit());
                    hist_pHitsMax[0]->Fill(picoTrack->nHitsMax());
                    hist_pFitPoss[0]->Fill(flFit / flPoss);
                    // fill DCA
                    hist_DCAz[0]->Fill(picoTrack->gDCA(event->primaryVertex()).Z());
                    hist_DCAmag[0]->Fill(picoTrack->gDCA(event->primaryVertex()).Mag());
                    hist_DCAxy[0]->Fill(picoTrack->gDCA(event->primaryVertex()).X(), picoTrack->gDCA(event->primaryVertex()).Y());
                    // fill dE/dx hits
                    hist_pDedxHits[0]->Fill(picoTrack->nHitsDedx());
                    // fill dEdx p/q
                    hist_pDedx[0]->Fill(picoTrack->pPtot() / picoTrack->charge(), picoTrack->dEdx());
                    // fill Phi vs Pt
                    hist_PhiPt[0]->Fill(picoTrack->pPt(), picoTrack->pMom().Phi());
                    // fill Eta vs Pt
                    hist_EtaPt[0]->Fill(picoTrack->pPt(), picoTrack->pMom().Eta());

                    // fill for TOF
                    if (picoTrack->isTofTrack())
                    {
                        // Retrieve corresponding trait
                        StPicoBTofPidTraits *trait = picoDst->btofPidTraits(picoTrack->bTofPidTraitsIndex());
                        if (!trait)
                            continue;

                        // Fill 1/beta
                        hist_reverseBeta[0]->Fill(picoTrack->pPtot() / picoTrack->charge(), 1. / trait->btofBeta());
                        // fill squared mass
                        hist_squaredMass[0]->Fill(picoTrack->pPt() / picoTrack->charge(), (1 / pow(trait->btofBeta(), 2) - 1) * pow(picoTrack->pPt(), 2));
                    }
                }

                // with cuts
                if (!(picoTrack->gDCA(event->primaryVertex()).Mag() < 3 &&
                      (event->primaryVertex().Z()) > -35 && (event->primaryVertex().Z()) < 25 &&
                      TMath::Sqrt(pow(event->primaryVertex().X(), 2) + pow(event->primaryVertex().Y(), 2)) < 2 &&
                      picoTrack->pPt() < 5 &&
                      picoTrack->pPt() > 0.015 &&
                      TMath::Abs(picoTrack->pMom().Eta()) < 1 &&
                      picoTrack->nHitsDedx() > 10 &&
                      picoTrack->nHitsFit() > 15 &&
                      flFit / flPoss > 0.52))

                    continue;
                if(picoTrack->isPrimary())
                {
                hist_Eta[1]->Fill(picoTrack->pMom().Eta());
                // fill primary track hits
                hist_pHits[1]->Fill(picoTrack->nHits());
                hist_pHitsFit[1]->Fill(picoTrack->nHitsFit());
                hist_pHitsMax[1]->Fill(picoTrack->nHitsMax());
                hist_pFitPoss[1]->Fill(flFit / flPoss);
                // fill DCA
                hist_DCAz[1]->Fill(picoTrack->gDCA(event->primaryVertex()).Z());
                hist_DCAmag[1]->Fill(picoTrack->gDCA(event->primaryVertex()).Mag());
                hist_DCAxy[1]->Fill(picoTrack->gDCA(event->primaryVertex()).X(), picoTrack->gDCA(event->primaryVertex()).Y());
                // fill dE/dx hits
                hist_pDedxHits[1]->Fill(picoTrack->nHitsDedx());
                // fill dEdx p/q
                hist_pDedx[1]->Fill(picoTrack->pPtot() / picoTrack->charge(), picoTrack->dEdx());
                // fill Phi vs Pt
                hist_PhiPt[1]->Fill(picoTrack->pPt(), picoTrack->pMom().Phi());
                // fill Eta vs Pt
                hist_EtaPt[1]->Fill(picoTrack->pPt(), picoTrack->pMom().Eta());

                // fill for TOF
                if (picoTrack->isTofTrack())
                {
                    // Retrieve corresponding trait
                    StPicoBTofPidTraits *trait = picoDst->btofPidTraits(picoTrack->bTofPidTraitsIndex());
                    if (!trait)
                        continue;
                    // Fill 1/beta
                    hist_reverseBeta[1]->Fill(picoTrack->pPtot() / picoTrack->charge(), 1. / trait->btofBeta());
                    // fill squared mass
                    hist_squaredMass[1]->Fill(picoTrack->pPt() / picoTrack->charge(), (1 / pow(trait->btofBeta(), 2) - 1) * pow(picoTrack->pPt(), 2));
                }
                // fill pt spectra for each centality
                double dEta = 2, dPt;
                dPt = (hist_spectra[0]->GetBinCenter(hist_spectra[0]->GetNbinsX()) - hist_spectra[0]->GetBinCenter(0)) / hist_spectra[0]->GetNbinsX();
                hist_spectra[cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2*M_PI * (picoTrack->pMom().Pt()) * dPt * dEta);
                }
            }
        // Filling multiplicity after cuts
            hist_mult[1]->Fill(event->refMult());
        
    //End of event cut 
    }
    picoReader->Finish();
    outputFile->Write();
    outputFile->Close();
    return 0;
}