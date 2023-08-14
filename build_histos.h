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
//TH1D*hist_spectra[9];
TH1D*hist_pip[9];
TH1D*hist_pim[9];
TH1D*hist_kap[9];
TH1D*hist_kam[9];
TH1D*hist_pr[9];
TH1D*hist_apr[9];
const int config_cut = 2;
TString config[config_cut] = {"before", "after"};

TH1D*hist_mult[config_cut];
TH1D*hist_Vz[config_cut];
TH1D*hist_Eta[config_cut];
TH1D*hist_pHits[config_cut];
TH1D*hist_pHitsFit[config_cut];
TH1D*hist_pHitsMax[config_cut];
TH1D*hist_pFitPoss[config_cut];
TH1D*hist_DCAz[config_cut];
TH1D*hist_DCAmag[config_cut];
TH1D*hist_DCAxy[config_cut];
TH1D*hist_pDedxHits[config_cut];
TH2D*hist_Vxy[config_cut];
TH2D*hist_pDedx[config_cut];
TH2D*hist_reverseBeta[config_cut];
TH2D*hist_PhiPt[config_cut];
TH2D*hist_EtaPt[config_cut];
TH2D*hist_squaredMass[config_cut];

void build()
{
    for (i = 0; i < config_cut; i++)
    {
        hist_mult[i]=new TH1D(Form("hist_mult_%s",config[i].Data()),Form("reference multiplicity %s cuts;RefMult",config[i].Data()),475,0,475);
        hist_Vz[i]=new TH1D(Form("hist_Vz_%s",config[i].Data()),Form("Vertex Z %s cuts;Z [cm]",config[i].Data()),100,-60,60);
        hist_Eta[i]=new TH1D(Form("hist_Eta_%s",config[i].Data()),Form("Pseudorapidity %s cuts;#eta",config[i].Data()),100,-2,2);
        hist_pHits[i]=new TH1D(Form("hist_pHits_%s",config[i].Data()),Form("Primary track hits %s cuts;nHits ",config[i].Data()) ,50,0,50);
        hist_pHitsFit[i]=new TH1D(Form("hist_pHitsFit_%s",config[i].Data()),Form("Primary track hits fit %s cuts;nHitsFit",config[i].Data()),50,0,50);
        hist_pHitsMax[i]=new TH1D(Form("hist_pHitsMax_%s",config[i].Data()),Form("Primary track hits max %s cuts;nHitsMax",config[i].Data()),50,0,50);
        hist_pFitPoss[i]=new TH1D(Form("hist_pFitPoss_%s",config[i].Data()),Form("nHitsFit/nHitsPoss %s cuts;nHitsFit/nHitsPoss",config[i].Data()),60,0.1,1.1);
        hist_DCAz[i]=new TH1D(Form("hist_DCAz_%s",config[i].Data()),Form("DCA z vertex %s cuts;DCAz",config[i].Data()),100,-5,5);
        hist_DCAmag[i]=new TH1D(Form("hist_DCAmag_%s",config[i].Data()),Form("DCA magnitude %s cuts;DCA",config[i].Data()),100,0,3.5);
        hist_DCAxy[i]=new TH1D(Form("hist_DCAxy_%s",config[i].Data()),Form("DCA XY vertex %s cuts;DCAxy",config[i].Data()),100,-5,5);
        hist_pDedxHits[i]=new TH1D(Form("hist_pdedxHits_%s",config[i].Data()),Form("Number of hits for dE/dx measurments %s cuts",config[i].Data()),50,0.1,50);
        hist_Vxy[i]=new TH2D(Form("hist_Vxy_%s",config[i].Data()),Form("Vertex XY %s cuts;;X, [cm];Y, [cm]",config[i].Data()),100,-3,3,100,-3,3);
        hist_pDedx[i]=new TH2D(Form("hist_pDedx_%s",config[i].Data()),Form("Energy loss vs rigidity %s cuts;p/q [GeV/c];dE/dx, [KeV/cm]",config[i].Data()),1000,-3,3,1000,0,55);
        hist_reverseBeta[i]=new TH2D(Form("hist_reverseBeta_%s",config[i].Data()),Form("Reverse Beta velocity vs rigidity %s cuts;p/q, [GeV/c];1/#beta, [1/c]",config[i].Data()),1000,-4,4,1000,0.7,4);
        hist_PhiPt[i]=new TH2D(Form("hist_PhiPt_%s",config[i].Data()),Form("Azimuthal angle vs transverse momenta %s cuts;p_{T}, (GeV/c);Phi, [rad]",config[i].Data()),1000,0,6.,1000,3.5,3.5);
        hist_EtaPt[i]=new TH2D(Form("hist_EtaPt_%s",config[i].Data()),Form("Pseudorapidity vs transverse momenta %s cuts;p_{T}, [GeV/c]",config[i].Data()),1000,0,7,1000,-2.6,2.6);
        hist_squaredMass[i]=new TH2D(Form("hist_squaredMass_%s",config[i].Data()),Form("Squared Mass %s cuts;;p_{T}/q, (GeV/c);m^{2}, (GeV)^{2}",config[i].Data()),1000,-3,3,1000,-0.5,1.5);
    }
}

void build_pt()
{
    TString centc[9]={"70-80", "60-70", "50-60", "40-50", "30-40", "20-30", "10-20", "5-10", "0-5"};
    for (k = 0; k < 9; k++)
    {
        //hist_spectra[k] = new TH1D(Form("hist_spectra_cent%i", k),
        //                           Form("p_{T} spectra for %s centrality", centc[k].Data()), 60, 0, 6);
        //hist_spectra[k] = new TH1D(Form("hist_spectra_cent%i", k),
        //                           "p_{T} spectra for each centrality", 60, 0, 6);
        hist_pip[k] = new TH1D(Form("hist_pip_cent%i", k),
                                   "pip p_{T} spectra for each centrality", 60, 0, 6);
        hist_pim[k] = new TH1D(Form("hist_pim_cent%i", k),
                                   "pim p_{T} spectra for each centrality", 60, 0, 6);    
        hist_kap[k] = new TH1D(Form("hist_kap_cent%i", k),
                                   "kap p_{T} spectra for each centrality", 60, 0, 6);
        hist_kam[k] = new TH1D(Form("hist_kam_cent%i", k),
                                   "kam p_{T} spectra for each centrality", 60, 0, 6);
        hist_pr[k] = new TH1D(Form("hist_pr_cent%i", k),
                                   "pr p_{T} spectra for each centrality", 60, 0, 6); 
        hist_apr[k] = new TH1D(Form("hist_pm_cent%i", k),
                                   "apr p_{T} spectra for each centrality", 60, 0, 6);                       
    }
}
