import os
import re
import getopt
import sys
from array import array
import ROOT

scan_dir = os.getcwd()
opts, args = getopt.getopt(sys.argv[1:], 'd:')
for o, a in opts:
    if o == '-d':
        scan_dir = os.path.abspath(a)

c = ROOT.TCanvas('B field scan')
 
p = re.compile(".*B=(\d+).*")
data_listdir = [os.path.join(scan_dir, d) for d in os.listdir(scan_dir) if os.path.isdir(os.path.join(scan_dir, d))]
 
B = []
hits = []
    
for d in data_listdir:
    m = p.match(d.split("/")[-1])
    if m:
        B.append(int(m.group(1)))
            
        f = ROOT.TFile("%s/Result.root" % d)
        try:
            hits.append(f.Get("T").GetBranch("Energy").GetEntries())
            print d, '\t', hits[-1]
        except:
            hits.append(0)
            print d, '\t', "Empty results file"
        f.Close()
          
          
if B and hits:
    description = scan_dir.split("/")[-1]
    with open(os.path.join(scan_dir, "%s.txt" % description), 'w') as out_file:
        tmp = zip(B, hits)
        tmp.sort(key=lambda tup: tup[0])
        for b_field, n_hits in tmp:
            out_file.write("%d\t\t%d\n" % (b_field, n_hits))
    
    g = ROOT.TGraph(len(B), array("d", B), array("d", hits))
  
    g.Draw("A*")
    g.SetTitle(description)
    g.GetXaxis().SetTitle("B, G")
    g.GetYaxis().SetTitle("Number of events")
 
    c.SetLogy()
    c.SetGrid()
    c.Update()
     
    raw_input('Press <ret> to end -> ')
 
 
 

