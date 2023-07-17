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

int i, j, k;
TH1F*hist_spectra[9];
const int config_cut = 2;
TString config[config_cut] = {"before", "after"};

TH1F*hist_mult[config_cut];
TH1F*hist_Vz[config_cut];
TH1F*hist_Eta[config_cut];
TH1F*hist_pHits[config_cut];
TH1F*hist_pHitsFit[config_cut];
TH1F*hist_pHitsMax[config_cut];
TH1F*hist_pFitPoss[config_cut];
TH1F*hist_DCAz[config_cut];
TH1F*hist_DCAmag[config_cut];
TH1F*hist_DCAxy[config_cut];
TH1F*hist_pDedxHits[config_cut];
TH2F*hist_Vxy[config_cut];
TH2F*hist_pDedx[config_cut];
TH2F*hist_reverseBeta[config_cut];
TH2F*hist_PhiPt[config_cut];
TH2F*hist_EtaPt[config_cut];
TH2F*hist_squaredMass[config_cut];

void build()
{
    for (i = 0; i < config_cut; i++)
    {
        hist_mult[i]=new TH1F(Form("hist_mult_%s",config[i].Data()),"reference multiplicity;RefMult",475,0,475);
        hist_Vz[i]=new TH1F(Form("hist_Vz_%s",config[i].Data()),"Vertex Z;Z [cm]",100,-60,60);
        hist_Eta[i]=new TH1F(Form("hist_Eta_%s",config[i].Data()),"Pseudorapidity;#eta",100,-2,2);
        hist_pHits[i]=new TH1F(Form("hist_pHits_%s",config[i].Data()),"Primary track hits;nHits ",50,0,50);
        hist_pHitsFit[i]=new TH1F(Form("hist_pHitsFit_%s",config[i].Data()),"Primary track hits fit;nHitsFit",50,0,50);
        hist_pHitsMax[i]=new TH1F(Form("hist_pHitsMax_%s",config[i].Data()),"Primary track hits max;nHitsMax",50,0,50);
        hist_pFitPoss[i]=new TH1F(Form("hist_pFitPoss_%s",config[i].Data()),"nHitsFit/nHitsPoss;nHitsFit/nHitsPoss",60,0.1,1.1);
        hist_DCAz[i]=new TH1F(Form("hist_DCAz_%s",config[i].Data()),"DCA z vertex;DCAz",100,-5,5);
        hist_DCAmag[i]=new TH1F(Form("hist_DCAmag_%s",config[i].Data()),"DCA magnitude;DCAmag",100,0,5);
        hist_DCAxy[i]=new TH1F(Form("hist_DCAxy_%s",config[i].Data()),"DCA XY vertex;DCAxy",100,-5,5);
        hist_pDedxHits[i]=new TH1F(Form("hist_pdedxHits_%s",config[i].Data()),"Number of hits for dE/dx measurments",50,0.1,50);
        hist_Vxy[i]=new TH2F(Form("hist_Vxy_%s",config[i].Data()),"Vertex XY;X, [cm];Y, [cm]",100,-3,3,100,-3,3);
        hist_pDedx[i]=new TH2F(Form("hist_pDedx_%s",config[i].Data()),"Energy loss vs rigidity;p/q [GeV/c];dE/dx, [KeV/cm]",1000,-3,3,1000,0,55);
        hist_reverseBeta[i]=new TH2F(Form("hist_reverseBeta_%s",config[i].Data()),"Reverse Beta velocity vs rigidity;p/q, [GeV/c];1/#beta, [1/c]",1000,-4,4,1000,0.7,4);
        hist_PhiPt[i]=new TH2F(Form("hist_PhiPt_%s",config[i].Data()),"Azimuthal angle vs transverse momenta;p_{T}, (GeV/c);Phi, [rad]",1000,0,6,1000,3.5,3.5);
        hist_EtaPt[i]=new TH2F(Form("hist_EtaPt_%s",config[i].Data()),"Pseudorapidity vs transverse momenta;p_{T}, [GeV/c]",1000,0,7,1000,-2.6,2.6);
        hist_squaredMass[i]=new TH2F(Form("hist_squaredMass_%s",config[i].Data()),"Squared Mass;p_{T}/q, (GeV/c);m^{2}, (GeV)^{2}",1000,-3,3,1000,-0.5,1.5);
    }
}

void build_pt()
{
    TString centc[9]={"0-5","5-10","10-20","20-30","30-40","40-50","50-60","60-70","70-80"};
    for (k = 0; k < 9; k++)
    {
        hist_spectra[k] = new TH1F(Form("hist_spectra_cent%i", k),
                                   Form("p_{T} spectra for %s centrality", centc[k].Data()), 100, 0, 6);
    }
}
