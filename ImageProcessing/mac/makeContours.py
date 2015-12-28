import sys,array
from ROOT import *
#from pylab import *

#Adding some stuff for skimage
import numpy as np
import matplotlib.pyplot as plt 
from scipy import ndimage as ndi 
from skimage import feature, measure 

fout=TFile.Open("temp.root","RECREATE")
#fout=TFile.Open("ana.root","WRITE")

#t = TTree("ctours","ctours")
#con = np.zeros(5,float)
#con2 = std.vector(std.vector(std.pair('double','double')))() #'double'))()
#t.Branch('Contour',con,'con/D')

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

  images = []
  ax = []

  fig, ((ax5,ax4,ax3),(ax5E,ax4E,ax3E),(ax2,ax1,ax0)) = plt.subplots(nrows=3,ncols=3) #figsize=(8,3))#,sharex=True,sharey=True)

  ax.append(ax0)
  ax.append(ax1)
  ax.append(ax2)
  ax.append(ax3)
  ax.append(ax4)
  ax.append(ax5)
  ax.append(ax3E)
  ax.append(ax4E)
  ax.append(ax5E)


  for i in xrange(6):

    im = getImage(e,i,N_bins)
    
    edges = feature.canny(im, sigma=3)
    contours = measure.find_contours(edges, 0.8,'high')
     

#    con2.resize(len(contours))    
#    for c in xrange(len(contours)):
#      for l in contours[c] :
#        print "c and ll: ", c, l[0], l[1]
#        ugh.append(std.make_pair(l[0],l[1]))
#      con2.push_back(ugh)
     
    if i > 2: 
      ax[i].imshow(im, cmap=plt.cm.winter)
      ax[i+3].imshow(edges,cmap=plt.cm.gray)
      ax[i].set_title('Plane %g ' % (i-3))
    else:
      ax[i].imshow(im,interpolation='nearest', cmap=plt.cm.winter)
      for n, contour in enumerate(contours):
        ax[i].plot(contour[:,1], contour[:,0],linewidth= 2)
      ax[i].axis('image')


    if i is 2:
      for c in contours[1]:
	temp = c
        temp[0] *= (e.x_max[i] + 50 - e.x_min[i] + 50 )/N_bins
	temp[0] += e.x_min[i] - 50

        temp[1] *= (e.y_max[i] - e.y_min[i] + 100)/N_bins
	temp[1] += e.y_min[i] - 50 
	print temp
    

  fig.subplots_adjust(wspace=0.02, hspace=0.02, top=0.9,
                        bottom=0.02, left=0.02, right=0.98)

  plt.draw()

  break

plt.show()
#fout.Close()

