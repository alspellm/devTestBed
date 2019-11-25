#include <iostream>
#include "TKey.h"
#include "TClass.h"
#include "TFile.h"
#include "utilities.h"
#include "HistogramHelpers.h"

std::vector<std::string> histos2dk;
std::map<std::string, TH2F*> histos2d;

int main() {

    std::string File = "physrun2019_10648_anaSvtCond.root";
    std::string folder = "~/work/run/analysis_runs/physrun2019_10648/";
    TFile* inFile = new TFile(Form("%s%s", folder.c_str(),File.c_str()), "READ");
    TFile* outfile = new TFile("testing.root","RECREATE");

    Get2DHistosFromFile(histos2d, histos2dk,inFile, folder, "0");
    std::cout << histos2d[histos2dk.at(0)] << std::endl; 
    std::cout << histos2dk.at(0) << std::endl;
    int binning =1;
   // std::cout << histos2dk[0] << std::endl;

    for (unsigned int ih2d = 0; ih2d<histos2dk.size();++ih2d) {
    //for (unsigned int ih2d = 0; ih2d<1;++ih2d) {
    std::string histoname = histos2d[histos2dk[ih2d]]->GetName();
    std::cout << histoname << std::endl;
    std::string graphname_m = "mean_"  + histoname;
    std::string graphname_w = "width_" + histoname;

    TH1F* histoMean  = new TH1F(graphname_m.c_str(),graphname_m.c_str(), histos2d[histos2dk[ih2d]]->GetNbinsX(),histos2d[histos2dk[ih2d]]->GetXaxis()->GetXmin(),histos2d[histos2dk[ih2d]]->GetXaxis()->GetXmax());
    outfile->cd();
  //  histos2d[histos2dk.at(ih2d)]->Write();

    TH1F* histoWidth = new TH1F(graphname_w.c_str(),graphname_w.c_str(), histos2d[histos2dk[ih2d]]->GetNbinsX(),histos2d[histos2dk[ih2d]]->GetXaxis()->GetXmin(),histos2d[histos2dk[ih2d]]->GetXaxis()->GetXmax());

    HistogramHelpers::profileYwithIterativeGaussFit(histos2d[histos2dk[ih2d]],histoMean,histoWidth,binning,0);
    outfile->cd();
    histoMean ->Write(graphname_m.c_str());
    histoWidth->Write(graphname_w.c_str());

  }



  outfile->Close();
  delete outfile;
  outfile = nullptr;

  return 0;
}    












