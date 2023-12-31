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
#include "TRatioPlot.h"
#include "TLatex.h"
using namespace std;

TFile *file = new TFile("/home/ubuntu/folder/1Myresults/isobar_RuRu_2018.root", "READ");

TH1D *hspectra1[6][9];
TH1D *hspectra2[6][9];
TH1D *h1D[2][11];
TH2D *h2D[2][6];
TH1D *hPt[9];
TH1D *hdedx_py;
TH1D *hbeta_py;
TH1D *hCentCount;
TH1D *hnspion;
TH1D *hnskaon;
TH1D *hnsproton;
TH2D *hmass2;
Float_t Nevents;
TCanvas *c;
TCanvas *c1;
TCanvas *c2;
TCanvas *c3;
TLegend *legend; 
TLatex *t;
TLatex *t1;
TLatex *t2;
TLatex *t3;



TString config_cut[2] = {"before", "after"};
TString name[17]      = {"mult", "Vz", "Eta", "pHits", "pHitsFit", "pHitsMax", "pFitPoss", "DCAz",
                         "DCAmag", "DCAxy", "pdedxHits", "Vxy", "pDedx", "reverseBeta", "EtaPt", "squaredMass", "PhiPt"}; //"EtaPt"
TString centc[9]      = {"70-80", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "5-10", "0-5"};
int col[9]            = {1,632,600,416,840,616,880,800,900};
int marker[9]         = {20,21,25,24,22,26,29,30,31}; 
Double_t x[75]        = {
                         0.1,0.125,0.15,0.175,0.2,0.225,0.25,0.275,0.3,0.325,0.35,0.375,0.4,0.425,0.45,0.475,0.5,0.525,0.55,0.575,0.6,0.625,0.65,0.675,0.7,0.725,0.75,0.775,0.8,0.825,0.85,0.875,0.9,0.925,0.95,0.975,1,1.05,
                         1.1,1.125,1.15,1.175,1.2,1.225,1.25,1.275,1.3,1.325,1.35,1.375,1.4,1.425,1.45,1.475,1.5,1.525,1.55,1.575,1.6,1.625,1.65,1.675,1.7,1.725,1.75,1.775,1.8,1.825,1.85,1.875,1.9,1.925,1.95,1.975,2};
