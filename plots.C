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
#include "TStyle.h"
#include "TLegend.h"
#include <TH2.h>

using namespace std;

TFile *file = new TFile("/home/ubuntu/folder/1Myresults/isobar_RuRu_2018.root", "READ");

TH1D *hspectra[9];
TH1D *h1D[2][11];
TH2D *h2D[2][6];
TH1D *hPt[9];
TH1D *h_py[3][30];
TH1D *hCentCount;


Float_t Nevents;
TCanvas *c;
TCanvas *c1;
TLegend *legend; 
TString config_cut[2] = {"before", "after"};
TString name[17] = {"mult", "Vz", "Eta", "pHits", "pHitsFit", "pHitsMax", "pFitPoss", "DCAz",
                    "DCAmag", "DCAxy", "pdedxHits", "Vxy", "pDedx", "reverseBeta", "EtaPt", "squaredMass", "PhiPt"}; //"EtaPt"
TString centc[9] = {"70-80", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "5-10", "0-5"};
int col[9]    = {1,632,600,416,840,616,880,800,900};
int marker[9] = {20,21,25,24,22,26,29,30,31}; 
double x[31]={0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,
              1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,
              2.2,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,3.1};
double xmax[17]={0,0,0,0,0,0,0,0,0,0,0,10,2,0,0.4,0};
double xmin[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.4,0};
              


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
            c->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/QA/%s_%s.png", name[j].Data(), config_cut[i].Data()));
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
            c->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/QA/%s_%s.png", name[j].Data(), config_cut[i].Data()));
        }
    }
}

void plotPt()
{
       
    c = new TCanvas(); 
    legend = new TLegend(0.6,0.7,0.9,0.9);
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
        hspectra[i]->SetLineColor(col[i]);
        hspectra[i]->SetMarkerColor(col[i]);
        hspectra[i]->SetMarkerStyle(marker[i]);

        
        legend->SetNColumns(3);
        legend->AddEntry(hspectra[i],Form("%s",centc[i].Data()),"lp");
        legend->Draw();
        //c->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/%s_spectra.png", centc[i].Data()));
    }
    c->SaveAs("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/cent_Pt_spectra.png");
}

void pid()
{
    for(int j=12;j<16;j++)
    {
    if(j==14) continue;
    file->GetObject(Form("hist_%s_after",name[j].Data()), h2D[1][j]);
    for(int i=0;i<30;i++)
    {
        c1 = new TCanvas();
    
        Double_t firstx = x[i], lastx = x[i+1];
        Int_t firstxbin = h2D[1][j]->GetXaxis()->FindFixBin(firstx);
        Int_t lastxbin = h2D[1][j]->GetXaxis()->FindFixBin(lastx);
        h_py[j][i] = new TH1D(Form("%s_py_%f_%f",name[j].Data(),firstx,lastx),Form("%s projection at p = %f-%f GeV/c",name[j].Data(),firstx,lastx),60,xmin[j],xmax[j]);
        h_py[j][i] = h2D[1][j]->ProjectionY("_py", firstxbin, lastxbin, "");
        gPad->SetLogy();
        h_py[j][i]->Draw();
        c1->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/PID/%s_py_%f_%f.png",name[j].Data(),firstx,lastx));
    }
    }
    //file->GetObject("hist_reverseBeta_after", hist_revbeta);
    //file->GetObject("hist_squaredMass_after", hist_msq);
    
}
int plots()
{
    gStyle->SetOptStat(0);
    //plot1D();
    //plot2D();
    //plotPt();
    pid();

    return 0;
}
