from ROOT import *
from array import array
from subprocess import call

import math
import os
import sys

# set input parameter
entrance_collimators = 20 # mm
if len(sys.argv) > 1:
    entrance_collimators = float(sys.argv[1])

# options
SavePlots = True
save_base = "opening"
save_files = []
result_file = 'exit_fit_entrance=%dmm.pdf' % entrance_collimators

gStyle.SetOptStat("emr")
gStyle.SetOptFit(1111)

# Statistics and fitting results replaced in:
gStyle.SetStatX(0.6);    # Top right corner.
gStyle.SetStatY(0.6);

exit_opening = range(1,21)
# exit_opening = [12]

x = exit_opening
y = []
dy = []

for d in exit_opening:
# d = 1;
    f = TFile("/exp/leetech/simulations/ExitCollScan/ExitCollScan_Entr%dmm/opening=%dmm/leetech.root" % (entrance_collimators, d))
    tree = f.Get("AfterExitWindow")
    N = tree.GetEntriesFast()
    
    # Define histograms (name, title, number of bins, minimum, maximum):
    Hist_x = TH1F("Hist_x", "Hist_x", 1000, 0., 4.)
    
    for event in tree:
        Hist_x.Fill(event.Energy)
    
    # Plot result:
    # ------------
    canvas = TCanvas( "canvas", "canvas", 50, 50, 1200, 600 )
    
    Hist_x.SetTitle("LEETECH exit energy fit, d_exit=%d mm" % d)
    Hist_x.GetXaxis().SetRangeUser(1., 3.)
    Hist_x.GetXaxis().SetTitle("Energy, MeV")
    Hist_x.GetYaxis().SetTitle("Counts")
    Hist_x.SetLineColor(kBlue)
    Hist_x.SetLineWidth(2)
    Hist_x.Draw()                     # The option "e" shows errors (Poisson!)
    
    # Fitting histogram (with predefined function):
    fit_func = TF1("fit_func", "gaus", 0., 3.)
    
    fit_func.SetRange(2.65, 2.8);
    # fit_func.SetParLimits(0, N/2, 1e5)
    fit_func.SetParLimits(1, 2.65, 2.74)
    fit_func.SetParLimits(2, 0.001, 0.15)
    
    fit_func.SetParameters(N, 2.73, 0.01);
    
    fit_func.SetLineColor(kRed-3)
    fit_func.SetLineWidth(3)
    Hist_x.Fit("fit_func")
    
    y.append(fit_func.GetParameter(2))
    dy.append(fit_func.GetParError(2))
    
    # Example of how to get e.g. means from a histogram and result of fit:
    print "Means:   mu_x = %6.3f"%(Hist_x.GetMean())
    print "Fitted mean of x:   mu_hat = %6.3f +- %5.3f"%(fit_func.GetParameter(1), fit_func.GetParError(1))
    
    # Legend:
    leg = TLegend( 0.15, 0.70, 0.38, 0.85 )
    leg.SetFillColor(kWhite)
    leg.SetLineColor(kWhite)
    leg.AddEntry(Hist_x, " Detector hits", "L")
    leg.AddEntry(fit_func, " Gaussian fit", "L")
    leg.Draw()
    
    canvas.Update()
#     if (SavePlots):
#         file_name = "%s=%dmm.pdf" % (save_base, d)
#         save_files.append(file_name)
#         canvas.SaveAs(file_name)
        
g = TGraph(len(x), array("d", x), array("d", y))
  
g.Draw("A*")
g.SetTitle("")
g.GetXaxis().SetTitle("Exit collimators opening, mm")
g.GetYaxis().SetTitle("Sigma of exit Gaussian fit")
  
canvas.SetGrid()
canvas.Update()
 
if (SavePlots):
    file_name = "%s_%s.pdf" % (save_base, 'fit')
    save_files.append(file_name)
    canvas.SaveAs(file_name)
    
    with open("%s.txt" % result_file[:-4], 'w') as out_data:
        template = "{0:20}{1:20}{2:20}"
        out_data.write(template.format("#opening, mm", "sigma, MeV", "sigma error, MeV"))
        out_data.write('\n')
        for i in range(len(exit_opening)):
            out_data.write(str(x[i]).ljust(20) + str(y[i]).ljust(20) + str(dy[i]).ljust(20) + "\n")
     
#     call(['pdfunite'] + save_files + [result_file])
#     for f in save_files:
#         os.remove(f)
    
# ---------------------------------------------------------------------------------- 
raw_input('Press Enter to exit')