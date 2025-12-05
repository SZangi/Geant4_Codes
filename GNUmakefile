# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------
#
# Adds functionality that allows us to link the Root libraries
# during compilation, to use ROOT functionality in our program.

name := dtTest
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

# Prevent shadow warnings needed to suppress ROOT/Geant4 shadowing
# mainly of global variable "s". This may come back to bite me...
CXXFLAGS := $(subst -Wshadow,,$(CXXFLAGS))


##########
#  ROOT  #
##########

# Several ROOT classes are used in ScintEval for their utility; this
# requires compiling and linking against ROOT as a dependency. Use
# 'root-config' to obtain the header and library locations.
ROOTINCLUDES = -I$(shell root-config --incdir)
ROOTDISTLIBS = $(shell root-config --nonew --libs --glibs)

CPPFLAGS += $(ROOTINCLUDES)
LDLIBS += $(ROOTDISTLIBS)


.PHONY:
