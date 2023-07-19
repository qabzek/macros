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
TFile *file = new TFile("/home/ubuntu/1Myresults/isobar_RuRu_2018.root", "READ");
TH1F *h1D[2][11];
TH2F *h2D[2][6];
TH1F *hPt[9];
TH1F *hCentCount;
Float_t Nevents;
TCanvas *c;
TCanvas *c1;
TString config_cut[2] = {"before", "after"};
TString name[17] = {"mult", "Vz", "Eta", "pHits", "pHitsFit", "pHitsMax", "pFitPoss", "DCAz",
                    "DCAmag", "DCAxy", "pdedxHits", "Vxy", "pDedx", "reverseBeta", "EtaPt", "squaredMass", "PhiPt"}; //"EtaPt"
TString centc[9] = {"70-80", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "5-10", "0-5"};
EColor *col;
int marker[9] = {20,21,22,24,25,26,29,30,31}; 

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
            c->SaveAs(Form("/home/ubuntu/1Myresults/isobar_2018/RuRu/%s_%s.png", name[j].Data(), config_cut[i].Data()));
        }
    }
}
void plot2D()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 11; j < 17; j++)
        {

            file->GetObject(Form("hist_%s_%s", name[j].Data(), config_cut[i].Data()), h2D[i][j]);
            c = new TCanvas();
            //if(j==18)
            //continue;
            if (j == 12 || j == 13 || j == 15)
            {
                gPad->SetLogz();
            }
            h2D[i][j]->Draw("col");
            c->SaveAs(Form("/home/ubuntu/1Myresults/isobar_2018/RuRu/%s_%s.png", name[j].Data(), config_cut[i].Data()));
        }
    }
}

void plotPt()
{
       
    c = new TCanvas(); 
    //c1 = new TCanvas();    
    for (int i = 0; i < 9; i++)
    {
        file->GetObject(Form("hist_spectra_cent%i", i), hspectra[i]);
        file->GetObject("CentCount",hCentCount);
        Float_t Numbins;
        Nevents= hCentCount->GetBinContent(i+1);
        Numbins = hspectra[i]->GetNbinsX();
        for(int j=0;j<Numbins;j++)
        {
            hspectra[i]->SetBinContent(j, (hspectra[i]->GetBinContent(j)/Nevents) * pow(2,i));
            hspectra[i]->SetBinError(j, hspectra[i]->GetBinError(j)/Nevents);
        }
        hspectra[i]->Sumw2();
        hspectra[i]->Draw("same");
        hspectra[i]->SetAxisRange(0.00001,10000,"Y");
        gPad->SetLogy();
        hspectra[i]->SetXTitle("p_{T} [Gev/c]");
        hspectra[i]->SetYTitle("Yields");
        //hspectra[i]->SetLineColor(col[i]);
        hspectra[i]->SetMarkerStyle(marker[i]);
        //c->SaveAs(Form("/home/ubuntu/1Myresults/isobar_2018/RuRu/%s_spectra.png", centc[i].Data()));
    }
    c->SaveAs("/home/ubuntu/1Myresults/isobar_2018/RuRu/cent_Pt_spectra.png");
}

int plots()
{
    //plot1D();
    //plot2D();
    plotPt();

    return 0;
}
