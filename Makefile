# stuff to make
#
ifndef ROOTSYS
all:
	@echo "ROOTSYS is not set. Please set ROOT environment properly"; echo
else

all: 	build
help:
	@echo "Available Targets:";\
	cat Makefile | perl -ne 'printf("\t%-15s %s\n",$$1,$$2) if(/^(\S+):[^#]+(#.*)$$/)'

ifndef VERBOSE
  QUIET := @
endif

CC=g++
CLING=rootcling
DIR=.
COREDIR=CORE
TOOLSDIR=CORE/Tools
INCLUDE=$(shell root-config --cflags) -I$(DIR)/ -I$(DIR)/CORE
CFLAGS=-Wall -Wno-unused-function -g -O2 $(INCLUDE)

DICTINCLUDE=$(ROOTSYS)/include/Math/QuantFuncMathCore.h $(ROOTSYS)/include/TLorentzVector.h $(ROOTSYS)/include/Math/Vector4D.h

LINKER=g++
LINKERFLAGS=$(shell root-config --ldflags --libs) -lEG -lGenVector -lTMVA 

CORESOURCES=$(DIR)/$(COREDIR)/CMS3.cc \
 $(DIR)/$(COREDIR)/Base.cc \
 $(DIR)/$(COREDIR)/ElectronSelections.cc \
 $(DIR)/$(COREDIR)/MuonSelections.cc \
 $(DIR)/$(COREDIR)/JetSelections.cc \
 $(DIR)/$(COREDIR)/MetSelections.cc \
 $(DIR)/$(COREDIR)/VertexSelections.cc \
 $(DIR)/$(COREDIR)/TriggerSelections.cc \
 $(DIR)/$(COREDIR)/MCSelections.cc \
 $(DIR)/$(COREDIR)/IsolationTools.cc \
 $(DIR)/$(COREDIR)/WWSelections.cc \
 $(DIR)/$(TOOLSDIR)/MT2/MT2Utility.cc \
 $(DIR)/$(TOOLSDIR)/MT2/MT2.cc \
 $(DIR)/$(TOOLSDIR)/dorky/dorky.cc \
 $(DIR)/$(TOOLSDIR)/goodrun.cc \
 $(DIR)/$(TOOLSDIR)/utils.cc \
 $(DIR)/$(TOOLSDIR)/JetCorrector.cc \
 $(DIR)/$(TOOLSDIR)/badEventFilter.cc

COREOBJECTS=$(CORESOURCES:.cc=.o)

SOURCES=$(wildcard $(DIR)/*.cc)
OBJECTS=$(SOURCES:.cc=.o)
DICT=LinkDef_out.o
LIBS=$(LIB)
EXE=main.exe

.PRECIOUS: $(DICT) $(OBJECTS) $(COREOBJECTS)

#
# how to make it
#

LinkDef_out.cxx: LinkDef.h
	$(QUIET) echo "Making dictionaries";
	$(CLING) -f $@ -c -p $(DICTINCLUDE) $<

%.exe:  $(DICT) $(OBJECTS) $(COREOBJECTS)
	$(QUIET) echo "Building $@";
	$(CC) -o $@ $(LINKERFLAGS) $(DICT) $(COREOBJECTS) $(OBJECTS) 

%.o: 	%.cc %.h
	$(QUIET) echo "Compiling $<";
	$(CC) $(CFLAGS) $< -c -o $@

%.o: 	%.cc
	$(QUIET) echo "Compiling $<";
	$(CC) $(CFLAGS) $< -c -o $@

%.o:    %.cxx 
	$(QUIET) echo "Compiling $<";
	$(CC) $(CFLAGS) $< -c -o $@

build:  $(EXE)

b: build

loopclean:
	rm -f \
	*_out.*	 \
	*.o \
	*.*~ \
	main.exe

clean: loopclean
	rm -f \
	$(CORELIB) \
	$(DIR)/$(COREDIR)/*.o \
	$(DIR)/$(TOOLSDIR)/*.o \
	$(DIR)/$(TOOLSDIR)/MT2/*.o \
	$(DIR)/$(TOOLSDIR)/dorky/*.o \

endif
