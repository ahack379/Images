import sys,array
from ROOT import *
#from pylab import *

#Adding some stuff for skimage
import numpy as np
import matplotlib.pyplot as plt 
from scipy import ndimage as ndi 
import cv2

fout=TFile.Open("cvContours.root","RECREATE")

run=TChain("event_tree")
run.AddFile(sys.argv[1])
c=TCanvas("c","",600,500)

run.GetEntries()

N_bins   = 200
n_planes = 3
event = 0

# e is the current event; p is the plane
def getImage(e,plane,n_bins):

  if(plane > 2):
    plane-=3

  arr = []

  for i in xrange(len(e.wire_v[plane])):
    arr.append((e.wire_v[plane][i],e.time_v[plane][i],e.charge_v[plane][i]))

  high_x = e.x_max[plane] +50 
  low_x  = e.x_min[plane] -50 
  high_y = e.y_max[plane] +50 
  low_y  = e.y_min[plane] -50 
  high_q = e.q_max[plane] 

  im = np.zeros((n_bins, n_bins))

  div_x = (high_x - low_x)/n_bins
  div_y = (high_y - low_y)/n_bins

  arr = list(arr)

  for a in arr:
    x = (a[0] - low_x)/div_x
    y = (a[1] - low_y)/div_y
    q = 256 * a[2] / high_q
    if q > 256:
      q = 256
    im[x,y] += q 

  return im 


for e in run:

  thresh = 5 

  for i in xrange(n_planes):
    print "plane: ", i 

    maker = imutil.ContourTreeMaker("event_%g_plane_%g" % (event,i) )

    im = getImage(e,i,N_bins)
    im = np.uint8(im) 

    blur = cv2.blur( im, (3,3))
    canny_output = cv2.Canny( blur, thresh, 2*thresh, 3 )
    _, found_contours, hier = cv2.findContours(canny_output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    contour_index = 0 
    print "N contours: ", len(found_contours)
    for found_contour in found_contours:
      area = cv2.contourArea(found_contour)
      if area > 30:
        print "Area and length of contour : ", area, ", " , len(found_contour)
        for pt in found_contour : 
          x_value = ( pt[0][0] * (e.x_max[i%n_planes] + 50 - e.x_min[i%n_planes] + 50 )/N_bins + e.x_min[i%n_planes] - 50 )
          y_value = ( pt[0][1] * (e.y_max[i%n_planes] + 50 - e.y_min[i%n_planes] + 50 )/N_bins + e.y_min[i%n_planes] - 50 )

          maker.FillContour(contour_index, x_value, y_value)
        maker.Fill()
        contour_index += 1

    maker.Write()

  event += 1

fout.Close()
