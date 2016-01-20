#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = Images_ClusteringDict
SHLIB = libImages_Clustering.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)
INCFLAGS += -I/usr/local/include/
#INCFLAGS += -I$(LARLITE_USERDEVDIR)/OpenCV
INCFLAGS += -I$(LARLITE_USERDEVDIR)/Images

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

LDFLAGS = $(shell larlite-config --libs)
#LDFLAGS += -L$(shell python-config --prefix)/lib/
LDFLAGS += $(shell python-config --ldflags)
LDFLAGS += -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_features2d -lopencv_imgproc
#LDFLAGS += -L$(LARLITE_LIBDIR) -lOpenCV_Utils
LDFLAGS += -L$(LARLITE_LIBDIR) -lImages_ImageProcessing
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtilAlg

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