TString part[6]       = {"#pi^{+}","#pi^{-}","K^{+}","K^{-}","p","#bar{p}"};
              


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
       
    //c1 = new TCanvas();    
    for(int k=0;k<6;k++)
    {
    for (int i = 0; i < 9; i++)
    {
        c = new TCanvas("", "", 1800,1000); 
        legend = new TLegend(0.68,0.55,0.65,0.85);
    
        file->GetObject(Form("hist_%s_nscut_cent%s", part[k].Data(), centc[i].Data()), hspectra1[k][i]);
        file->GetObject(Form("hist_%s_nsm2cut_cent%s", part[k].Data(), centc[i].Data()), hspectra2[k][i]);
        file->GetObject("CentCount",hCentCount);
        Float_t Numbins1,Numbins2;
        Nevents= hCentCount->GetBinContent(i+1);
        Numbins1 = hspectra1[k][i]->GetNbinsX();
        Numbins2 = hspectra2[k][i]->GetNbinsX();
        for(int j=0;j<Numbins1;j++)
        {
            hspectra1[k][i]->SetBinContent(j, (hspectra1[k][i]->GetBinContent(j)/Nevents));
            hspectra1[k][i]->SetBinError(j, hspectra1[k][i]->GetBinError(j)/Nevents);
        }
        for(int j=0;j<Numbins2;j++)
        {
            hspectra2[k][i]->SetBinContent(j, (hspectra2[k][i]->GetBinContent(j)/Nevents));
            hspectra2[k][i]->SetBinError(j, hspectra2[k][i]->GetBinError(j)/Nevents);
        }
        hspectra1[k][i]->Sumw2();
        hspectra2[k][i]->Sumw2();
        hspectra1[k][i]->Draw();
        hspectra2[k][i]->Draw("same");
        hspectra1[k][i]->SetAxisRange(1e-9,1e10,"Y");
        hspectra2[k][i]->SetAxisRange(1e-9,1e10,"Y");
        hspectra1[k][i]->SetAxisRange(0,9,"X");
        hspectra2[k][i]->SetAxisRange(0,9,"X");
        gPad->SetLogy();
        hspectra1[k][i]->SetXTitle("p_{T} [Gev/c]");
        hspectra2[k][i]->SetXTitle("p_{T} [Gev/c]");
        hspectra1[k][i]->SetYTitle("#frac{1}{2#pi} #frac{d^{2}N}{p_{T}dp_{T}d#eta} [(GeV/c)^{2}]");
        hspectra2[k][i]->SetYTitle("#frac{1}{2#pi} #frac{d^{2}N}{p_{T}dp_{T}d#eta} [(GeV/c)^{2}]");
        hspectra1[k][i]->SetLineColor(1);
        hspectra2[k][i]->SetLineColor(600);
        hspectra1[k][i]->SetMarkerColor(1);
        hspectra2[k][i]->SetMarkerColor(600);
        hspectra1[k][i]->SetMarkerStyle(24);
        hspectra2[k][i]->SetMarkerStyle(20);
        TPad *p;
        TRatioPlot *rp = new TRatioPlot(hspectra1[k][i],hspectra2[k][i]);
        rp->SetH1DrawOpt("E");
        rp->SetH2DrawOpt("E");
        rp->Draw();
        p = rp->GetUpperPad();
        p->cd();

        rp->GetLowerRefYaxis()->SetTitle("ratio");
        rp->GetUpperRefYaxis()->SetTitle("#frac{1}{2#pi} #frac{d^{2}N}{p_{T}dp_{T}d#eta} [(GeV/c)^{2}]");
        rp->GetLowerRefGraph()->SetMinimum(0);
        rp->GetLowerRefGraph()->SetMaximum(25);
        t = new TLatex(1,1e7,Form("Ru+Ru 200 GeV, %s",part[k].Data()));
        t->Draw();
        legend->SetTextSize(0.05);
        legend->SetBorderSize(0);
       // legend->SetNColumns(1);
        legend->AddEntry(hspectra1[k][i],Form("%s, n#sigma selection",centc[i].Data()),"p");
        legend->AddEntry(hspectra2[k][i],Form("%s, n#sigma and m^2 selection",centc[i].Data()),"p");
        legend->Draw();
        c->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/%s_%s_spectra.png",part[k].Data(),centc[i].Data()));
        //c->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/%s_spectra.png", centc[i].Data()));
    }
    }
    
    file->GetObject("hist_nspion",hnspion);
    file->GetObject("hist_nskaon",hnskaon);
    file->GetObject("hist_nsproton",hnsproton);
    file->GetObject("hist_squaredMass_after",hmass2);
    c=new TCanvas();
    hnspion->Draw();
    t3 = new TLatex(-14,14e6,"Ru+Ru 200 GeV,n#sigma_{#pi}");
    t3->Draw();
    TLine* l1 = new TLine(-2,0,-2,18*1e6);
    TLine* l2 = new TLine(2,0,2,18*1e6);
    l1->SetLineWidth(2);
    l2->SetLineWidth(2);
    l1->Draw();
    l2->Draw();
    c->SaveAs("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/nspioncut.png");
    c1=new TCanvas();
    hnskaon->Draw();
    t1 = new TLatex(-14.3,4e6,"Ru+Ru 200 GeV,n#sigma_{K}");
    t1->Draw();
    TLine* l3 = new TLine(-3,0,-3,4.5*1e6);
    TLine* l4 = new TLine(3,0,3,4.5*1e6);
    l3->SetLineWidth(2);
    l4->SetLineWidth(2);
    l3->Draw();
    l4->Draw();
    c1->SaveAs("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/nskaoncut.png");
    c2=new TCanvas();
    hnsproton->Draw();
    t2 = new TLatex(-14,18e5,"Ru+Ru 200 GeV,n#sigma_{p}");
    t2->Draw();
    TLine* l5 = new TLine(-2,0,-2,2*1e6);
    TLine* l6 = new TLine(2,0,2,2*1e6);
    l5->SetLineWidth(2);
    l6->SetLineWidth(2);
    l5->Draw();
    l6->Draw();
    c2->SaveAs("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/nsprotoncut.png");
    c3= new TCanvas();
    gPad->SetLogz();
    hmass2->Draw("col");
    TLine* l7 =new TLine(-3,-0.1,3,-0.1);
    l7->SetLineWidth(2);
    l7->SetLineColor(1);
    TLine* l8 =new TLine(-3,0.1,3,0.1);
    l8->SetLineWidth(2);
    l8->SetLineColor(1);
    TLine* l9 =new TLine(-3,0.11,3,0.11);
    l9->SetLineWidth(2);
    l9->SetLineColor(2);
    TLine* l10 =new TLine(-3,0.32,3,0.32);
    l10->SetLineWidth(2);
    l10->SetLineColor(2);
    TLine* l11 =new TLine(-3,0.35,3,0.35);
    l11->SetLineWidth(2);
    l11->SetLineColor(28);
    TLine* l12 =new TLine(-3,1.1,3,1.1);
    l12->SetLineWidth(2);
    l12->SetLineColor(28);

    l7->Draw();
    l8->Draw();
    l9->Draw();
    l10->Draw();
    l11->Draw();
    l12->Draw();
    c3->SaveAs("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/Spectra/mass2cut.png");

}
void pid()
{
    
    
    file->GetObject("hist_pDedx_after", h2D[1][1]);
    for(int i=0;i<75;i++)
    {
        legend = new TLegend(0.6,0.7,0.9,0.9);
        c1 = new TCanvas();
    
        Double_t firstx = x[i], lastx = x[i+1];
        Int_t firstxbin = h2D[1][1]->GetXaxis()->FindFixBin(firstx);
        Int_t lastxbin = h2D[1][1]->GetXaxis()->FindFixBin(lastx);
        hdedx_py = h2D[1][1]->ProjectionY("_py", firstxbin, lastxbin, "");
        //hdedx_py->SetAxisRange(0,20,"X");
        gPad->SetLogy();
        hdedx_py->Draw();
        legend->AddEntry(hdedx_py,Form("p/q (%f-%f) [GeV/c]",x[i],x[i+1]),"lp");
        legend->Draw();
        c1->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/PID/%s_py_%f_%f.png",name[12].Data(),firstx,lastx));
    }

    file->GetObject("hist_reverseBeta_after", h2D[1][2]);
    for(int i=0;i<75;i++)
    {
        legend = new TLegend(0.6,0.7,0.9,0.9);
        c1 = new TCanvas();
    
        Double_t firstx = x[i], lastx = x[i+1];
        Int_t firstxbin = h2D[1][2]->GetXaxis()->FindFixBin(firstx);
        Int_t lastxbin = h2D[1][2]->GetXaxis()->FindFixBin(lastx);
        hbeta_py = h2D[1][2]->ProjectionY("_py", firstxbin, lastxbin, "");
        //hbeta_py->SetAxisRange(0,2.5,"X");
        gPad->SetLogy();
        hbeta_py->Draw();
        legend->AddEntry(hbeta_py,Form("p/q (%f-%f) [GeV/c]",x[i],x[i+1]),"lp");
        legend->Draw();
        c1->SaveAs(Form("/home/ubuntu/folder/1Myresults/isobar_2018/RuRu/PID/%s_py_%f_%f.png",name[13].Data(),firstx,lastx));
    }
    
    
}
int plots()
{
    gStyle->SetOptStat(0);
    //plot1D();
    //plot2D();
    plotPt();
    //pid();

    return 0;
}
