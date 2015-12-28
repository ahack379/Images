Description of contents:

Image.cxx:
Stores hit information from full view of an event based on hit information. Currently this tree information can be passed to mac/makeContours.py to get contours information readout.  This will be dealt with better soon, but for now, this is the description.

checkInside.cxx:
When passed a list of coordinates that describe a contour (from mac/makeContours.py), this loops over all hits to determine which contours/clusters they belong to. Currently hard coded and crappy. 

Scripts:
mac/analyze.py      : Make tree output from Image.cxx
mac/makeContours.py : Makes contours based on scikit image processing of located images

