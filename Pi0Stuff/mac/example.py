import sys
from ROOT import gSystem
gSystem.Load("libImages_Pi0Stuff")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Pi0Stuff..."

sys.exit(0)

