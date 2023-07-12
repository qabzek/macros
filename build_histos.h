#include <iostream>

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
TH1F *hist1[2][12];
TH2F *hist2[2][6];
TString config[2] = {"before","after "};
TString name[18] = {"mult", "VtxZ", "Eta", "pHits", "pHitsFit", "pHitsMax",
                    "pFitPoss", "gHits", "gHitsFit", "gHitsMax", "gFitPoss","pDedxHits",
                    "VtxX_VyxY", "pDedx", "reveraseBeta", "PhiPt", "pMomTrVEta", "sqaredMass"};
Int_t bin[18] = {475, 100, 100, 80, 80, 80, 100, 80, 80, 80, 100, 40, 200, 1000, 1000, 1000, 200, 1000};
Double_t xmin[18] = {0, -250, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0.1, -10, -2, -3, 0, 0, -3};
Double_t xmax[18] = {475, 250, 3, 80, 80, 80, 1.1, 80, 80, 80, 1.1, 80, 10, 2, 3, 5, 10, 3};
Double_t ymin[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, 0, 0.7, -3.5, -3, -0.5};
Double_t ymax[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 50, 4, 3.5, 3, 1.5};


int i,j;

void build()
{   
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 18; j++)
        {
            if (j < 12)
                hist1[i][j] =new TH1F(Form("hist_%s_%s", name[j].Data(), config[i].Data()),
                    Form("%s_%s", name[j].Data(), config[i].Data()), bin[j], xmin[j], xmax[j]);
            else
                hist2[i][j] = new TH2F(Form("hist_%s_%s", name[j].Data(), config[i].Data()),
                      Form("%s_%s", name[j].Data(), config[i].Data()), bin[j], xmin[j], xmax[j], bin[j], ymin[j], ymax[j]);
        }       
    }
}


