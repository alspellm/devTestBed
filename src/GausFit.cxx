#include "GausFit.h"
#include <iostream>
#include "TKey.h"
#include "TClass.h"
GausFit::GausFit() {GausFit();};

void GausFit::GetHistosFromFile(TFile* inFile, std::string folder, std::string timesample) {
    std::cout << zzz << std::endl;
    TIter next(inFile->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        std::string classType = key->GetClassName();
        std::string s(key->GetName());
        //if (classType.find("TH1")!= std::string::npos)
          // if (s.find(Form("timesample_%s",timesample.c_str())) != std::string::npos)
            //    histos1dkeys.push_back(s);
              //  histos1d[key->GetName()] = (TH1F*) key->ReadObj();
        if (classType.find("TH2")!=std::string::npos)
            if (s.find(Form("timesample_%s", timesample.c_str())) != std::string::npos) {
                histos2dkeys.push_back(s);
                histos2d[key->GetName()] = (TH2F*) key->ReadObj();
            }
        //if (classType.find("TH3")!=std::string::npos)
          //  histos3d[key->GetName()] = (TH3F*) key->ReadObj();
    }
        


}

std::pair<TGraphErrors*,TGraphErrors*> GausFit::FillPlot(TH2* histo2d, const std::string y_title,int method,TFile* outfile, int binning, float* fitRange) {

  int point = 0;
  std::pair < TGraphErrors*,TGraphErrors* > graphs;
  TGraphErrors* graph   = new TGraphErrors();
  TGraphErrors* graph_w = new TGraphErrors();
  std::pair<float,float> mean;
  mean.first  = 0.;
  mean.second = 0.;
  std::pair<float,float> width;
  width.first  = 0.;
  width.second = 0.;

  //General graphics settings
  graph->SetLineWidth(2);
  graph->SetMarkerColor(kBlack);
  graph->SetLineColor(kBlack);
  graph->GetXaxis()->SetLabelFont(42);
  graph->GetXaxis()->SetTitleSize(0.035);
  graph->GetXaxis()->SetTitleFont(42);
  graph->GetXaxis()->SetTitleOffset(1.2);
  graph->GetYaxis()->SetTitle(y_title.c_str());
  graph->GetYaxis()->SetLabelFont(42);
  graph->GetYaxis()->SetTitleSize(0.035);
  graph->GetYaxis()->SetTitleOffset(1.8);
  graph->GetYaxis()->SetTitleFont(42);
  graph->SetTitle(0);

graph_w->SetLineWidth(2);
  graph_w->SetMarkerColor(kBlack);
  graph_w->SetLineColor(kBlack);
  graph_w->GetXaxis()->SetLabelFont(42);
  graph_w->GetXaxis()->SetTitleSize(0.035);
  graph_w->GetXaxis()->SetTitleFont(42);
  graph_w->GetXaxis()->SetTitleOffset(1.2);
  graph_w->GetYaxis()->SetTitle(y_title.c_str());
  graph_w->GetYaxis()->SetLabelFont(42);
  graph_w->GetYaxis()->SetTitleSize(0.035);
  graph_w->GetYaxis()->SetTitleOffset(1.8);
  graph_w->GetYaxis()->SetTitleFont(42);
  graph_w->SetTitle(0);

  std::string histotitle = histo2d->GetName();
  std::string name="";
  if (binning > 1 && (histo2d->GetNbinsX() % binning == 0))
    histo2d->RebinX(binning);
  for (int ibinx = 1; ibinx < histo2d->GetNbinsX()+1;++ibinx) {

    name = histotitle+"_proj_"+std::to_string(ibinx);
    TH1D* proj_y = histo2d->ProjectionY(name.c_str(),ibinx,ibinx);

    if (proj_y -> GetEntries() > 100) {
      GetMean(proj_y,mean,method,outfile,fitRange);

      graph->SetPoint(point,ibinx,mean.first);
      graph->SetPointError(point,0,mean.second);
      if (method==2){
        graph_w->SetPoint(point,ibinx,width.first);
        graph_w->SetPointError(point,0,width.second);
      }
      point++;
    }

    delete proj_y; proj_y = nullptr;
  }//loop over the bins
  graphs.first = graph;
  graphs.second = graph_w;
  return graphs;
}


GausFit::GetMean(tmp,mean_and_err,width_and_err,method,outFile,ss.str(),layer);

if (mean_and_err.first == 0) {
  std::cout<<"FillPlot::WARNING::Mean value = 0 for run number "<<runNumber<<std::endl;
  std::cout<<"Plot saved in order to check"<<std::endl;
  TCanvas c;
  c.cd();
  tmp->Draw();
  std::string c_name = "c_";
  c_name+=tmp->GetName();
  c_name+=("_"+std::to_string(runNumber));
  c.Print((c_name+".png").c_str());
}

if (mean_and_err.first!=0) {
  if (runNo) {
    graph->SetPoint(point,runNumber,mean_and_err.first);
    graph->SetPointError(point,0,mean_and_err.second);
    if (method ==2) {
        graph_w->SetPoint(point,runNumber,width_and_err.first);
        graph_w->SetPointError(point,0,width_and_err.second);
}
  }
  else {
    if (m_RunLumiMap[runNumber]<=0) {
        std::cout<<"WARNING:Lumi value not found for this run "<< runNumber<<" skipping...."<<std::endl;
    continue;
}
    graph->SetPoint(point,m_RunLumiMap[runNumber],mean_and_err.first);
    graph->SetPointError(point,0,mean_and_err.second);
    if (method==2) {
      graph_w->SetPoint(point,m_RunLumiMap[runNumber],width_and_err.first);
      graph_w->SetPointError(point,0,width_and_err.second);
}
  }

  //Map lumi to runNumber not done yet.
  //Will use goodRunList or lumiCal output directly.

  //std::cout<<mean_and_err.first<<" "<<mean_and_err.second
  //<<" "<<runNumber<<std::endl;

  point++;
  delete tmp;
    }   
}
 std::pair<TGraphErrors*,TGraphErrors*> graphs;
  graphs.first  = graph;
  graphs.second = graph_w;
  return graphs;
}








void GausFit::Fit() {

TFile* outfile = new TFile("testing.root");
for (unsigned int ih=0; ih<histos2dkeys.size();ih++) {
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
    std::pair<TGraphErrors*,TGraphErrors*> results = FillPlot(histos2d[histos2dkeys.at(ih2d)],histos2d[histos2dkeys.at(ih2d)]->GetName(),2,outfile, binning,fitRange);
    outfile->cd();
    results.first->Write(graphname.c_str());
    }
}
