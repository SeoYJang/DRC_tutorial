#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"

#include <sstream>
void makePoints(Int_t n, Double_t *x, Double_t *y, Double_t *e, Int_t p, string m, string parti);
void ReverseXAxis(TMultiGraph *his);
string to_string_with_precision(float a, int n);
//void FitOne(Int_t i);

void ele_resolution_forplot_ele_af(string mater, string Particle)
{
  string material = mater;
  string particle = Particle;

  Int_t n = 8;
  Double_t *x = new Double_t[n];
  Double_t *y = new Double_t[n];
  Double_t *e = new Double_t[n];
  TCanvas *myc = new TCanvas("myc",  "Energy Resolution");

  // c--------------------------------------------------------- 
  makePoints(n, x, y, e, 2, material, particle);
  TGraphErrors *gre2 = new TGraphErrors(n, x, y, 0, e);
  gre2->SetMarkerStyle(22);
  gre2->SetMarkerColor(kBlue);
  gre2->SetLineColor(kBlue);  
  //Fit the graph with the predefined "pol3" function

  gre2->Fit("pol1");
  //Access the fit resuts

  TF1 *f2 = gre2->GetFunction("pol1");
  //f2->SetLineColor(kBlue);
  f2->SetLineWidth(0);  


  auto *f22 = new TGraph();
  f22->SetPoint(0, 0.05, 0.05*(f2->GetParameter(1))+(f2->GetParameter(0)));
  f22->SetPoint(1, 0.45, 0.45*(f2->GetParameter(1))+(f2->GetParameter(0)));
  f22->SetLineColor(kBlue);
  f22->SetLineWidth(1);  
  f22->SetLineStyle(7);

  std::cout << "Cerenkov" << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << y[i] << std::endl;
  }

  // TFitResultPtr r =   gre2->Fit("pol1","S");  // TFitResultPtr contains the TFitResult
  // r->Print("V");                                // print full information of fit including covariance matrix
  // r->Write();                                   // store the result in a file

// s-------------------------------------------------
  makePoints(n, x, y, e, 3, material, particle);
  TGraphErrors *gre3 = new TGraphErrors(n, x, y, 0, e);
  //gre3->Draw("a*");
  gre3->SetMarkerStyle(20);
  gre3->SetMarkerColor(kRed);
  gre3->SetLineColor(kRed); 
  //Fit the graph with the predefined "pol3" function
  gre3->Fit("pol1");
  //Access the fit resuts
  TF1 *f3 = gre3->GetFunction("pol1");
  //f3->SetLineColor(kRed);
  f3->SetLineWidth(0);  
  
  auto *f33 = new TGraph();
  f33->SetPoint(0, 0.05, 0.05*(f3->GetParameter(1))+(f3->GetParameter(0)));
  f33->SetPoint(1, 0.45, 0.45*(f3->GetParameter(1))+(f3->GetParameter(0)));
  f33->SetLineColor(kRed);
  f33->SetLineWidth(1); 
  f33->SetLineStyle(1); 

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
  gre4->Fit("pol1");
  //Access the fit resuts
  
  TF1 *f4 = gre4->GetFunction("pol1");
  //f4->SetLineColor(kBlack);

  f4->SetLineWidth(0);      

  auto *f44 = new TGraph();
  f44->SetPoint(0, 0.05, 0.05*(f4->GetParameter(1))+(f4->GetParameter(0)));
  f44->SetPoint(1, 0.45, 0.45*(f4->GetParameter(1))+(f4->GetParameter(0)));
  f44->SetLineColor(kBlack);
  f44->SetLineWidth(1);  
  f44->SetLineStyle(2);
  
  std::cout << "Summation" << std::endl;
  for (int i=0; i<n; i++) {
    std::cout << y[i] << std::endl;
  }

