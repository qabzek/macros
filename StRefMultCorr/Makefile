# Define compiler
CXX = g++

# Define flags. -D_VANILLA_ROOT_ is needed to avoid StMessMgr confusion
# CFLAGS = $(shell root-config --cflags) -O2 -fPIC -Wall -W -Woverloaded-virtual -Wno-deprecated-declarations
CFLAGS = $(shell root-config --cflags) -O2 -fPIC -Wall -pipe -std=c++14 -D_VANILLA_ROOT_ -I.
# CFLAGS += -pipe -std=c++11 -D_VANILLA_ROOT_ -I.
LIBS = $(shell root-config --libs)
INCS = $(shell root-config --incdir)

# Define output library
STREFMULTCORR = libStRefMultCorr.so

# Compile all *.cxx classes in the directory
SRC = $(shell find . -name "*.cxx")

all: $(STREFMULTCORR)

# $(SRC:.cc=.o)
$(STREFMULTCORR): $(SRC:.cxx=.o) StRefMultCorr_Dict.C
	$(CXX) $(CFLAGS) -shared $^ -o $(STREFMULTCORR) $(LIBS)

%.o: %.cxx
	$(CXX) -fPIC $(CFLAGS) -c -o $@ $<

StRefMultCorr_Dict.C: $(shell find . -name "*.h" ! -name "*LinkDef*")
	rootcint -f $@ -c -D_VANILLA_ROOT_ -DROOT_CINT -D__ROOT__ -I. -I$(INCS) $^ StRefMultCorrLinkDef.h

.PHONY: clean distclean

clean:
	rm -vf *.o StarRoot/*.o StRefMultCorr_Dict*

distclean:
	rm -vf *.o StarRoot/*.o StRefMultCorr_Dict* $(STREFMULTCORR)
