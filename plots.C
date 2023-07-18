#include <iostream>
#include <fstream>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include "TLine.h"
#include "TPad.h"

using namespace std;

TH1F *hspectra[9];
TFile *file = new TFile("/home/ubuntu/1Myresults/isobar_Test.root", "READ");
TH1F *h1D[2][11];
TH2F *h2D[2][6];
TH1F *hPt[9];
TCanvas *c;
TString config_cut[2] = {"before", "after"};
TString name[18] = {"mult", "Vz", "Eta", "pHits", "pHitsFit", "pHitsMax", "pFitPoss", "DCAz",
                    "DCAmag", "DCAxy", "pdedxHits", "Vxy", "pDedx", "reverseBeta", "EtaPt", "EtaPt", "PhiPt", "squaredMass"}; //"EtaPt"
TString centc[9] = {"0-5", "5-10", "10-20", "20-30", "30-40", "40-50", "50-60", "60-70", "70-80"};

void plot1D()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            file->GetObject(Form("hist_%s_%s", name[j].Data(), config_cut[i].Data()), h1D[i][j]);
            c = new TCanvas();
            h1D[i][j]->Draw("col");
            if (j == 0)
            {
                gPad->SetLogy();
            }
            c->SaveAs(Form("/home/ubuntu/1Myresults/plots/%s_%s.png", name[j].Data(), config_cut[i].Data()));
        }
    }
}
void plot2D()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 11; j < 18; j++)
        {

            file->GetObject(Form("hist_%s_%s", name[j].Data(), config_cut[i].Data()), h2D[i][j]);
            c = new TCanvas();
            h2D[i][j]->Draw("col");
            if (j == 12 || j == 13)
            {
                gPad->SetLogz();
            }
           if(j==15)
           continue;
            c->SaveAs(Form("/home/ubuntu/1Myresults/plots/%s_%s.png", name[j].Data(), config_cut[i].Data()));
        }
    }
}

void plotPt()
{
    for (int i = 0; i < 9; i++)
    {
        file->GetObject(Form("hist_spectra_cent%i", i), hspectra[i]);
        c = new TCanvas();
        hspectra[i]->Draw("col");
        gPad->SetLogy();
        hspectra[i]->SetXTitle("p_{T} [Gev/c]");
        hspectra[i]->SetYTitle("Yields");
        c->SaveAs(Form("/home/ubuntu/1Myresults/plots/%s_spectra.png", centc[i].Data()));
    }
}

int plots()
{
    // plot1D();
    plot2D();
    // plotPt();

    return 0;
}
