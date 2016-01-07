import sys,array
from ROOT import *
#from pylab import *

#Adding some stuff for skimage
import numpy as np
import matplotlib.pyplot as plt 
from scipy import ndimage as ndi 
import cv2

fout=TFile.Open("temp.root","RECREATE")

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

  images = []
  ax = []

  fig, ((ax5,ax4,ax3),(ax5E,ax4E,ax3E),(ax2,ax1,ax0)) = plt.subplots(nrows=3,ncols=3) 
#  fig, (ax2, ax1, ax0) = plt.subplots(nrows=1,ncols=3) 
  src = cv2.imread('pi0.png')

  ax.append(ax0)
  ax.append(ax1)
  ax.append(ax2)
  ax.append(ax3)
  ax.append(ax4)
  ax.append(ax5)
  ax.append(ax3E)
  ax.append(ax4E)
  ax.append(ax5E)

  thresh = 5 

  for i in xrange(6):

    im = getImage(e,i,N_bins)
    im = np.uint8(im) 

#    cvtColor needs 3 or 4 dimensions to function properly
#    blur = cv2.pyrDown(im,(2*len(im),2*len(im[0])))
    blur = cv2.blur( im, (3,3))
    canny_output = cv2.Canny( blur, thresh, 2*thresh, 3 )
    _, contours, hier = cv2.findContours(canny_output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if i > 2:  
      ax[i].imshow(im) #, cmap=plt.cm.winter)
      ax[i+3].imshow(canny_output,cmap=plt.cm.gray)
      ax[i].set_title('Plane %g ' % (i-3))
    else:
      print "Plane: ", i
      for n, c in enumerate(contours):
          ar = c[:,0]
        
          temp_x = ar[:len(ar),0]
          temp_y = ar[:len(ar),1]

          area = cv2.contourArea(c)
          if area > 30 :
            print "Area is: ", area
            ax[i].plot(temp_x, temp_y,linewidth= 2)
	    ax[i].axis([0,200,200,0]) 

    fig.subplots_adjust(wspace=0.02, hspace=0.02, top=0.9,
                          bottom=0.02, left=0.02, right=0.98)

  plt.draw()
  plt.show(block=False)
  sys.stdin.readline()
