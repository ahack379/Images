import sys,array
from ROOT import *
#from pylab import *

#Adding some stuff for skimage
import numpy as np
import matplotlib.pyplot as plt 
from scipy import ndimage as ndi 
from skimage import feature, measure 

fout=TFile.Open("contours.root","RECREATE")

run=TChain("event_tree")
run.AddFile(sys.argv[1])
c=TCanvas("c","",600,500)

run.GetEntries()

N_bins = 200
n_planes = 3


# e is the current event; p is the plane
def getImage(e,plane,n_bins):

  if(plane > (n_planes -1)):
    plane-= n_planes

  arr = []

  for i in xrange(len(e.wire_v[plane])):
    arr.append((e.wire_v[plane][i],e.time_v[plane][i]))

  high_x = e.x_max[plane] +50 
  low_x  = e.x_min[plane] -50 
  high_y = e.y_max[plane] +50 
  low_y  = e.y_min[plane] -50 

  im = np.zeros((n_bins, n_bins))

  div_x = (high_x - low_x)/n_bins
  div_y = (high_y - low_y)/n_bins

  arr = list(arr)

  for a in arr:
    x = (a[0] - low_x)/div_x
    y = (a[1] - low_y)/div_y
    im[x,y] = 1

  return im 

event = 0
for e in run:
  
  print "new event" 

  for i in xrange(n_planes):

    maker = imutil.ContourTreeMaker("event_%g_plane_%g" % (event,i) )

    im = getImage(e,i,N_bins)
    
    edges = feature.canny(im, sigma=3)
    found_contours = measure.find_contours(edges, 0.8,'high')

    contour_index = 0
    for found_contour in found_contours:
      for pt in found_contour : 
	temp = pt
        temp[0] *= (e.x_max[i%n_planes] + 50 - e.x_min[i%n_planes] + 50 )/N_bins
	temp[0] += e.x_min[i%n_planes] - 50

        temp[1] *= (e.y_max[i%n_planes] - e.y_min[i%n_planes] + 100)/N_bins
	temp[1] += e.y_min[i%n_planes] - 50 

        x_value,y_value=(float(temp[0]),float(temp[1]))
        maker.FillContour(contour_index, x_value, y_value)

      contour_index += 1

    maker.Fill()
    maker.Write()

  event += 1

fout.Close()
