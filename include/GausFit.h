#ifndef GAUSFIT_H
#define GAUSFIT_H

#include "TFile.h"
#include "TClass.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

class GausFit {

    public:
        GausFit();
        virtual void GetHistosFromFile(TFile* inFile, std::string folder="", std::string timesample="");
        std::pair<TGraphErrors*,TGraphErrors*> FillPlot(TH2* histo2d, const std::string y_title, int method,TFile* outfile,int binning,float* fitRange);
        virtual void Fit();
        void GetMean(TH1* hist, std::pair<float,float> &info, int method = 0,TFile* outfile =nullptr,float* fitRange=nullptr);
        void GetMean(TH1F* hist, std::pair<float,float> &info, std::pair<float,float>& info_width,int method =0,TFile* outfile = nullptr,std::string runNumber="0",std::string name="");

    protected:
        std::string zzz;
        std::map<std::string, TH1F*> histos1d;
        typedef std::map<std::string, TH1F*>::iterator it1d;
        std::map<std::string, TH2F*> histos2d;
        typedef std::map<std::string, TH2F*>::iterator it2d;
        std::map<std::string, TH3F*> histos3d;
        typedef std::map<std::string, TH3F*>::iterator it3d;

        std::vector<std::string> histos1dkeys;
        std::vector<std::string> histos2dkeys;


};























#endif
