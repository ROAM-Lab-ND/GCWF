
CC=g++
IPATHS=-I$(MSKPATH)/h -I$(MSKPATH)/include
LPATHS=-L$(MSKPATH)/bin -Wl,-rpath-link,$(MSKPATH)/bin '-Wl,-rpath=$(MSKPATH)/bin'

# Compile MOSEK

fusion:
	make install -C $(MSKPATH)/src/fusion_cxx

$(BUILDPATH)CWC: fusion CWC_MOSEK/CWC.cc
	$(CC) -pthread -std=c++11 -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC CWC_MOSEK/CWC.cc -lfusion64 -lmosek64

$(BUILDPATH)CWC_Lin: fusion CWC_MOSEK/CWC_Lin.cc
	$(CC) -pthread -std=c++11 -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC_Lin CWC_MOSEK/CWC_Lin.cc -lfusion64 -lmosek64

.PHONY : CWC_MOSEK
CWC_MOSEK: $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin

# Compile GCWF

$(BUILDPATH)LCL: GCWF/LongestCenteredLine.cpp $(HEADERS)
	$(CC) -std=c++11 -O3 -o $(BUILDPATH)LCL GCWF/LongestCenteredLine.cpp

$(BUILDPATH)EAV: GCWF/EnumerateAllVertices.cpp $(HEADERS)
	$(CC) -std=c++11 -O3 -o $(BUILDPATH)EAV GCWF/EnumerateAllVertices.cpp

$(BUILDPATH)Grid: GCWF/Grid.cpp $(HEADERS)
	$(CC) -std=c++11 -O3 -o $(BUILDPATH)Grid GCWF/Grid.cpp

.PHONY : GCWF
GCWF: $(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid
