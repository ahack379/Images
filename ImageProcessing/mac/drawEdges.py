import sys,array
from ROOT import *
#from pylab import *

#Adding some stuff for skimage
import numpy as np
import matplotlib.pyplot as plt 
from scipy import ndimage as ndi 
from skimage import feature, measure 

run=TChain("event_tree")
run.AddFile(sys.argv[1])
c=TCanvas("c","",600,500)

run.GetEntries()

N_bins = 200

# e is the current event; p is the plane
def getImage(e,plane,n_bins):

  if(plane > 2):
    plane-=3

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

for e in run:

  ax = []

  fig, (ax0) = plt.subplots(nrows=1,ncols=1) 

  ax.append(ax0)

#  for i in xrange(3):

  im = getImage(e,1,N_bins)
    
  edges = feature.canny(im, sigma=3)

  ax0.imshow(im, cmap=plt.cm.winter)
#    ax[i].imshow(edges,cmap=plt.cm.gray)
  ax0.set_title('Plane %g ' % 0)


  fig.subplots_adjust(wspace=0.02, hspace=0.02, top=0.9,
                        bottom=0.02, left=0.02, right=0.98)

  fig.savefig('edges.jpg')


  plt.draw()

  break

plt.show()