// merging ------------------------------------------------------

  TMultiGraph  *mg  = new TMultiGraph();
  mg->Add(gre2, "P");
  mg->Add(gre3, "P");
  mg->Add(gre4, "P");
  mg->Add(f22, "L");
  mg->Add(f33, "L");
  mg->Add(f44, "L");
    //mg->add(f22);

    //string title = "Pion Energy Resolution ("+material+", (#it{#theta}, #it{#phi}) = (1.5#circ, 1.0#circ))";

  mg->SetTitle("");
  //mg->GetXaxis()->SetTitle("1/#sqrt{E} [GeV^{-1/2}]");
  //mg->GetYaxis()->SetTitle("#sigma/E");

  mg->GetXaxis()->SetTitle("");
  mg->GetYaxis()->SetTitle("");

  mg->Draw("A RX");

  myc->SetTicky();
  //myc->SetTickx();

  mg->GetXaxis()->SetLimits(0,0.5);
  float ypi = 0.1;
  if (particle == "ele" && material == "lead") ypi = 0.12;
  if (particle == "ele" && material == "tungsten") ypi = 0.18;
  if (particle == "pi") ypi = 0.7;
  mg->GetYaxis()->SetRangeUser(0.00,ypi);

// legend------------------------------------------------------
  TLegend *leg = new TLegend(0.53, 0.58, 0.86, 0.86);

  string c_f = "#color[4]{C : "+to_string_with_precision(f2->GetParameter(1), 3)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f2->GetParameter(0), 3)+ "}";
  const char* c_ff = c_f.c_str();
  leg->AddEntry(gre2, c_ff, "lpe");
  string s_f = "#color[2]{S : "+to_string_with_precision(f3->GetParameter(1), 3)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f3->GetParameter(0), 3)+ "}";
  const char* s_ff = s_f.c_str();
  leg->AddEntry(gre3, s_ff, "lpe");
  string isDR = "Sum";
  if (particle == "pi") isDR = "DR";
  string a_f = "#color[1]{" + isDR + " : "+to_string_with_precision(f4->GetParameter(1), 3)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f4->GetParameter(0), 3)+ "}";
  const char* a_ff = a_f.c_str();
  leg->AddEntry(gre4, a_ff, "lpe");

  TF1 *axisfunction1 = new TF1("axisfunction1", "1/sqrt(x)", 4, 10);
  TGaxis *newaxis1 = new TGaxis(0.183772, ypi, 0.0, ypi, "axisfunction1", 502, "S");
  TF1 *axisfunction2 = new TF1("axisfunction2", "1/sqrt(x)", 10, 100);
  TGaxis *newaxis2 = new TGaxis(0.4, ypi, 0.183772, ypi, "axisfunction2",502, "S");
  TF1 *axisfunction3 = new TF1("axisfunction3", "1/sqrt(x)", 100, 1000);
  TGaxis *newaxis3 = new TGaxis(0.468377, ypi, 0.4, ypi, "axisfunction3",502, "S");

  newaxis1->SetTickLength(0.082);
  newaxis1->SetLabelOffset(0.04);
  newaxis1->SetLabelFont(42);
  newaxis1->SetLabelSize(0.035);
  newaxis2->SetTickLength(0.07);
  newaxis2->SetLabelOffset(0.04);
  newaxis2->SetLabelFont(42);
  newaxis2->SetLabelSize(0.035);
  newaxis3->SetTickLength(0.221);
  newaxis3->SetLabelSize(0);
  
  newaxis1->Draw();
  newaxis2->Draw();
  newaxis3->Draw();

  std::string savename_wo = material + "_"+particle+"_resolution_wostat.pdf";
  myc->SaveAs(savename_wo.c_str());

  leg->SetMargin(0.2);
  leg->SetBorderSize(0);
  leg->Draw();

  std::string savename = material + "_"+particle+"_resolution.pdf";
  myc->SaveAs(savename.c_str());

  myc->cd();
  mg->Draw("A RX");
  newaxis1->Draw();
  newaxis2->Draw();
  newaxis3->Draw();

  TLegend *leg1 = new TLegend(0.65, 0.65, 0.85, 0.85);

  string isDRfull = "Summation";
  if (particle == "pi") isDRfull = "DR Corrected";

  leg1->AddEntry(gre2, "#color[4]{Cerenkov}", "lpe");
  leg1->AddEntry(gre3, "#color[2]{Scintillation}", "lpe");
  string entry_DR = "#color[1]{"+isDRfull+"}";
  leg1->AddEntry(gre4, entry_DR.c_str(), "lpe");

  leg1->SetBorderSize(0);
  leg1->Draw("p");

  std::string savename_ol = material + "_"+particle+"_resolution_onlylabel.pdf";
  myc->SaveAs(savename_ol.c_str());

  TLegend *leg2 = new TLegend(0.55, 0.55, 0.95, 0.9);

  string c_fst = "#color[4]{#splitline{C : "+to_string_with_precision(f2->GetParameter(1), 4)+" #pm "+to_string_with_precision(f2->GetParError(1), 4)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f2->GetParameter(0), 4)+ " #pm "+to_string_with_precision(f2->GetParError(0), 4)+"}{#chi^{2} / n : "
                                      +to_string(f2->GetChisquare())+" / "+to_string(f2->GetNDF())+"}}";
  const char* c_ffst = c_fst.c_str();
  leg2->AddEntry(gre2, c_ffst, "lpe");
  string s_fst = "#color[2]{#splitline{S : "+to_string_with_precision(f3->GetParameter(1), 4)+" #pm "+to_string_with_precision(f3->GetParError(1), 4)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f3->GetParameter(0), 4)+ " #pm "+to_string_with_precision(f3->GetParError(0), 4)+"}{#chi^{2} / n : "
                                    +to_string(f3->GetChisquare())+" / "+to_string(f3->GetNDF())+"}}";
  const char* s_ffst = s_fst.c_str();
  leg2->AddEntry(gre3, s_ffst, "lpe");
  string a_fst = "#color[1]{#splitline{DR : "+to_string_with_precision(f4->GetParameter(1), 4)+" #pm "+to_string_with_precision(f4->GetParError(1), 4)+" / "+"#sqrt{E} #plus "+to_string_with_precision(f4->GetParameter(0), 4)+ " #pm "+to_string_with_precision(f4->GetParError(0), 4)+"}{#chi^{2} / n : "
                                    +to_string(f4->GetChisquare())+" / "+to_string(f4->GetNDF())+"}}";
  const char* a_ffst = a_fst.c_str();
  leg2->AddEntry(gre4, a_ffst, "lpe");

  leg2->SetBorderSize(0);
  leg2->Draw("p");

  std::string savename_st = material + "_"+particle+"_resolution_stats.pdf";
  myc->SaveAs(savename_st.c_str());
}

