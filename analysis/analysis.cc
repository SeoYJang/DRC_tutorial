#include "RootInterface.h"
#include "RecoInterface.h"
#include "DRsimInterface.h"
#include "functions.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <iostream>
#include <string>
#include "Riostream.h"

std::string to_string_with_precision(float a, int n) {

  std::ostringstream out;
  out.precision(n);
  out<<std::fixed<<a;
  return out.str();
  
}

int main(int argc, char* argv[]) {
  TString filename = argv[1];
  float low = std::stof(argv[2]);
  float high = std::stof(argv[3]);
  float en_val = std::stof(argv[4]);
  TString material = argv[5];
  TString particle = argv[6];

  int mNum = 24;

  gStyle->SetOptFit(1);

  RootInterface<DRsimInterface::DRsimEventData>* drInterface = new RootInterface<DRsimInterface::DRsimEventData>(std::string(filename)+".root", true);
  drInterface->set("DRsim","DRsimEventData");
  //drInterface->GetChain("DRsim");

  int binnumber = 0;
  while (low + binnumber * 0.35 < high) {
    binnumber++;
  }

  TH1F* tEdep = new TH1F("totEdep",";MeV;Evt",100,low*1000.,high*1000.);
  tEdep->Sumw2(); tEdep->SetLineColor(kRed); tEdep->SetLineWidth(2);
  TH1F* tHit_C = new TH1F("Hit_C",";# of p.e.;Evt",200,low*40,high*80);
  tHit_C->Sumw2(); tHit_C->SetLineColor(kBlue); tHit_C->SetLineWidth(2);
  TH1F* tHit_S = new TH1F("Hit_S",";# of p.e.;Evt",200,low*650,high*1100);
  tHit_S->Sumw2(); tHit_S->SetLineColor(kRed); tHit_S->SetLineWidth(2);
  TH1F* tE_C = new TH1F("E_C",";;",binnumber,low,low + binnumber*0.35);
  tE_C->Sumw2(); tE_C->SetLineColor(kBlue); tE_C->SetLineWidth(2);
  TH1F* tE_S = new TH1F("E_S",";;",binnumber,low,low + binnumber*0.35);
  tE_S->Sumw2(); tE_S->SetLineColor(kRed); tE_S->SetLineWidth(2);
  TH1F* tE_Scorr = new TH1F("E_Scorr",";;",binnumber,low,low + binnumber*0.35);
  tE_Scorr->Sumw2(); tE_Scorr->SetLineColor(kRed); tE_Scorr->SetLineWidth(2);
  TH1F* tE_Scorr_30 = new TH1F("E_Scorr_30",";;",binnumber,low,low + binnumber*0.35);
  tE_Scorr_30->Sumw2(); tE_Scorr_30->SetLineColor(kRed+2); tE_Scorr_30->SetLineWidth(2);
  TH1F* tE_SC = new TH1F("E_SC",";;",binnumber*2,2.*low,2*(low + binnumber*0.35));
  tE_SC->Sumw2(); tE_SC->SetLineColor(kBlack); tE_SC->SetLineWidth(2);
  TH1F* tE_DR = new TH1F("E_DR",";Evt",binnumber,low,low + binnumber*0.35);
  tE_DR->Sumw2(); tE_DR->SetLineColor(kBlack); tE_DR->SetLineWidth(2);
  TH1F* tE_DRcorr = new TH1F("E_DRcorr",";GeV;Evt",binnumber,low,low + binnumber*0.35);
  tE_DRcorr->Sumw2(); tE_DRcorr->SetLineColor(kBlack); tE_DRcorr->SetLineWidth(2);
  TH1F* tE_DRcorr_30 = new TH1F("E_DRcorr_30",";GeV;Evt",binnumber,low,low + binnumber*0.35);
  tE_DRcorr_30->Sumw2(); tE_DRcorr_30->SetLineColor(kGray+1); tE_DRcorr_30->SetLineWidth(2);
  TH1F* tP_leak = new TH1F("Pleak",";MeV;Evt",100,0.,1000.*high);
  tP_leak->Sumw2(); tP_leak->SetLineWidth(2);
  TH1F* tP_leak_nu = new TH1F("Pleak_nu",";MeV;Evt",100,0.,1000.*high);
  tP_leak_nu->Sumw2(); tP_leak_nu->SetLineWidth(2);
  TH1F* tDepth = new TH1F("depth",";m;Evt",70,-1.0,3.0);
  tDepth->Sumw2(); tDepth->SetLineWidth(2); tDepth->SetLineColor(kGray+1);
  TH1F* tDepth_30 = new TH1F("depth_30",";m;Evt",70,-1.0,3.0);
  tDepth_30->Sumw2(); tDepth_30->SetLineWidth(2); tDepth_30->SetLineColor(kBlack);

  TH1F* tP_leak_theta = new TH1F("Pleak_theta",";degree;MeV",180,-180.,180.);
  tP_leak_theta->Sumw2(); tP_leak_theta->SetLineWidth(2); tP_leak_theta->SetStats(0);
  TH1F* tP_leak_phi = new TH1F("Pleak_phi",";degree;MeV",180,-180.,180.);
  tP_leak_phi->Sumw2(); tP_leak_phi->SetLineWidth(2); tP_leak_phi->SetStats(0);

  TH2F* tScvsC = new TH2F("SvsC", ";E_{Scorr};E_{C}", 100, low, high, 100, low, high);
  tScvsC->Sumw2(); tScvsC->SetStats(0);

  TH1F* tT_C = new TH1F("time_C",";ns;p.e.",700,0.,70.);
  tT_C->Sumw2(); tT_C->SetLineColor(kBlue); tT_C->SetLineWidth(2);
  TH1F* tT_S = new TH1F("time_S",";ns;p.e.",700,0.,70.);
  tT_S->Sumw2(); tT_S->SetLineColor(kRed); tT_S->SetLineWidth(2);
  TH1F* tWav_S = new TH1F("wavlen_S",";nm;p.e.",120,300.,900.);
  tWav_S->Sumw2(); tWav_S->SetLineColor(kRed); tWav_S->SetLineWidth(2);
  TH1F* tWav_C = new TH1F("wavlen_C",";nm;p.e.",120,300.,900.);
  tWav_C->Sumw2(); tWav_C->SetLineColor(kBlue); tWav_C->SetLineWidth(2);
  TH1F* tNhit_S = new TH1F("nHits_S",";n",200,0.,200.);
  tNhit_S->Sumw2(); tNhit_S->SetLineColor(kRed); tNhit_S->SetLineWidth(2);
  TH1F* tNhit_C = new TH1F("nHits_C",";n",50,0.,50.);
  tNhit_C->Sumw2(); tNhit_C->SetLineColor(kBlue); tNhit_C->SetLineWidth(2);
  TH1F* oEdep = new TH1F("oEdep",";MeV;Evt",100,low*1000.,high*1000.);
  oEdep->Sumw2(); oEdep->SetLineColor(kBlack); oEdep->SetLineWidth(2);
  TH1I* Chit = new TH1I("C_Hit","",100,0.,3000.);
  Chit->Sumw2(); Chit->SetLineColor(kBlue); Chit->SetLineWidth(2);
  TH1I* Shit = new TH1I("S_Hit","",100,0.,40000.);
  Shit->Sumw2(); Shit->SetLineColor(kRed); Shit->SetLineWidth(2); 

  TH2D* t2DRecoE_S = new TH2D("2D_RecoE_S", "", 7, -0.5, 6.5, 7, -0.5, 6.5); t2DRecoE_S->Sumw2(); t2DRecoE_S->SetStats(0);
  TH2D* t2DRecoE_C = new TH2D("2D_RecoE_C", "", 7, -0.5, 6.5, 7, -0.5, 6.5); t2DRecoE_C->Sumw2(); t2DRecoE_C->SetStats(0);

  TH2D* t2DRecoE_S_frac = new TH2D("2D_RecoE_S_frac", "", 7, -0.5, 6.5, 7, -0.5, 6.5); t2DRecoE_S_frac->Sumw2(); t2DRecoE_S_frac->SetStats(0);
  TH2D* t2DRecoE_C_frac = new TH2D("2D_RecoE_C_frac", "", 7, -0.5, 6.5, 7, -0.5, 6.5); t2DRecoE_C_frac->Sumw2(); t2DRecoE_C_frac->SetStats(0);

  TH2F* t2DDES = new TH2F("2D Depth Scint", "", 70, -1.0, 3.0, 50, low, high); t2DDES->Sumw2(); t2DDES->SetStats(0);
  TH2F* t2DDEC = new TH2F("2D Depth Ceren", "", 70, -1.0, 3.0, 50, low, high); t2DDEC->Sumw2(); t2DDEC->SetStats(0);

  TH2D* t2DhitC = new TH2D("2D_Hit_C", "", 462, -346.5, 346.5, 462, -346.5, 346.5); t2DhitC->Sumw2(); t2DhitC->SetStats(0);
  TH2D* t2DhitS = new TH2D("2D_Hit_S", "", 462, -346.5, 346.5, 462, -346.5, 346.5); t2DhitS->Sumw2(); t2DhitS->SetStats(0);

  TH1D* tS_lateral = new TH1D("S_lateral", "", 231, 0., 346.5); tS_lateral->Sumw2(); tS_lateral->SetStats(0);
  TH1D* tC_lateral = new TH1D("C_lateral", "", 231, 0., 346.5); tC_lateral->Sumw2(); tC_lateral->SetStats(0);

  std::string material_base = std::string(material).substr(0, std::string(material).find("_"));

  std::string filename_var = "/u/user/syjang/DRC_tutorial/results/Variables.csv";
  std::ifstream in;
  std::string mat;
  float fCalibC, fCalibS, Cs, Ss, es, al, mtmax, rl, ch, Cpi;
  float Cthres, Cth, chi, Cpithres;

  float effspeed, attlen, matTmax, radilen;

  in.open(filename_var, std::ios::in);
  while (true) {
    in >> mat >> Cth >> Cs >> Ss >> es >> al >> mtmax >> rl >> ch >> Cpi;
    if (mat == material) {
      Cthres = Cth;
      fCalibC = Cs;
      fCalibS = Ss;      
      effspeed = es;
      attlen = al;
      matTmax = mtmax;
      radilen = rl;
      chi = ch;
      Cpithres = Cpi;
    }
    if (!in.good()) break;
  }

  if (particle == "pi") Cthres = Cpithres;

  std::vector<float> E_Ss,E_Cs,E_Sscorr,E_Sscorr_30;

  //int drawplots = 0;

  TCanvas* c = new TCanvas("c","");

  int entry_count = 0;
  unsigned int entries = drInterface->entries();
  if (entries > 3000) entries = 3000;
  while (drInterface->numEvt() < entries) {
    if (drInterface->numEvt() % 100 == 0) printf("Analyzing %dth event ...\n", drInterface->numEvt());

    DRsimInterface::DRsimEventData drEvt;
    drInterface->read(drEvt);

    float Edep = 0.; float fEdep = 0.;
    int moduleNum = 0;

    float rE_C, rE_S;
    float rE_C_module[49], rE_S_module[49];
    float sE_C, sE_S;
    float sE_C_module[49] = {0.}; float sE_S_module[49] = {0.};
    sE_C = 0.; sE_S = 0.;

    for (auto edepItr = drEvt.Edeps.begin(); edepItr != drEvt.Edeps.end(); ++edepItr) {
      auto edep = *edepItr;
      Edep += edep.Edep;
      moduleNum = edep.ModuleNum;
      if (moduleNum==mNum) fEdep += edep.Edep;
    }
    tEdep->Fill(Edep);
    oEdep->Fill(fEdep);

    TH1F* tT_max = new TH1F("tmax","",600,10.,70.); tT_max->Sumw2(); tT_max->SetLineColor(kRed);
    TH1F* tC_max = new TH1F("cmax","",600,10.,70.); tC_max->Sumw2(); tC_max->SetLineColor(kBlue);

    float Pleak = 0.;
    float Eleak_nu = 0.;
    float Pleak_theta = 0., Pleak_phi = 0.;
    for (auto leak : drEvt.leaks) {
      TLorentzVector leak4vec;
      leak4vec.SetPxPyPzE(leak.px,leak.py,leak.pz,leak.E);
      if ( std::abs(leak.pdgId)==12 || std::abs(leak.pdgId)==14 || std::abs(leak.pdgId)==16 ) {
        Eleak_nu += leak4vec.P();
      } else {
        Pleak += leak4vec.P();
        if (leak4vec.Pz() >= 0) {
          Pleak_theta = TMath::ATan(leak4vec.Py()/leak4vec.Pz())*180/TMath::Pi();
          Pleak_phi   = TMath::ATan(leak4vec.Px()/leak4vec.Pz())*180/TMath::Pi();
        } else {
          Pleak_theta = (leak4vec.Py() >= 0) ? (TMath::ATan(leak4vec.Py()/leak4vec.Pz())*180/TMath::Pi()) + 180 : (TMath::ATan(leak4vec.Py()/leak4vec.Pz())*180/TMath::Pi() - 180);
          Pleak_phi   = (leak4vec.Px() >= 0) ? (TMath::ATan(leak4vec.Px()/leak4vec.Pz())*180/TMath::Pi()) + 180 : (TMath::ATan(leak4vec.Px()/leak4vec.Pz())*180/TMath::Pi() - 180);
        }
        tP_leak_theta->Fill(Pleak_theta, leak4vec.P()/entries);
        tP_leak_phi->Fill(Pleak_phi, leak4vec.P()/entries);
      }
    }
    tP_leak->Fill(Pleak);
    tP_leak_nu->Fill(Eleak_nu);

    int nHitC = 0; int nHitS = 0;
    int fC_hits = 0; int fS_hits = 0;
    int fC_hits_module[49] = {0}; int fS_hits_module[49] = {0};
    int sum;

    for (auto tower = drEvt.towers.begin(); tower != drEvt.towers.end(); ++tower) {

      moduleNum = tower->ModuleNum;
      rE_C = 0.;
      rE_S = 0.;
      rE_C_module[moduleNum] = 0.;
      rE_S_module[moduleNum] = 0.;

      for (auto sipm = tower->SiPMs.begin(); sipm != tower->SiPMs.end(); ++sipm) {
        int plateNum = sipm->x; int fiberNum = sipm->y; 
        double distance = 0;
        if ( RecoInterface::IsCerenkov(sipm->x,sipm->y) ) {
          tNhit_C->Fill(sipm->count);
          sum = 0;
          fC_hits_module[moduleNum] = 0;
          for (const auto timepair : sipm->timeStruct) {
            tT_C->Fill(timepair.first.first+0.05,timepair.second);
            tC_max->Fill(timepair.first.first+0.05,timepair.second);
            if (timepair.first.first < Cthres) {
              nHitC += timepair.second;
              sum += timepair.second;
              fC_hits_module[moduleNum] += timepair.second;
              if (moduleNum==mNum) {
                fC_hits += timepair.second;
              }
              t2DhitC->Fill((66*(moduleNum%7)+fiberNum)*1.5 - 346.5, (66*(moduleNum/7)+plateNum)*1.5 - 346.5, float(timepair.second) / float(entries));
              distance = sqrt(pow((66*(moduleNum%7)+fiberNum)*1.5 - 346.5,2) + pow((66*(moduleNum/7)+plateNum)*1.5 - 346.5,2));
              tC_lateral->Fill(distance, float(timepair.second) / float(entries));
            }
          }

          for (const auto wavpair : sipm->wavlenSpectrum) {
            tWav_C->Fill(wavpair.first.first,wavpair.second);
          }

          rE_C += (float)sum / fCalibC;
          rE_C_module[moduleNum] += (float)sum / fCalibC;
          

        } else {
          tNhit_S->Fill(sipm->count);
          nHitS += sipm->count; 
          rE_S += (float)sipm->count / fCalibS;
          rE_S_module[moduleNum] += (float)sipm->count / fCalibS;
          if (moduleNum==mNum) {
            fS_hits += sipm->count;
            fS_hits_module[moduleNum] += sipm->count;
          }
          t2DhitS->Fill((66*(moduleNum%7)+fiberNum)*1.5 - 346.5, (66*(moduleNum/7)+plateNum)*1.5 - 346.5, float(sipm->count) / float(entries));
          distance = sqrt(pow((66*(moduleNum%7)+fiberNum)*1.5 - 346.5,2) + pow((66*(moduleNum/7)+plateNum)*1.5 - 346.5,2));
          tS_lateral->Fill(distance, float(sipm->count) / float(entries));
          for (const auto timepair : sipm->timeStruct) {
            tT_S->Fill(timepair.first.first+0.05,timepair.second);
            tT_max->Fill(timepair.first.first+0.05,timepair.second);
          }
          for (const auto wavpair : sipm->wavlenSpectrum) {
            tWav_S->Fill(wavpair.first.first,wavpair.second);
          }
        }
      }
      sE_C += rE_C;
      sE_S += rE_S;
      sE_C_module[moduleNum] += rE_C_module[moduleNum];
      sE_S_module[moduleNum] += rE_S_module[moduleNum];
    }

    float T_max = tT_max->GetBinCenter( tT_max->GetMaximumBin() );
    float depth = (matTmax + radilen*(1./effspeed - 1./0.3) - T_max)/(1./effspeed - 1./0.3);

    float C_max = tC_max->GetBinCenter( tC_max->GetMaximumBin() );

    float sE_Scorr = sE_S*std::exp((radilen-depth)/attlen);

    // Get T_max as 30% leading edge of single timing distribution tT_max
    int maxBin = tT_max->GetMaximumBin();
    float maxValue = tT_max->GetBinContent(maxBin);
    float threshold = 0.3 * maxValue;
    for (int bin = maxBin; bin > 1; --bin) {
      if (tT_max->GetBinContent(bin) < threshold) {
        T_max = tT_max->GetBinCenter(bin+1);
        break;
      }
    }

    int maxBin_C = tC_max->GetMaximumBin();
    float maxValue_C = tC_max->GetBinContent(maxBin_C);
    float threshold_C = 0.3 * maxValue_C;
    for (int bin = maxBin_C; bin > 1; --bin) {
      if (tC_max->GetBinContent(bin) < threshold_C) {
        C_max = tC_max->GetBinCenter(bin+1);
        break;
      }
    }

    float matTmax_30 = 0.;
    if (material_base == "copper") matTmax_30 = 18.95;
    if (material_base == "brass") matTmax_30 = 18.95;
    if (material_base == "iron") matTmax_30 = 18.85;
    if (material_base == "lead") matTmax_30 = 19.25;
    if (material_base == "tungsten") matTmax_30 = 19.25;

    float depth_30 = (matTmax_30 + radilen*(1./effspeed - 1./0.3) - T_max)/(1./effspeed - 1./0.3);

    float sE_Scorr_30 = sE_S*std::exp((radilen-depth_30)/attlen);

    /*if (drawplots < 100) {
      drawplots++;
      TCanvas* c1 = new TCanvas("c1","");
      c1->cd();
      tT_max->Draw("Hist");
      c1->SaveAs(filename+"_Tmax"+std::to_string(drawplots)+".pdf");
      delete c1;
    }*/

    delete tT_max;
    delete tC_max;
    
    E_Cs.push_back(sE_C);
    E_Ss.push_back(sE_S);
    E_Sscorr.push_back(sE_Scorr);
    E_Sscorr_30.push_back(sE_Scorr_30);

    tHit_C->Fill(nHitC);
    tHit_S->Fill(nHitS);
    Chit->Fill(fC_hits);
    Shit->Fill(fS_hits);
    tE_C->Fill(sE_C);
    tE_S->Fill(sE_S);
    tE_Scorr->Fill(sE_Scorr);
    tE_Scorr_30->Fill(sE_Scorr_30);
    tE_SC->Fill(sE_C + sE_S);

    tScvsC->Fill(sE_Scorr, sE_C);

    tE_DR->Fill(functions::E_DR(sE_C,sE_S,chi));
    tE_DRcorr->Fill(functions::E_DR(sE_C,sE_Scorr,chi));
    tE_DRcorr_30->Fill(functions::E_DR(sE_C,sE_Scorr_30,chi));
    tDepth->Fill(depth);
    tDepth_30->Fill(depth_30);

    t2DDES->Fill(depth_30, sE_S);

    for (int i=0; i<49; i++) {
      t2DRecoE_C->Fill(i%7, i/7, sE_C_module[i]/entries);
      t2DRecoE_S->Fill(i%7, i/7, sE_S_module[i]/entries);
      t2DRecoE_S_frac->Fill(i%7, i/7, (sE_S_module[i]/sE_S)/entries * 100.);
      t2DRecoE_C_frac->Fill(i%7, i/7, (sE_C_module[i]/sE_C)/entries * 100.);
    }

    entry_count++;
  } // event loop

  for (int i=0; i<49; i++) {
    t2DRecoE_C->Fill(i%7, i/7, 0.0000001);
    t2DRecoE_S->Fill(i%7, i/7, 0.0000001);
    t2DRecoE_S_frac->Fill(i%7, i/7, 0.0000001);
    t2DRecoE_C_frac->Fill(i%7, i/7, 0.0000001);
  }

  //gStyle->SetStatX(0.9); gStyle->SetStatW(0.23);
  //gStyle->SetStatY(0.9); gStyle->SetStatH(0.18);

  TFile* outputRoot = new TFile(filename+"_plots.root", "RECREATE");
  outputRoot->cd();

  c->cd();

  c->cd();
  t2DDES->Draw("COL"); t2DDES->SetMarkerStyle(kFullDotMedium);
  t2DDES->GetXaxis()->SetRangeUser(-0.5, 2.5);
  c->SaveAs(filename+"_2DdepthScint.pdf");

  c->cd();
  t2DDEC->Draw("COL"); t2DDEC->SetMarkerStyle(kFullDotMedium);
  t2DDEC->GetXaxis()->SetRangeUser(-0.5, 2.5);
  c->SaveAs(filename+"_2DdepthCeren.pdf");

  int t2Dxbins = t2DDES->GetNbinsX();
  float tdepval[t2Dxbins], tdeperr[t2Dxbins];
  float tDEx[t2Dxbins], tDExerr[t2Dxbins];
  int tcount = 0;
  int SgraphN = 0;

  TGraphErrors* tDEgraph = new TGraphErrors();
  
  for (int i=0; i<t2DDES->GetNbinsX(); i++) {
    TH1F* tdepth_bin = new TH1F("tdepth","",t2DDES->GetNbinsY(),low,high);
    for (int j=0; j<t2DDES->GetNbinsY(); j++) {
      if (t2DDES->GetBinContent(i+1, j+1) != 0) {
        tdepth_bin->Fill(t2DDES->GetYaxis()->GetBinCenter(j+1),t2DDES->GetBinContent(i+1, j+1));
        tcount += t2DDES->GetBinContent(i+1, j+1);
      }
    }
    if (tcount != 0) {
      tdepval[SgraphN] = tdepth_bin->GetMean();
      tdeperr[SgraphN] = tdepth_bin->GetRMS()/sqrt(tcount);
    //  std::cout << "Bin Num " << i << "  Mean Error : "<< tdepth_bin->GetMean() << " and " << tdepth_bin->GetRMS()/sqrt(tcount) << std::endl;
    } else {
      delete tdepth_bin;
      continue;
    }
    tDEx[SgraphN] = t2DDES->GetXaxis()->GetBinCenter(i+1);
    tDExerr[SgraphN] = 0.0285;
    tDEgraph->SetPoint(SgraphN, tDEx[SgraphN], tdepval[SgraphN]);
    tDEgraph->SetPointError(SgraphN, tDExerr[SgraphN], tdeperr[SgraphN]);
    SgraphN++;
    tcount = 0;
    delete tdepth_bin;
  }

  TF1 *f1 = new TF1("f1", "exp([0] + [1]*x)");
  tDEgraph->Fit(f1, "", "", 0.1, 1.0);
  c->cd();
  tDEgraph->Draw("AP");
  tDEgraph->SetTitle(""); 
  tDEgraph->GetXaxis()->SetRangeUser(-0.5, 2.5);
  tDEgraph->GetYaxis()->SetRangeUser(low, high); 

  c->SaveAs(filename+"_AttenLengthplotS.pdf");

  int t2Cxbins = t2DDEC->GetNbinsX();
  float tCdepval[t2Cxbins], tCdeperr[t2Cxbins];
  float tCDEx[t2Cxbins], tCDExerr[t2Cxbins];
  int tCount = 0;
  int CgraphN = 0;

  TGraphErrors* tCEgraph = new TGraphErrors();
  
  for (int i=0; i<t2DDEC->GetNbinsX(); i++) {
    TH1F* tdepth_bin = new TH1F("tdepth","",t2DDEC->GetNbinsY(),low,high);
    for (int j=0; j<t2DDEC->GetNbinsY(); j++) {
      if (t2DDEC->GetBinContent(i+1, j+1) != 0) {
        tdepth_bin->Fill(t2DDEC->GetYaxis()->GetBinCenter(j+1),t2DDEC->GetBinContent(i+1, j+1));
        tCount += t2DDEC->GetBinContent(i+1, j+1);
      }
    }
    if (tCount != 0) {
      tCdepval[CgraphN] = tdepth_bin->GetMean();
      tCdeperr[CgraphN] = tdepth_bin->GetRMS()/sqrt(tCount);
    //  std::cout << "Bin Num " << i << "  Mean Error : "<< tdepth_bin->GetMean() << " and " << tdepth_bin->GetRMS()/sqrt(tcount) << std::endl;
    } else {
      delete tdepth_bin;
      continue;
    }
    tCDEx[CgraphN] = t2DDEC->GetXaxis()->GetBinCenter(i+1);
    tCDExerr[CgraphN] = 0.0285;
    tCEgraph->SetPoint(CgraphN, tCDEx[CgraphN], tCdepval[CgraphN]);
    tCEgraph->SetPointError(CgraphN, tCDExerr[CgraphN], tCdeperr[CgraphN]);
    CgraphN += 1;
    tCount = 0;
    delete tdepth_bin;
  }

  TF1 *f2 = new TF1("f2", "exp([0] + [1]*x)");
  //f2->SetParLimits(1, 0.00, 0.30);
  f2->SetLineColor(kRed);
  tCEgraph->Fit(f2, "MR", "", 0.1, 1.0);
  c->cd();
  tCEgraph->Draw("AP");
  tCEgraph->SetTitle(""); 
  tCEgraph->GetXaxis()->SetRangeUser(-0.5, 2.5);
  tCEgraph->GetYaxis()->SetRangeUser(low, high); 

  c->SaveAs(filename+"_AttenLengthplotC.pdf");

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  c->cd();
  tE_S->SetLineColor(kPink+2);
  tE_Scorr_30->SetLineColor(kRed);

  tE_Scorr_30->Draw("Hist"); c->Update();
  tE_S->Draw("Hist&sames"); c->Update();
  tE_C->Draw("Hist&sames");

  TLegend* leg_SSC = new TLegend(0.15, 0.65, 0.45, 0.85);
  std::string tE_S_stat = "#splitline{#color[902]{Scintllation}}{Mean : " + to_string_with_precision(tE_S->GetMean(), 3) + ", RMS : " + to_string_with_precision(tE_S->GetRMS(), 3) + "}";
  std::string tE_Scorr_30_stat = "#splitline{#color[632]{Scintllation (Atten.)}}{Mean : " + to_string_with_precision(tE_Scorr_30->GetMean(), 3) + ", RMS : " + to_string_with_precision(tE_Scorr_30->GetRMS(), 3) + "}";
  std::string tE_C_stat = "#splitline{#color[4]{Cerenkov}}{Mean : " + to_string_with_precision(tE_C->GetMean(), 3) + ", RMS : " + to_string_with_precision(tE_C->GetRMS(), 3) + "}";
  leg_SSC->AddEntry(tE_S, tE_S_stat.c_str(), "l");
  leg_SSC->AddEntry(tE_Scorr_30, tE_Scorr_30_stat.c_str(), "l");
  leg_SSC->AddEntry(tE_C, tE_C_stat.c_str(), "l");
  leg_SSC->SetBorderSize(0);
  leg_SSC->Draw();

  c->SaveAs(filename+"_E_SScorrC_hist.pdf");

  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);

  c->cd();
  tDepth_30->Draw("Hist"); c->Update();
  c->SaveAs(filename+"_Depth_30.pdf");

  tEdep->Draw("Hist"); c->SaveAs(filename+"_Edep.pdf");
  oEdep->Draw("Hist"); c->SaveAs(filename+"_CenterEdep.pdf");

  tE_S->Write();
  tE_C->Write();
  tE_SC->Write();
  tHit_C->Write();
  tHit_S->Write();

  tEdep->Write();
  oEdep->Write();
  tP_leak->Write();
  tP_leak_nu->Write();
  tP_leak_phi->Write();
  tP_leak_theta->Write();
  tScvsC->Write();
  tT_C->Write();
  tT_S->Write();
  tWav_C->Write();
  tWav_S->Write();
  t2DhitC->Write();
  t2DhitS->Write();
  t2DRecoE_C->Write();
  t2DRecoE_S->Write();
  t2DRecoE_S_frac->Write();
  t2DRecoE_C_frac->Write();

  c->cd();
  tE_S->Draw("Hist"); c->Update();
  c->SaveAs(filename+"_ES_Hist.pdf");

  c->cd();
  tE_C->Draw("Hist"); c->Update();
  c->SaveAs(filename+"_EC_Hist.pdf");

  c->cd();
  tDepth->Draw("hist"); c->Update();
  TPaveStats* statsDepth = (TPaveStats*)c->GetPrimitive("stats");
  statsDepth->SetName("Depth");
  statsDepth->SetTextColor(kGray+1);
  statsDepth->SetX1NDC(.66); statsDepth->SetX2NDC(.9);
  statsDepth->SetY1NDC(.7); statsDepth->SetY2NDC(.9);

  c->cd();
  tDepth_30->Draw("hist&sames"); c->Update();
  TPaveStats* statsDepth_30 = (TPaveStats*)c->GetPrimitive("stats");
  statsDepth_30->SetName("Depth_30");
  statsDepth_30->SetTextColor(kBlack);
  statsDepth_30->SetX1NDC(.66); statsDepth_30->SetX2NDC(.9);
  statsDepth_30->SetY1NDC(.5); statsDepth_30->SetY2NDC(.7);
  c->SaveAs(filename+"_Depth_30.pdf");

  c->cd();
  tE_S->SetTitle("");
  tE_S->Draw("Hist"); c->Update();
  TPaveStats* statsE_S = (TPaveStats*)c->GetPrimitive("stats");
  statsE_S->SetName("Scint");
  statsE_S->SetTextColor(kRed);
  statsE_S->SetX1NDC(.66); statsE_S->SetX2NDC(.9);
  statsE_S->SetY1NDC(.5); statsE_S->SetY2NDC(.7);

  tE_C->Draw("Hist&sames"); c->Update();
  TPaveStats* statsE_C = (TPaveStats*)c->GetPrimitive("stats");
  statsE_C->SetName("Cerenkov");
  statsE_C->SetTextColor(kBlue);
  statsE_C->SetX1NDC(.66); statsE_C->SetX2NDC(.9);
  statsE_C->SetY1NDC(.7); statsE_C->SetY2NDC(.9);
  c->SaveAs(filename+"_EcsHist.pdf");

  c->cd();
  tE_Scorr->Draw("Hist"); c->Update();
  TPaveStats* statsE_Scorr = (TPaveStats*)c->GetPrimitive("stats");
  statsE_Scorr->SetName("Corr_bf");
  statsE_Scorr->SetTextColor(kRed);
  statsE_Scorr->SetX1NDC(.66); statsE_Scorr->SetX2NDC(.9);
  statsE_Scorr->SetY1NDC(.7); statsE_Scorr->SetY2NDC(.9);

  tE_Scorr_30->Draw("Hist&sames"); c->Update();
  TPaveStats* statsE_Scorr_30 = (TPaveStats*)c->GetPrimitive("stats");
  statsE_Scorr_30->SetName("S cpor");
  statsE_Scorr_30->SetTextColor(kRed);
  statsE_Scorr_30->SetX1NDC(.66); statsE_Scorr_30->SetX2NDC(.9);
  statsE_Scorr_30->SetY1NDC(.5); statsE_Scorr_30->SetY2NDC(.7);

  c->SaveAs(filename+"_EcsHist_30.pdf");

  TF1* grE_C = new TF1("Cfit","gaus",low,high); grE_C->SetLineColor(kBlue);
  TF1* grE_S = new TF1("Sfit","gaus",low,high); grE_S->SetLineColor(kRed);
  tE_C->SetOption("p"); tE_C->Fit(grE_C,"R+&same");
  tE_S->SetOption("p"); tE_S->Fit(grE_S,"R+&same");

  c->cd();
  tE_S->Draw(""); c->Update();
  statsE_S->SetX1NDC(.68); statsE_S->SetX2NDC(.95);
  statsE_S->SetY1NDC(.65); statsE_S->SetY2NDC(.95);
  c->SaveAs(filename+"_ES_Hist.pdf");

  c->cd();
  tE_C->Draw(""); c->Update();
  statsE_C->SetX1NDC(.68); statsE_C->SetX2NDC(.9);
  statsE_C->SetY1NDC(.65); statsE_C->SetY2NDC(.9);
  c->SaveAs(filename+"_EC_Hist.pdf");

  c->cd();
  tE_S->SetTitle("");
  tE_S->SetMarkerStyle(20);
  tE_S->SetMarkerColor(kRed);
  tE_S->Draw(""); c->Update();
  statsE_S->SetName("Scint");
  statsE_S->SetTextColor(kRed);
  statsE_S->SetX1NDC(.63); statsE_S->SetX2NDC(.9);
  statsE_S->SetY1NDC(.3); statsE_S->SetY2NDC(.6);

  tE_C->SetMarkerStyle(22);
  tE_C->SetMarkerColor(kBlue);
  tE_C->Draw("sames"); c->Update();
  statsE_C->SetName("Cerenkov");
  statsE_C->SetTextColor(kBlue);
  statsE_C->SetX1NDC(.63); statsE_C->SetX2NDC(.9);
  statsE_C->SetY1NDC(.6); statsE_C->SetY2NDC(.9);

  c->SaveAs(filename+"_Ecs.pdf");

  c->cd();
  tE_DRcorr->Draw("Hist"); c->Update();
  TPaveStats* statsE_DRcorr = (TPaveStats*)c->GetPrimitive("stats");
  statsE_DRcorr->SetName("Sumbf");
  statsE_DRcorr->SetTextColor(kBlack);
  statsE_DRcorr->SetX1NDC(.63); statsE_DRcorr->SetX2NDC(.9);
  statsE_DRcorr->SetY1NDC(.7); statsE_DRcorr->SetY2NDC(.9);

  tE_DRcorr_30->Draw("Hist&sames"); c->Update();
  TPaveStats* statsE_DRcorr_30 = (TPaveStats*)c->GetPrimitive("stats");
  statsE_DRcorr_30->SetName("Sumaf");
  statsE_DRcorr_30->SetTextColor(kGray+1);
  statsE_DRcorr_30->SetX1NDC(.63); statsE_DRcorr_30->SetX2NDC(.9);
  statsE_DRcorr_30->SetY1NDC(.5); statsE_DRcorr_30->SetY2NDC(.7);
  c->SaveAs(filename+"_EDR_30.pdf");

  TF1* grE_DRcorr = new TF1("DRcorrfit","gaus",low,high); grE_DRcorr->SetLineColor(kBlack);
  tE_DRcorr->SetOption("p"); tE_DRcorr->Fit(grE_DRcorr,"R+&same");
  tE_DRcorr->Draw(""); c->SaveAs(filename+"_EDR_corr.pdf");
  TF1* grE_DRcorr_30 = new TF1("DRcorr30fit","gaus",low,high); grE_DRcorr_30->SetLineColor(kGray+1);
  tE_DRcorr_30->SetOption("p"); tE_DRcorr_30->Fit(grE_DRcorr_30,"R+&same");
  tE_DRcorr_30->Draw(""); c->SaveAs(filename+"_EDR_corr_30.pdf");

  c->cd();
  tE_SC->Draw("Hist"); c->Update();
  c->SaveAs(filename+"_EsumHist.pdf");

  TF1* grE_SC = new TF1("S+Cfit","gaus",2.*low,2.*high); grE_SC->SetLineColor(kBlack);
  tE_SC->SetOption("p"); tE_SC->Fit(grE_SC,"R+&same");
  tE_SC->Draw(""); c->SaveAs(filename+"_Esum.pdf");

  c->SetLogy(1);
  tP_leak->Draw("Hist"); c->SaveAs(filename+"_Pleak.pdf");
  tP_leak_nu->Draw("Hist"); c->SaveAs(filename+"_Pleak_nu.pdf");
  c->SetLogy(0);

  tP_leak_theta->Draw("Hist"); c->SaveAs(filename+"_Pleak_theta.pdf");
  tP_leak_phi->Draw("Hist"); c->SaveAs(filename+"_Pleak_phi.pdf");

  tHit_C->Draw("Hist"); c->SaveAs(filename+"_nHitpEventC.pdf");
  tHit_S->Draw("Hist"); c->SaveAs(filename+"_nHitpEventS.pdf");
  Chit->Draw(); c->SaveAs(filename+"_Chit.pdf");
  Shit->Draw(); c->SaveAs(filename+"_Shit.pdf");

  TF1* grhit_C = new TF1("hit_Cfit","gaus",low*40,high*80); grhit_C->SetLineColor(kBlue);
  TF1* grhit_S = new TF1("hit_Sfit","gaus",low*650,high*1100); grhit_S->SetLineColor(kRed);
  tHit_C->SetOption("p"); tHit_C->Fit(grhit_C,"R+&same");
  tHit_S->SetOption("p"); tHit_S->Fit(grhit_S,"R+&same");

  tHit_C->Draw(); c->SaveAs(filename+"_nHitpEventC_fit.pdf");
  tHit_S->Draw(); c->SaveAs(filename+"_nHitpEventS_fit.pdf");

  t2DhitS->Draw("COLZ"); 
  t2DhitS->GetXaxis()->SetRangeUser(-70, 70);
  t2DhitS->GetYaxis()->SetRangeUser(-70, 70);
  c->SaveAs(filename+"_n2DHitS.pdf");
  t2DhitC->Draw("COLZ"); 
  t2DhitC->GetXaxis()->SetRangeUser(-70, 70);
  t2DhitC->GetYaxis()->SetRangeUser(-70, 70);
  c->SaveAs(filename+"_n2DHitC.pdf");

  tC_lateral->SetLineColor(kBlue);
  tC_lateral->SetLineWidth(2);
  tC_lateral->Scale(1/73.45);
  tC_lateral->Rebin(2);
  tC_lateral->Draw("Hist");

  tS_lateral->SetLineColor(kRed);
  tS_lateral->SetLineWidth(2);
  tS_lateral->Scale(1/1132.5);
  tS_lateral->Rebin(2);
  tS_lateral->Draw("Hist&sames");

  TLegend* leg_lateral = new TLegend(0.65,0.65,0.85,0.85);
  leg_lateral->AddEntry(tC_lateral, "#color[4]{Cerenkov}", "l");
  leg_lateral->AddEntry(tS_lateral, "#color[2]{Scintillation}", "l");
  leg_lateral->SetBorderSize(0);
  leg_lateral->Draw();

  c->SaveAs(filename+"_lateral_shower.pdf");

  c->SetLogy(1);
  c->SaveAs(filename+"_lateral_shower_log.pdf");

  tC_lateral->GetXaxis()->SetRangeUser(0, 200);
  tS_lateral->GetXaxis()->SetRangeUser(0, 200);
  c->SaveAs(filename+"_lateral_shower_zoom.pdf");
  c->SetLogy(0);

  c->cd();
  // c->SetLeftMargin(0.14);
  // c->SetRightMargin(0.19);
  gStyle->SetPaintTextFormat("2.4f");
  c->SetLogz(1);
  t2DRecoE_C->SetMarkerSize(1.3);
  t2DRecoE_S->SetMarkerSize(1.3);
  t2DRecoE_S_frac->SetMarkerSize(1.3);
  t2DRecoE_C_frac->SetMarkerSize(1.3);
  t2DRecoE_C->SetMaximum(20);
  t2DRecoE_S->SetMaximum(20);
  t2DRecoE_C->SetMinimum(1e-3);
  t2DRecoE_S->SetMinimum(1e-3);
  t2DRecoE_S_frac->SetMinimum(1e-3);
  t2DRecoE_C_frac->SetMinimum(1e-3);

  t2DRecoE_C->GetXaxis()->SetLabelSize(0.);
  t2DRecoE_C->GetYaxis()->SetLabelSize(0.);
  t2DRecoE_S->GetXaxis()->SetLabelSize(0.);
  t2DRecoE_S->GetYaxis()->SetLabelSize(0.);
  t2DRecoE_S_frac->GetXaxis()->SetLabelSize(0.);
  t2DRecoE_S_frac->GetYaxis()->SetLabelSize(0.);
  t2DRecoE_C_frac->GetXaxis()->SetLabelSize(0.);
  t2DRecoE_C_frac->GetYaxis()->SetLabelSize(0.);

  t2DRecoE_C->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoEC.pdf");
  t2DRecoE_S->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoES.pdf");
  t2DRecoE_S_frac->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoES_frac.pdf");
  t2DRecoE_C_frac->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoEC_frac.pdf");

  t2DRecoE_C->GetXaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_C->GetYaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_S->GetXaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_S->GetYaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_S_frac->GetXaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_S_frac->GetYaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_C_frac->GetXaxis()->SetRangeUser(1.5, 4.5);
  t2DRecoE_C_frac->GetYaxis()->SetRangeUser(1.5, 4.5);

  t2DRecoE_C->SetMarkerSize(2.2);
  t2DRecoE_S->SetMarkerSize(2.2);
  t2DRecoE_S_frac->SetMarkerSize(2.2);
  t2DRecoE_C_frac->SetMarkerSize(2.2);

  t2DRecoE_C->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoEC_zoom.pdf");
  t2DRecoE_S->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoES_zoom.pdf");
  t2DRecoE_S_frac->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoES_frac_zoom.pdf");
  t2DRecoE_C_frac->Draw("COL0Z TEXT"); c->SaveAs(filename+"_n2DRecoEC_frac_zoom.pdf");
  c->SetLogz(0);

  c->cd();
  TH1D* t2DhitS_projX = t2DhitS->ProjectionX(); t2DhitS_projX->SetStats(0);
  TH1D* t2DhitC_projX = t2DhitC->ProjectionX(); t2DhitC_projX->SetStats(0);
  t2DhitS_projX->SetLineColor(kRed); t2DhitS_projX->SetLineWidth(2); t2DhitS_projX->Rebin(6);
  t2DhitC_projX->SetLineColor(kBlue); t2DhitC_projX->SetLineWidth(2); t2DhitC_projX->Rebin(6);
  t2DhitS_projX->Draw("Hist"); c->SaveAs(filename+"_n2DHitS_projX.pdf");
  t2DhitC_projX->Draw("Hist"); c->SaveAs(filename+"_n2DHitC_projX.pdf");

  t2DhitS_projX->Scale(1/1132.5);
  t2DhitC_projX->Scale(1/73.45);

  TLegend* leg_projX_bf = new TLegend(0.65,0.65,0.85,0.85);
  leg_projX_bf->AddEntry(t2DhitS_projX, "#color[2]{Scintillation}", "l");
  leg_projX_bf->AddEntry(t2DhitC_projX, "#color[4]{Cerenkov}", "l");
  leg_projX_bf->SetBorderSize(0);

  t2DhitC_projX->Draw("Hist");
  t2DhitS_projX->Draw("Hist&sames"); 
  leg_projX_bf->Draw();
  c->SaveAs(filename+"_n2DHitSC_projX_norm.pdf");

  c->SetLogy(1);
  t2DhitS_projX->Draw("Hist");
  t2DhitC_projX->Draw("Hist&sames"); 
  leg_projX_bf->Draw();
  c->SaveAs(filename+"_n2DHitSC_projX_log.pdf");
  c->SetLogy(0);

  // Draw difference plot between t2DhitS_projX and t2DhitC_projX (linear scale)
  int nbins = t2DhitS_projX->GetNbinsX();
  TH1D* hDiff_projX = (TH1D*)t2DhitS_projX->Clone("hDiff_projX");
  TH1D* hDiff_line  = (TH1D*)t2DhitS_projX->Clone("hDiff_line");
  hDiff_projX->SetLineColor(kBlack);
  hDiff_projX->SetLineWidth(2);
  for (int i = 1; i <= nbins; ++i) {
    double s = t2DhitS_projX->GetBinContent(i);
    double c = t2DhitC_projX->GetBinContent(i);
    hDiff_projX->SetBinContent(i, (c != 0) ? s/c : 0.);
    hDiff_line->SetBinContent(i, 1.);
  }

  // Create a canvas with two pads
  TCanvas* cDiff = new TCanvas("cDiff", "ProjX S, C, and Difference", 800, 600);
  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.25, 1.0, 1.0);
  TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.25);
  pad1->SetBottomMargin(0.02);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.3);
  pad1->Draw();
  pad2->Draw();

  // Top pad: original histograms
  pad1->cd();
  pad1->SetLogy(0);
  t2DhitS_projX->Draw("Hist");
  t2DhitC_projX->Draw("Hist&sames");
  t2DhitS_projX->GetXaxis()->SetLabelSize(0);
  t2DhitC_projX->GetXaxis()->SetLabelSize(0);
  t2DhitC_projX->SetStats(0);
  t2DhitS_projX->SetStats(0);
  TLegend* leg_projX = new TLegend(0.15,0.65,0.33,0.85);
  leg_projX->AddEntry(t2DhitS_projX, "#color[2]{Scintillation}", "l");
  leg_projX->AddEntry(t2DhitC_projX, "#color[4]{Cerenkov}", "l");
  leg_projX->SetBorderSize(0);
  leg_projX->Draw();

  // Bottom pad: ratio plot
  pad2->cd();
  hDiff_projX->GetYaxis()->SetTitle("S/C");
  hDiff_projX->GetYaxis()->SetTitleSize(0.13);
  hDiff_projX->GetYaxis()->SetTitleOffset(0.4);
  hDiff_projX->GetYaxis()->SetLabelSize(0.10);
  //hDiff_projX->GetYaxis()->SetRangeUser(0.9,1.1);
  hDiff_projX->GetYaxis()->SetRangeUser(0.5,1.5);
  hDiff_projX->GetYaxis()->SetNdivisions(505);
  hDiff_projX->GetXaxis()->SetLabelSize(0.10);
  hDiff_projX->GetXaxis()->SetTitleSize(0.13);
  hDiff_projX->GetXaxis()->SetTitleOffset(1.0);
  hDiff_projX->SetStats(0);

  hDiff_line->SetLineColor(kGreen);
  hDiff_line->SetLineStyle(2);
  hDiff_line->SetStats(0);

  hDiff_projX->Draw("p");
  hDiff_line->Draw("Hist&sames");

  cDiff->SaveAs(filename+"_n2DHitSC_projX_diff.pdf");

  c->cd();
  TH1D* t2DhitS_projY = t2DhitS->ProjectionY(); t2DhitS_projY->SetStats(0);
  TH1D* t2DhitC_projY = t2DhitC->ProjectionY(); t2DhitC_projY->SetStats(0);
  t2DhitS_projY->SetLineColor(kRed); t2DhitS_projY->SetLineWidth(2); t2DhitS_projY->Rebin(6);
  t2DhitC_projY->SetLineColor(kBlue); t2DhitC_projY->SetLineWidth(2); t2DhitC_projY->Rebin(6);
  t2DhitS_projY->Draw("Hist"); c->SaveAs(filename+"_n2DHitS_projY.pdf");
  t2DhitC_projY->Draw("Hist"); c->SaveAs(filename+"_n2DHitC_projY.pdf");

  t2DhitS_projY->Scale(1/1132.5);
  t2DhitC_projY->Scale(1/73.45);

  TLegend* leg_projY_bf = new TLegend(0.65,0.65,0.85,0.85);
  leg_projY_bf->AddEntry(t2DhitS_projY, "#color[2]{Scintillation}", "l");
  leg_projY_bf->AddEntry(t2DhitC_projY, "#color[4]{Cerenkov}", "l");
  leg_projY_bf->SetBorderSize(0);

  t2DhitS_projY->Draw("Hist");
  t2DhitC_projY->Draw("Hist&sames"); 
  leg_projY_bf->Draw();
  c->SaveAs(filename+"_n2DHitSC_projY_norm.pdf");

  c->SetLogy(1);
  t2DhitS_projY->Draw("Hist");
  t2DhitC_projY->Draw("Hist&sames"); 
  leg_projY_bf->Draw();
  c->SaveAs(filename+"_n2DHitSC_projY_log.pdf");
  c->SetLogy(0);

  // Draw difference plot between t2DhitS_projY and t2DhitC_projY (linear scale)
  nbins = t2DhitS_projY->GetNbinsX();
  TH1D* hDiff_projY = (TH1D*)t2DhitS_projY->Clone("hDiff_projY");
  TH1D* hDiff_lineY = (TH1D*)t2DhitS_projY->Clone("hDiff_lineY");
  hDiff_projY->SetLineColor(kBlack);
  hDiff_projY->SetLineWidth(2);
  hDiff_projY->SetStats(0);
  hDiff_projY->SetTitle("");
  for (int i = 1; i <= nbins; ++i) {
    double s = t2DhitS_projY->GetBinContent(i);
    double c = t2DhitC_projY->GetBinContent(i);
    hDiff_projY->SetBinContent(i, (c != 0) ? s/c : 0.);
    hDiff_lineY->SetBinContent(i, 1.);
  }
  hDiff_lineY->SetLineColor(kGreen+2);
  hDiff_lineY->SetLineStyle(2);
  hDiff_lineY->SetStats(0);

  // Top pad: original histograms
  TCanvas* cDiffY = new TCanvas("cDiffY", "ProjY S, C, and Difference", 800, 600);
  TPad* pad11 = new TPad("pad11", "pad1", 0.0, 0.25, 1.0, 1.0);
  TPad* pad22 = new TPad("pad22", "pad2", 0.0, 0.0, 1.0, 0.25);
  pad11->SetBottomMargin(0.02);
  pad22->SetTopMargin(0.05);
  pad22->SetBottomMargin(0.3);
  pad11->Draw();
  pad22->Draw();

  pad11->cd();
  pad11->SetLogy(1);
  t2DhitS_projY->Draw("Hist");
  t2DhitC_projY->Draw("Hist&sames");
  t2DhitC_projY->SetStats(0);
  t2DhitS_projY->SetStats(0);
  t2DhitS_projY->GetXaxis()->SetLabelSize(0);
  t2DhitC_projY->GetXaxis()->SetLabelSize(0);
  TLegend* leg_projY = new TLegend(0.15,0.65,0.33,0.85);
  leg_projY->AddEntry(t2DhitS_projY, "#color[2]{Scintillation}", "l");
  leg_projY->AddEntry(t2DhitC_projY, "#color[4]{Cerenkov}", "l");
  leg_projY->SetBorderSize(0);
  leg_projY->Draw();

  // Bottom pad: ratio plot
  pad22->cd();
  hDiff_projY->GetYaxis()->SetTitle("S/C");
  hDiff_projY->GetYaxis()->SetTitleSize(0.13);
  hDiff_projY->GetYaxis()->SetTitleOffset(0.4);
  hDiff_projY->GetYaxis()->SetLabelSize(0.10);
  hDiff_projY->GetXaxis()->SetLabelSize(0.10);
  hDiff_projY->GetXaxis()->SetTitleSize(0.13);
  hDiff_projY->GetXaxis()->SetTitleOffset(1.0);
  hDiff_projY->GetYaxis()->SetRangeUser(0.,2.0);
  hDiff_projY->GetYaxis()->SetNdivisions(505);
  //hDiff_projY->GetYaxis()->SetRangeUser(0.5,1.5);
  hDiff_projY->Draw("p");
  hDiff_lineY->Draw("Hist&sames");
  pad22->Update();
  cDiffY->Update();
  cDiffY->SaveAs(filename+"_n2DHitSC_projY_diff.pdf");

  c->cd();

  TGraph* grSvsC = new TGraph(entries,&(E_Ss[0]),&(E_Cs[0]));
  grSvsC->SetTitle("");
  grSvsC->SetMarkerSize(0.5); grSvsC->SetMarkerStyle(20);
  grSvsC->GetXaxis()->SetLimits(0.,high);
  grSvsC->GetYaxis()->SetRangeUser(0.,high);
  grSvsC->SetMaximum(high);
  grSvsC->SetMinimum(0.);
  grSvsC->Draw("ap");
  c->SaveAs(filename+"_SvsC.pdf");

  tT_C->Draw("Hist"); c->SaveAs(filename+"_tC.pdf");
  tT_S->Draw("Hist"); c->SaveAs(filename+"_tS.pdf");
  tWav_C->Draw("Hist"); c->SaveAs(filename+"_wavC.pdf");
  tWav_S->Draw("Hist"); c->SaveAs(filename+"_wavS.pdf");
  tNhit_C->Draw("Hist"); c->SaveAs(filename+"_nhitC.pdf");
  tNhit_S->Draw("Hist"); c->SaveAs(filename+"_nhitS.pdf");

  grE_C->Write();
  grE_S->Write();
  grE_SC->Write();
  grhit_C->Write();
  grhit_S->Write();

  outputRoot->Close();

  /*std::string fileout_CSS = "/u/user/syjang/DRC_generic/result/";
  std::ofstream ofstream1;

  ofstream1.open(fileout_CSS+"/"+material_base+"_Attenuation_points.csv", std::ios::out | std::ios::app);
  ofstream1 << en_val << "   C    mean    "<<tE_C->GetMean() << "    " << tE_C->GetMeanError() << std::endl;
  ofstream1 << en_val << "   C    sig     "<<tE_C->GetRMS() << "    " << tE_C->GetRMSError() << std::endl;
  ofstream1 << en_val << "   S    mean    "<<tE_S->GetMean() << "    " << tE_S->GetMeanError() << std::endl;
  ofstream1 << en_val << "   S    sig     "<<tE_S->GetRMS() << "    " << tE_S->GetRMSError() << std::endl;

  /*ofstream1.open(fileout_CSS+"/"+material_base+"/"+material_base+"_CSS_Error_atten_final.csv", std::ios::out | std::ios::app);
  ofstream1 << en_val << "   C    mean    "<<tE_C->GetMean() << "    " << tE_C->GetMeanError() << std::endl;
  ofstream1 << en_val << "   C    sig     "<<tE_C->GetRMS() << "    " << tE_C->GetRMSError() << std::endl;
  ofstream1 << en_val << "   Sbf    mean    "<<tE_Scorr->GetMean() << "    " << tE_S->GetMeanError() << std::endl;
  ofstream1 << en_val << "   Sbf    sig     "<<tE_Scorr->GetRMS() << "    "  << tE_S->GetRMSError() << std::endl;
  ofstream1 << en_val << "   Saf    mean    "<<tE_Scorr_30->GetMean() << "    " << tE_Scorr_30->GetMeanError() << std::endl;
  ofstream1 << en_val << "   Saf    sig     "<<tE_Scorr_30->GetRMS() << "    "  << tE_Scorr_30->GetRMSError() << std::endl;
  ofstream1 << en_val << "   Sumbf      mean    "<<grE_DRcorr->GetParameter(1) << "    " << grE_DRcorr->GetParError(1) << std::endl;
  ofstream1 << en_val << "   Sumbf      sig     "<<grE_DRcorr->GetParameter(2) << "    " << grE_DRcorr->GetParError(2) << std::endl;
  ofstream1 << en_val << "   Sumaf  mean    "<<grE_DRcorr_30->GetParameter(1) << "    " << grE_DRcorr_30->GetParError(1) << std::endl;
  ofstream1 << en_val << "   Sumaf  sig     "<<grE_DRcorr_30->GetParameter(2) << "    " << grE_DRcorr_30->GetParError(2) << std::endl;

  ofstream1.close();*/

}
