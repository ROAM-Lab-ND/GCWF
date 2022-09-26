
include mosekpath.mk

BUILDPATH=./build/

CC=clang++
IPATHS=-I$(MOSEKPATH)/platform/osx64x86/h -I$(MOSEKPATH)/platform/osx64x86/include
LPATHS=-L$(MOSEKPATH)/platform/osx64x86/bin


# Compile all

.PHONY : all

all: CWC_MOSEK GCWF


# Compile MOSEK

fusion:
	make install -C $(MOSEKPATH)/platform/osx64x86/src/fusion_cxx


$(BUILDPATH)CWC: fusion CWC_MOSEK/CWC.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC CWC_MOSEK/CWC.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libfusion64.9.3.dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC
	install_name_tool -change libmosek64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libmosek64.9.3.dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC

$(BUILDPATH)CWC_Lin: fusion CWC_MOSEK/CWC_Lin.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC_Lin CWC_MOSEK/CWC_Lin.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libfusion64.9.3.dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin
	install_name_tool -change libmosek64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libmosek64.9.3.dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin

.PHONY : CWC_MOSEK
CWC_MOSEK: $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin

# Compile GCWF

HEADERS=$(shell find ./GCWF -type f -name *.hpp)
	
$(BUILDPATH)LCL: GCWF/LongestCenteredLine.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)LCL GCWF/LongestCenteredLine.cpp
	
$(BUILDPATH)EAV: GCWF/EnumerateAllvertices.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)EAV GCWF/EnumerateAllvertices.cpp

$(BUILDPATH)Grid: GCWF/Grid.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)Grid GCWF/Grid.cpp

.PHONY : GCWF
GCWF: $(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid


# Clean

.PHONY : clean

clean:
	-rm -rf $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin\
		$(BUILDPATH)CWC.dSYM $(BUILDPATH)CWC_Lin.dSYM\
		$(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid
	
clean_MOSEK:
	make clean -C $(MOSEKPATH)/platform/osx64x86/src/fusion_cxx