void makePoints(Int_t n, Double_t *x, Double_t *y, Double_t *e, Int_t p, string m, string parti)
{

  string type, var;
  double en, mean, std, val, merr, serr, err;
  std::ifstream in;
  std::string FileName = "/fcc/home/syjang/DRC_tutorial/results/"+m+"_CSS_Error.csv";
  in.open(FileName,std::ios::in);
  for (int i=0; i<n; i++) {
    for (int j=0; j<10; j++) {
      in >> en >> type >> var >> val >> err;
      x[i] = 1/sqrt(en);
//       cout << var << endl;
      if (p==2 && type=="C") {
        if (var=="mean") {
          mean=val;
          merr=err;
        } else {
          std=val;
          serr=err;
          y[i]=std/mean;
          e[i]=(std/mean)*sqrt(pow(merr/mean,2)+pow(serr/std,2));
        }
      }
      if (p==3 && type=="Saf") {
        if (var=="mean") {
          mean=val;
          merr=err;
        } else {
          std=val;
          serr=err;
          y[i]=std/mean;
          e[i]=(std/mean)*sqrt(pow(merr/mean,2)+pow(serr/std,2));
        }
      }
      if (p==4 && type=="Sumaf") {
        if (var=="mean") {
          mean=val;
          merr=err;
        } else {
          std=val;
          serr=err;
          y[i]=std/mean;
          e[i]=(std/mean)*sqrt(pow(merr/mean,2)+pow(serr/std,2));
        }
      }
    }
  }
  in.close();
  //analysis.cc에서 바로 가져오기. 
  // 백업에다가 나온 그래프들 다 포함시키기! 

  //E_C Mean, Sigma and E_S Mean, Sigma and Sum Mean, Simga
}

string to_string_with_precision(float a, int n) {

  std::ostringstream out;
  out.precision(n);
  out<<std::fixed<<a;
  return out.str();
  
}

void ReverseXAxis(TMultiGraph *his) {
  his->GetXaxis()->SetLabelOffset(999);
  his->GetXaxis()->SetTickLength(0);

  gPad->Update();
  TGaxis *newaxis = new TGaxis(0.5, 0.0, 0.0, 0.18, 0.0, 0.5, 510, "-");
  newaxis->SetLabelOffset(-0.03);
  newaxis->Draw();
} 