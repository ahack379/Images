#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $LARLITE_USERDEVDIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN LARLITE_USERDEVDIR & PWD =>
#   do not compile if PWD !=$LARLITE_USERDEVDIR is set elsewhere
#
ifndef LARLITE_USERDEVDIR
ERROR_MESSAGE := $(error LARLITE_USERDEVDIR is not defined!)
endif
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
SUBDIRS := ImageProcessing #ADD_NEW_SUBDIR ... do not remove this comment from this line

#####################################################################################
#
# COMPILATION...
#
#.phony: all configure default-config clean
.phony: all clean

all:
<<<<<<< HEAD
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $(LARLITE_USERDEVDIR)/Images/$$i && $(MAKE) ) || exit $$?; done
=======
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $(LARLITE_USERDEVDIR)/ImageProcessing/$$i && $(MAKE) ) || exit $$?; done
>>>>>>> 60e2c16114d06b5063b48cba8eebb0b149534eed
#####################################################################################
#
# CLEANs...
#
clean:
<<<<<<< HEAD
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $(LARLITE_USERDEVDIR)/Images/$$i && $(MAKE) clean && rm -f $(LARLITE_LIBDIR)/$$i.* ) || exit $$?; done
=======
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $(LARLITE_USERDEVDIR)/ImageProcessing/$$i && $(MAKE) clean && rm -f $(LARLITE_LIBDIR)/$$i.* ) || exit $$?; done
>>>>>>> 60e2c16114d06b5063b48cba8eebb0b149534eed

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
<<<<<<< HEAD
	@mkdir -p $(LARLITE_USERDEVDIR)/Images/doc/dOxygenMyProject
	@doxygen $(LARLITE_USERDEVDIR)/Images/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(LARLITE_USERDEVDIR)/Images/doc/dOxygenMyProject+
	@doxygen $(LARLITE_USERDEVDIR)/Images/doc/doxygenMyProject+.script
=======
	@mkdir -p $(LARLITE_USERDEVDIR)/ImageProcessing/doc/dOxygenMyProject
	@doxygen $(LARLITE_USERDEVDIR)/ImageProcessing/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(LARLITE_USERDEVDIR)/ImageProcessing/doc/dOxygenMyProject+
	@doxygen $(LARLITE_USERDEVDIR)/ImageProcessing/doc/doxygenMyProject+.script
>>>>>>> 60e2c16114d06b5063b48cba8eebb0b149534eed
#
#####################################################################################
#EOF
