import sys
from ROOT import gSystem
gSystem.Load("libImages_ImageProcessing")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing ImageProcessing..."

sys.exit(0)
