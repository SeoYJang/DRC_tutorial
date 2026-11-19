#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"

#include <iostream>
#include <fstream>

void makePoints(Int_t n, Double_t *x, Double_t *y, Double_t *e, Int_t p, string m, string parti);
void ReverseXAxis (TH1 *h);
void ReverseYAxis (TH1 *h);

void ele_linearity_forplot_af(string mater, string Particle)
{
  string material = mater;
  string particle = Particle;

  Int_t n = 8;
  Double_t *x = new Double_t[n];
  Double_t *y = new Double_t[n];
  Double_t *e = new Double_t[n];
  TCanvas *myc = new TCanvas("myc",  "Energy Linearity");
  //myc->SetGrid();
  myc->SetTickx();
  myc->SetTicky();

  // c--------------------------------------------------------- 
  makePoints(n, x, y, e, 2, material, particle);
  TGraphErrors *gre2 = new TGraphErrors(n, x, y, 0, e);
  gre2->SetMarkerStyle(22);
   gre2->SetMarkerColor(kBlue);
   gre2->SetLineColor(kBlue);  
  //Fit the graph with the predefined "pol3" function

  std::cout << "Cerenkov" << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << y[i] << std::endl;
  }

  // s-------------------------------------------------
  makePoints(n, x, y, e, 3, material, particle);
  TGraphErrors *gre3 = new TGraphErrors(n, x, y, 0, e);
  //gre3->Draw("a*");
  gre3->SetMarkerStyle(20);
  gre3->SetMarkerColor(kRed);
  gre3->SetLineColor(kRed);
  //Fit the graph with the predefined "pol3" function

  std::cout << "Scintillation" << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << y[i] << std::endl;
  }

  // sum-----------------------------------------------
  makePoints(n, x, y, e, 4, material, particle);
  TGraphErrors *gre4 = new TGraphErrors(n, x, y, 0, e);
  //gre4->Draw("a*");
  gre4->SetMarkerStyle(21);
  gre4->SetMarkerColor(kBlack);
  gre4->SetLineColor(kBlack);
  gre4->Fit("pol0");

  TF1 *f4 = gre4->GetFunction("pol0");
  f4->SetLineWidth(0);

  auto *f44 = new TGraph();
  f44->SetPoint(0, 0., f4->GetParameter(0));
  f44->SetPoint(1, 120., f4->GetParameter(0));
  f44->SetLineColor(kBlack);
  f44->SetLineWidth(2);  
  f44->SetLineStyle(2);

  std::cout << "DR corrected" << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << y[i] << std::endl;
  }

  float max_diff = 0.;
  for (int i=1; i<n; i++) {
    float diff = abs(y[i] - f4->GetParameter(0));
    if (diff > max_diff) max_diff = diff;
  }

  auto *f_line_1 = new TGraph();
  f_line_1->SetPoint(0, 0., 1.);
  f_line_1->SetPoint(1, 120., 1.);
  f_line_1->SetLineColor(kBlack);
  f_line_1->SetLineWidth(2);
  f_line_1->SetLineStyle(2);

  std::cout << "max_diff: " << max_diff << std::endl;

// merging ------------------------------------------------------

   TMultiGraph  *mg  = new TMultiGraph();
    //mg->Add(f44,"L");
    mg->Add(f_line_1, "L");
    mg->Add(gre2);
    mg->Add(gre3);
    mg->Add(gre4);

    //string title = "Pion Energy Linearity ("+mater+", (#it{#theta}, #it{#phi}) = (1.5#circ, 1.0#circ))";
    mg->SetTitle("");
    //mg->GetXaxis()->SetTitle("E [GeV]");
    //mg->GetYaxis()->SetTitle("Response");

    mg->GetYaxis()->SetRangeUser(0.98, 1.02);
    if (particle == "pi") mg->GetYaxis()->SetRangeUser(0.3, 1.1);

    mg->Draw("A P");

    mg->GetXaxis()->SetLimits(0, 120);

    //mg->SetMinimum(0.98);
    //mg->SetMaximum(1.02);

// legend------------------------------------------------------
  float legendX1, legendX2, legendY1, legendY2;
  legendX1 = 0.65; legendX2 = 0.85; legendY1 = 0.15; legendY2 = 0.35;
  TLegend *leg = new TLegend(0.65, 0.15, 0.85, 0.35);
  //  TLegend *leg = new TLegend(0.7, 0.75, 0.9, 0.9);
                                //   (           x2, y2)
                                //   (x1, y1,          )

  string c_f = "#color[4]{Cerenkov}";
  const char* c_ff = c_f.c_str();
  leg->AddEntry(gre2, c_ff, "lpe");

  string s_f = "#color[2]{Scintillation}";                               
  const char* s_ff = s_f.c_str();
  leg->AddEntry(gre3, s_ff, "lpe");

  string a_f = "#color[1]{Summation}";    
  if (particle == "pi") a_f = "#color[1]{DR corrected}";              
  const char* a_ff = a_f.c_str();
  leg->AddEntry(gre4, a_ff, "lpe");

  std::string savename_wo = material + "_"+particle+"_linearity_wostat.pdf";
  myc->SaveAs(savename_wo.c_str());

  leg->SetBorderSize(0);
  leg->Draw("p");

  /*TLegend *leg1 = new TLegend(0.5, 0.225, 0.7, 0.275);
  std::string titlename = "#bf{#it{W e^{-}}}";
  leg1->SetHeader(titlename.c_str());
  leg1->SetBorderSize(0);
  leg1->Draw("");*/
    cout<<""<<endl;

  std::string savename = material + "_"+particle+"_linearity.pdf";
  myc->SaveAs(savename.c_str());

}

void makePoints(Int_t n, Double_t *x, Double_t *y, Double_t *e, Int_t p, string m, string parti)
{

    string type, var;
    double en, mean, std;
    std::ifstream in;
    std::string FileName = "/fcc/home/syjang/DRC_tutorial/results/"+m+"_CSS_Error.csv";
    in.open(FileName,std::ios::in);
    for (int i=0; i<n; i++) {
      for (int j=0; j<10; j++) {
        in >> en >> type >> var >> mean >> std;
        x[i] = en;
//        cout << var << endl;

        if (p==2 && type=="C") {
          if (var=="mean") {y[i]=mean/en; e[i]=std/en;}
        }

        if (p==3 && type=="Saf") {
          if (var=="mean") {y[i]=mean/en; e[i]=std/en;}
        }

        if (p==4 && type=="Sumaf") {
          if (var=="mean") {
            y[i]=mean/(en); e[i]=std/(en);
            if (parti == "ele") {y[i]=mean/(en*2); e[i]=std/(en*2);}
          }
        }
      }
    }
    in.close();

  //analysis.cc에서 바로 가져오기. 
  // 백업에다가 나온 그래프들 다 포함시키기! 

  //E_C Mean, Sigma and E_S Mean, Sigma and Sum Mean, Simga
}