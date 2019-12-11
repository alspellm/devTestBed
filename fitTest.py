import ROOT as r
import numpy as np
from copy import deepcopy
r.gROOT.SetBatch(True)
from optparse import OptionParser
parser=OptionParser()
parser.add_option("--if", dest="inFile", help="inputFile",default="")
parser.add_option("--s", dest="sensor", help="Sensor")
parser.add_option("--cc", dest="channels", help="List of Channels", default=[])
parser.add_option("--of", dest="outFile", help="output File", default="fitTest.root")
(options, args) = parser.parse_args()

inFile=r.TFile(options.inFile)
outFile=r.TFile(options.outFile, "RECREATE")
sensor=options.sensor
channels=options.channels.split(',')
for i in range(len(channels)):
    channels[i]=int(channels[i])
sensor_prefix="raw_hits_"
sensor_suffix="_timesample_0"



#DeepCopy of all required fit parameters, and original histograms, for one sensor, which are stored in a ROOT file that is output by the gFit executable
hh=deepcopy(getattr(inFile,(sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
mean_h=deepcopy(getattr(inFile,("mean_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
sigma_h=deepcopy(getattr(inFile,("width_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
norm_h=deepcopy(getattr(inFile,("norm_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
FRL_h=deepcopy(getattr(inFile,("FitRangeLower_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
FRU_h=deepcopy(getattr(inFile,("FitRangeUpper_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))

#Define TF1 fit function using parameters extracted from fitting analysis ROOT file
def fitFunc(mean, sigma, norm, FRLD, FRUD):
    fit_func=r.TF1("fit_func","gaus")
    fit_func.SetRange(FRLD,FRUD)
    fit_func.SetParameter(0,norm)
    fit_func.SetParameter(1,mean)
    fit_func.SetParameter(2,sigma)
    return fit_func

#Collect fit parameters, create TCanvas for each channel, plot fit_func over Individual Channel histo
TCanvasDic={}
outFile.cd()
for cc in channels:
    yproj=hh.ProjectionY("%s_ch%i_h"%(sensor,cc), cc+1,cc+1,"e")
    mean=mean_h.GetBinContent(cc+1)
    sigma=sigma_h.GetBinContent(cc+1)
    norm=norm_h.GetBinContent(cc+1)
    FRL=FRL_h.GetBinContent(cc+1)
    FRU=FRU_h.GetBinContent(cc+1)
    print FRU

    
    TCanvasDic.update({cc:r.TCanvas("Channel %s"%(cc))})
    TCanvasDic[cc].cd()
    fit_func=fitFunc(mean, sigma, norm, FRL, FRU)
    yproj.SetTitle("%s%s%s_ch%i_FitTest"%(sensor_prefix,sensor,sensor_suffix,cc))
    yproj.SetTitleOffset(1.0)
   # yproj.GetXaxis().SetRangeUser(FRL,yproj.GetBinCenter(yproj.FindLastBinAbove(0)))
    yproj.GetXaxis().SetRangeUser(FRL,FRL+3000)
    yproj.Draw()
    fit_func.Draw("SAME")
    TCanvasDic[cc].Write()
    TCanvasDic[cc].SaveAs("FitCheck_Ch%i.png"%(cc))







