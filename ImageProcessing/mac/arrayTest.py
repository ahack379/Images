from ROOT import *
from pylab import *

f = TFile.Open('hit_trees.root')
tree = f.Get('hits_event_0_plane_0')



#tree=TTree('hits_event_0_plane_0','')
#tree.AddFile('hit_trees.root')
#tree.GetEntry(0)
#print "run siez: ", tree.hits
#c=TCanvas("c","",600,500)
for entry in xrange(tree.GetEntries()):

  tree.GetEntry(entry)
  hit_array = tree.hits
  print tree.hits

  for hit_index in xrange(hit_array.size()):
 
    print 'Hit',hit_index,'...',
 
    contour = hit_array[hit_index]
 
    # Loop over coordinates
    for coordinate_index in xrange(contour.size()):
 
        coordinate = contour_array[hit_index][coordinate_index]
 
        print '(%.2g,%.2g)' % (coordinate.first,coordinate.second),





#for h in run.GetEntries():
#  print "do we come here" , h
#  print "h entries: ", run.hits[0]


#
#x4=[]
#y4=[]
#arr4 = arr3[0:len(arr3)/2]
#arr4.append(arr3[0])
#
#for a in arr4:
#  x4.append(a[0])
#  y4.append(a[1])
#
#x5=[]
#y5=[]
#
#for h in hits2:
#  x5.append(h[0])
#  y5.append(h[1])
#
# 
#
#plot(x,y) #,'+')
#plot(x4,y4) #,'+')
##plot(x5,y5,'+')
#show()
