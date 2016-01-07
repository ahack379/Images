Description of contents:

Image.cxx:
Stores hit information from full view of an event based on hit information. Currently this tree information can be passed to mac/storeContours.py to get contours information.  This will be dealt with better soon.

checkInside.cxx:
When passed a list of coordinates that describe a contour (from output of mac/storeContours.py), this loops over all hits to determine which contours/clusters they belong to. Currently crappy 

Scripts:
Skimage:
mac/runImage.py      : Make tree that stores hit wire-time information using Image class
mac/storeContours.py : Makes + stores contours based on scikit image processing of located images
mac/drawContours.py  : Makes + draws contours based on scikit image processing of located images

mac/runcheckInside.py : Make tree that stores hits in branches according to the contour they belong to and plane they're in.  Will output 3 trees per event (for 3 trees for 3 planes), each of which fills hit vector of pairs per contour association.

OpenCV:
mac/testCV.py         : Attempt to repeat the above drawContours using opencv instead of skimage


To run:
(this is ridiculous right now; needs to change before will become useful):

> python runImage.py reco2dfile.root     --> output file "out.root"
> python storeContours.py out.root       --> output file "contours.root"
> python runcheckInside.py contours.root --> output file "hit\_tree.root"

At this point, I suggest that the hits are merged (in order to test the truth value of that statement, need to run fully through shower reco).  Have not figured out best way to incorporate this into matching framework, and perhaps should not continue along this path at all. Current idea is to convert each group of 'clustered' hits into cluster\_params objects.  If can do this, can pass the output root file to matchign algorithms and see what pops out.

To run testCV.py:
> python testCV.py out.root

Ariana Hackenburg
