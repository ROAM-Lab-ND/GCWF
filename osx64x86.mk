
CC=clang++
IPATHS=-I$(MSKPATH)/h -I$(MSKPATH)/include
LPATHS=-L$(MSKPATH)/bin

# Compile MOSEK

fusion:
	make install -C $(MSKPATH)/src/fusion_cxx


$(BUILDPATH)CWC: fusion CWC_MOSEK/CWC.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC CWC_MOSEK/CWC.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.$(MSKVERSION).dylib $(MSKPATH)/bin/libfusion64.$(MSKVERSION).dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC
	install_name_tool -change libmosek64.$(MSKVERSION).dylib $(MSKPATH)/bin/libmosek64.$(MSKVERSION).dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC

$(BUILDPATH)CWC_Lin: fusion CWC_MOSEK/CWC_Lin.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC_Lin CWC_MOSEK/CWC_Lin.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.$(MSKVERSION).dylib $(MSKPATH)/bin/libfusion64.$(MSKVERSION).dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin
	install_name_tool -change libmosek64.$(MSKVERSION).dylib $(MSKPATH)/bin/libmosek64.$(MSKVERSION).dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin

.PHONY : CWC_MOSEK
CWC_MOSEK: $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin

# Compile GCWF

HEADERS=$(shell find ./GCWF -type f -name *.hpp)

$(BUILDPATH)LCL: GCWF/LongestCenteredLine.cpp $(HEADERS)
	$(CC) -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)LCL GCWF/LongestCenteredLine.cpp

$(BUILDPATH)EAV: GCWF/EnumerateAllVertices.cpp $(HEADERS)
	$(CC) -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)EAV GCWF/EnumerateAllVertices.cpp

$(BUILDPATH)Grid: GCWF/Grid.cpp $(HEADERS)
	$(CC) -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)Grid GCWF/Grid.cpp

.PHONY : GCWF
GCWF: $(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid
