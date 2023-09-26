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
#include "../StROOT/StPicoEvent/StPicoDstReader.h"
#include "../StROOT/StPicoEvent/StPicoDst.h"
#include "../StROOT/StPicoEvent/StPicoEvent.h"
#include "../StROOT/StPicoEvent/StPicoTrack.h"
#include "../StROOT/StPicoEvent/StPicoBTofHit.h"
#include "../StROOT/StPicoEvent/StPicoBTowHit.h"
#include "../StROOT/StPicoEvent/StPicoEmcTrigger.h"
#include "../StROOT/StPicoEvent/StPicoBTofPidTraits.h"
#include "../StROOT/StPicoEvent/StPicoTrackCovMatrix.h"
#include "../StROOT/StRefMultCorr/StRefMultCorr.h"
#include "../StROOT/StRefMultCorr/BadRun.h"
#include "../StROOT/StRefMultCorr/CentralityMaker.h"
#include "../StROOT/StRefMultCorr/Param.h"
//_________________
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
R__LOAD_LIBRARY(../StROOT/StPicoEvent/libStPicoDst);
#else
gSystem->Load("../StROOT/StPicoEvent/libStPicoDst.so");
#endif

#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 0, 0)
R__LOAD_LIBRARY(../StROOT/StRefMultCorr/libStRefMultCorr);
#else
gSystem->Load("../StROOT/StRefMultCorr/libStRefMultCorr.so");
#endif

const Char_t *defaultOutFile = "../1Myresults/isobar_RuRu_2018.root";

Double_t tofBeta = -999;
Double_t ToF = -999;
short matchflag = -999;
Double_t mass2;
Int_t cent9 = -999;
Double_t CentWeight;
Int_t multCounter = 0;
Float_t flFit, flPoss;
double dEta = 2, dPt,dPtpip,dPtpim,dPtkap,dPtkam,dPtpr,dPtapr;

int IsobarAnalysis(const Char_t *inFile = "/home/ubuntu/folder/isobar_files/production_isobar_2018/RuRu/st_physics_*.picoDst.root", const Char_t *outFileName = defaultOutFile)
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
    TH1D *hist_CentCount = new TH1D("CentCount", "CentCount", 10, 0, 10);

    //event loop
    Long64_t events2read = picoReader->chain()->GetEntries();
    for (Long64_t iEvent = 0; iEvent < events2read; iEvent++)
    {

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

       
       
       
        // same filling with cuts
        if (!((event->primaryVertex().Z()) > -35 && (event->primaryVertex().Z()) < 25 &&
              TMath::Sqrt(pow(event->primaryVertex().X(), 2) + pow(event->primaryVertex().Y(), 2)) < 2))
            continue;
        // Vxy
        hist_Vxy[1]->Fill(event->primaryVertex().X(), event->primaryVertex().Y());
        // Vz
        hist_Vz[1]->Fill(event->primaryVertex().Z());
        hist_CentCount->Fill(cent9, CentWeight);

        
        //track loop
        Int_t nTracks = picoDst->numberOfTracks();
        for (Int_t iTrack = 0; iTrack < nTracks; iTrack++)
        {
            StPicoTrack *picoTrack = picoDst->track(iTrack);
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
                     tofBeta = trait->btofBeta();
                     ToF = trait->btof();
                     matchflag = trait->btofMatchFlag();
                     mass2 = (picoTrack->pPt() / picoTrack->charge(), (1 / pow(tofBeta, 2) - 1) * pow(picoTrack->pPt(), 2));
                    if (!trait)
                        continue;

                    // Fill 1/beta
                    hist_reverseBeta[0]->Fill(picoTrack->pPtot() / picoTrack->charge(), 1. / trait->btofBeta());
                    // fill squared mass
                    hist_squaredMass[0]->Fill(picoTrack->pPt() / picoTrack->charge(), (1 / pow(trait->btofBeta(), 2) - 1) * pow(picoTrack->pPt(), 2));
                }
            }

            // track cuts
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
            if (picoTrack->isPrimary())
            {
                // fill pseudorapidity
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
                // fill nsigma
                hist_nspion->Fill(picoTrack->nSigmaPion());
                hist_nskaon->Fill(picoTrack->nSigmaKaon());
                hist_nsproton->Fill(picoTrack->nSigmaProton());

                // fill for TOF
                if (picoTrack->isTofTrack())
                {
                    // Retrieve corresponding trait
                     StPicoBTofPidTraits *trait = picoDst->btofPidTraits(picoTrack->bTofPidTraitsIndex());
                     tofBeta = trait->btofBeta();
                     ToF = trait->btof();
                     short matchflag = trait->btofMatchFlag();
                     mass2 = (picoTrack->pPt() / picoTrack->charge(), (1 / pow(tofBeta, 2) - 1) * pow(picoTrack->pPt(), 2));
                    if (!trait) 
                        continue;

                    // Fill 1/beta
                    hist_reverseBeta[1]->Fill(picoTrack->pPtot() / picoTrack->charge(), 1. / trait->btofBeta());
                    // fill squared mass
                    hist_squaredMass[1]->Fill(picoTrack->pPt() / picoTrack->charge(), (1 / pow(trait->btofBeta(), 2) - 1) * pow(picoTrack->pPt(), 2));
                }
                
                //dPt
                dPt = (hist_spectra[0]->GetBinCenter(hist_spectra[0]->GetNbinsX()) - hist_spectra[0]->GetBinCenter(0)) / hist_spectra[0]->GetNbinsX();
                dPtpip = (hist_pip[0][0]->GetBinCenter(hist_pip[0][0]->GetNbinsX()) - hist_pip[0][0]->GetBinCenter(0)) / hist_pip[0][0]->GetNbinsX();
                dPtpim = (hist_pim[0][0]->GetBinCenter(hist_pim[0][0]->GetNbinsX()) - hist_pim[0][0]->GetBinCenter(0)) / hist_pim[0][0]->GetNbinsX();
                dPtkap = (hist_kap[0][0]->GetBinCenter(hist_kap[0][0]->GetNbinsX()) - hist_kap[0][0]->GetBinCenter(0)) / hist_kap[0][0]->GetNbinsX();
                dPtkam = (hist_kam[0][0]->GetBinCenter(hist_kam[0][0]->GetNbinsX()) - hist_kam[0][0]->GetBinCenter(0)) / hist_kam[0][0]->GetNbinsX();
                dPtpr = (hist_pr[0][0]->GetBinCenter(hist_pr[0][0]->GetNbinsX()) - hist_pr[0][0]->GetBinCenter(0)) / hist_pr[0][0]->GetNbinsX();
                dPtapr = (hist_apr[0][0]->GetBinCenter(hist_apr[0][0]->GetNbinsX()) - hist_apr[0][0]->GetBinCenter(0)) / hist_apr[0][0]->GetNbinsX();
                
                
                
                // fill pt spectra for each centality
                hist_spectra[cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPt * dEta);
                
                // PID with nsigma
                if (TMath::Abs(picoTrack->nSigmaPion()) < 2)
                    {
                        if (picoTrack->charge() == 1)
                            hist_pip[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpip * dEta);
                        else
                            hist_pim[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpim * dEta);
                    }
                
                if (TMath::Abs(picoTrack->nSigmaKaon()) < 3)
                    {
                        if (picoTrack->charge() == 1)
                            hist_kap[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtkap * dEta);
                        else
                            hist_kam[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtkam * dEta);
                    }
                
                if (TMath::Abs(picoTrack->nSigmaProton()) < 2)
                    {
                        if (picoTrack->charge() == 1)
                            hist_pr[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpr * dEta);
                        else
                            hist_apr[0][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtapr * dEta);
                    }
                
                
                //PID with TPC+TOF selection
                if(tofBeta > 0 && ToF > 0 && matchflag > 0)    
                   { 
                    if (TMath::Abs(picoTrack->nSigmaPion()) < 2 && mass2 > -0.1 && mass2 < 0.1)
                    {
                        if (picoTrack->charge() == 1)
                            hist_pip[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpip * dEta);
                        else
                            hist_pim[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpim * dEta);
                    }

                    if (TMath::Abs(picoTrack->nSigmaKaon()) < 3 && mass2 > 0.11 && mass2 < 0.32)
                    {
                        if (picoTrack->charge() == 1)
                            hist_kap[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtkap * dEta);
                        else
                            hist_kam[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtkam * dEta);
                    }
                    if (TMath::Abs(picoTrack->nSigmaProton()) < 2 && mass2 > 0.35 && mass2 < 1.1)
                    {
                        if (picoTrack->charge() == 1)
                            hist_pr[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtpr * dEta);
                        else
                            hist_apr[1][cent9]->Fill(picoTrack->pMom().Pt(), 1. / 2 * M_PI * (picoTrack->pMom().Pt()) * dPtapr * dEta);
                    }
                   }
            }
           //End of track loop 
        }
        // Filling multiplicity after cuts
        hist_mult[1]->Fill(event->refMult());

        // End of event loop
    }
    picoReader->Finish();
    outputFile->Write();
    outputFile->Close();
    return 0;
}