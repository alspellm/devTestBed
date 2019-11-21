#include "GausFit.h"
#include <iostream>
#include "TKey.h"
#include "TClass.h"
#include "TFile.h"
int main() {

    std::string File = "physrun2019_10648_anaSvtCond.root";
    std::string folder = "~/work/run/analysis_runs/physrun2019_10648/";
    TFile* inFile = new TFile(Form("%s%s", folder.c_str(),File.c_str()), "READ");
    GausFit* tools = new GausFit();

    tools->GetHistosFromFile(inFile, folder, "0"); 

    TFile* outfile = new TFile("testing.root");
    std::cout << zzz << std::endl;
   // std::cout << std::to_string(histos2d) << std::endl;

/*for (unsigned int ih=0; ih<histos2dkeys.size();ih++) {
    std::cout<<histos2d[histos2dkeys.at(ih)]->GetTitle()<<std::endl;
  }

for (unsigned int ih2d = 0; ih2d<histos2dkeys.size();++ih2d) {
    //for (unsigned int ih2d = 0; ih2d<0;++ih2d) {
    std::string histoname = histos2d[histos2dkeys.at(ih2d)]->GetName();
    std::string graphname = "mean_" + histoname;
    //Cut low charge hits in layers 2-6
    float fitRange[2] = {300,8000};
    std::cout<<"Working on "<<histoname<<std::endl;
    if (histoname.find("L0") == std::string::npos && histoname.find("L1") == std::string::npos) {
      std::cout<<"Setting lower limit of "<<histoname<<" to 700"<<std::endl;
      fitRange[0] = 700;
    }
    std::pair<TGraphErrors*,TGraphErrors*> results = FillPlot(histos2d[histos2dkeys.at(ih2d)],histos2d[histos2dkeys.at(ih2d)]->GetName(),2,out
    file, binning,fitRange);
    outfile->cd();
    results.first->Write(graphname.c_str());
    }

*/
    













}
