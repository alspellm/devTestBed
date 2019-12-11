import ROOT as r
import numpy as np
from copy import deepcopy
r.gROOT.SetBatch(True)
from optparse import OptionParser
parser=OptionParser()
parser.add_option("--if", dest="inFile", help="inputFile",default="")
parser.add_option("--ifb", dest="inFileB", help="input File for baseline data", default="")
parser.add_option("--s", dest="sensor", help="Sensor. For all sensors, type 'all'", default=[])
parser.add_option("--cc", dest="channels", help="List of Channels", default=[])
parser.add_option("--of", dest="outFile", help="output File", default="fitTest.root")
(options, args) = parser.parse_args()

#In and Out File Settings
inFile=r.TFile(options.inFile)
if options.inFileB != "":
    inFileBaseline=r.TFile("/nfs/slac/g/hps3/svtTests/jlabSystem/baselines/fits/"+options.inFileB)
outFile=r.TFile(options.outFile, "RECREATE")

def GetSensors():
    layers=["L0T", "L0B", "L1T", "L1B","L2T","L2B","L3T","L3B","L4T","L4B","L5T","L5B","L6T","L6B"]
    sensor=[]
    for i in range(len(layers)):
        if i<8:
            sensor.append(layers[i]+"_axial")
            sensor.append(layers[i]+"_stereo")
        if i>7:
            sensor.append(layers[i]+"_axial_ele")
            sensor.append(layers[i]+"_axial_pos")
            sensor.append(layers[i]+"_stereo_ele")
            sensor.append(layers[i]+"_stereo_pos")
    return sensor




#DeepCopy of all required fit parameters, and original histograms, for one sensor, which are stored in a ROOT file that is output by the gFit executable
def DeepCopyGFit(inFile,sensor):
    sensor_prefix="raw_hits_"
    sensor_suffix="_timesample_0"
    hh=deepcopy(getattr(inFile,(sensor_prefix+"%s"%(sensor)+sensor_suffix+"_hh")))
    mean_h=deepcopy(getattr(inFile,("mean_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_h")))
    sigma_h=deepcopy(getattr(inFile,("width_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_h")))
    norm_h=deepcopy(getattr(inFile,("norm_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_h")))
    FRL_h=deepcopy(getattr(inFile,("FitRangeLower_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_h")))
    FRU_h=deepcopy(getattr(inFile,("FitRangeUpper_"+sensor_prefix+"%s"%(sensor)+sensor_suffix+"_h")))
    return (hh, mean_h, sigma_h, norm_h, FRL_h, FRU_h)

#Define TF1 fit function using parameters extracted from fitting analysis ROOT file
def fitFunc(mean, sigma, norm, FRLD, FRUD):
    fit_func=r.TF1("fit_func","gaus")
    fit_func.SetRange(FRLD,FRUD)
    fit_func.SetParameter(0,norm)
    fit_func.SetParameter(1,mean)
    fit_func.SetParameter(2,sigma)
    return fit_func
def str_to_hw(sensor):
    dic={"L0T_axial":"F0H0","L0T_stereo":"F0H1","L1T_axial":"F0H2","L1T_stereo":"F0H3","L1B_axial":"F1H0","L1B_stereo":"F1H1","L0B_axial":"F1H2","L0B_stereo":"F1H3","L2T_axial":"F2H0","L2T_stereo":"F2H1","L3T_axial":"F2H3","L3T_stereo":"F2H2","L2B_stereo":"F3H0","L2B_axial":"F3H1","L3B_stereo":"F3H2","L3B_axial":"F3H3","L4T_axial_ele":"F4H0","L4T_axial_pos":"F4H1","L4T_stereo_ele":"F4H2","L4T_stereo_pos":"F4H3","L4B_axial_ele":"F5H2","L4B_axial_pos":"F5H3","L4B_stereo_ele":"F5H0","L4B_stereo_pos":"F5H1","L5T_axial_ele":"F6H0","L5T_axial_pos":"F6H1","L5T_stereo_ele":"F6H2","L5T_stereo_pos":"F6H3","L5B_axial_ele":"F7H2","L5B_axial_pos":"F7H3","L5B_stereo_ele":"F7H0","L5B_stereo_pos":"F7H1","L6T_axial_ele":"F8H0","L6T_axial_pos":"F8H1","L6T_stereo_ele":"F8H2","L6T_stereo_pos":"F8H3","L6B_axial_ele":"F9H2","L6B_axial_pos":"F9H3","L6B_stereo_ele":"F9H0","L6B_stereo_pos":"F9H1"}
    return dic[sensor]

#DeepCopy of the baseline run data pre-installation
#/nfs/slac/g/hps3/svtTests/jlabSystem/baselines/fits
def DeepCopyBaseline(inFile,sensor):
   prefix="baseline_"
   suffix="_ge"
   sensor=str_to_hw(sensor) 
   bg=deepcopy(getattr(inFile,("baseline/"+prefix+"%s"%(sensor)+suffix)))
   return bg

def GetBinContent(histo,cc):
    histo.GetBinContent(cc+1)

#####################################################################################################

if options.sensor=="all":
    sensor=GetSensors()
else:
    sensor=options.sensor.split(',')
print sensor
channels=options.channels.split(',')
for i in range(len(channels)):
    channels[i]=int(channels[i])
TCanvasDic={}
for e in sensor:    
    TCanvasDic.update({e:r.TCanvas(e)})
    TCanvasDic[e].cd()

    baseline_g=DeepCopyBaseline(inFileBaseline,e)
    baseline_g.Draw()
    (hh, mean_h, sigma_h, norm_h, FRL_h, FRU_h)=DeepCopyGFit(inFile, e)
    x=np.arange(hh.GetNbinsX(),dtype=float)
    y=[]
    sigy=[]
    for cc in range(hh.GetNbinsX()):  ###Check is this possible bug?
        mean=mean_h.GetBinContent(cc+1)
        sigma=sigma_h.GetBinContent(cc+1)
        norm=norm_h.GetBinContent(cc+1)
        FRL=FRL_h.GetBinContent(cc+1)
        FRU=FRU_h.GetBinContent(cc+1)
        fit_func=fitFunc(mean,sigma,norm,FRL,FRU)
        y.append(mean)
        sigy.append(sigma)
    fit_g=r.TGraphErrors(hh.GetNbinsX(),x,np.array(y),np.zeros(hh.GetNbinsX(),dtype=float),np.array(sigy))        
    baseline_g.Draw()
    fit_g.SetLineColor(2)
    fit_g.Draw("SAME")
    TCanvasDic[e].Write()
    TCanvasDic[e].SaveAs("images/%sBaseline_Comparison.png"%(e))


#######################################################################################
gaus=False
if gaus==True:
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







