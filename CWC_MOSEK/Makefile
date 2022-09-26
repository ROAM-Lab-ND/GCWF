
#
# Compile MOSEK
#
#
# This makefile is not in use

MOSEKPATH=/your/path/to/mosek/

BUILDPATH=../build/

CC=clang++
IPATHS=-I$(MOSEKPATH)/platform/osx64x86/h -I$(MOSEKPATH)/platform/osx64x86/include
LPATHS=-L$(MOSEKPATH)/platform/osx64x86/bin

fusion:
	make install -C $(MOSEKPATH)/platform/osx64x86/src/fusion_cxx


CWC: fusion CWC.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC CWC.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libfusion64.9.3.dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC
	install_name_tool -change libmosek64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libmosek64.9.3.dylib $(BUILDPATH)CWC || rm -f $(BUILDPATH)CWC

CWC_Lin: fusion CWC_Lin.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o $(BUILDPATH)CWC_Lin CWC_Lin.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libfusion64.9.3.dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin
	install_name_tool -change libmosek64.9.3.dylib $(MOSEKPATH)/platform/osx64x86/bin/libmosek64.9.3.dylib $(BUILDPATH)CWC_Lin || rm -f $(BUILDPATH)CWC_Lin


all: CWC CWC_Lin

.PHONY : clean

clean:
	-rm -rf $(BUILDPATH)CWC $(BUILDPATH)CWC_Lin $(BUILDPATH)CWC.dSYM $(BUILDPATH)CWC_Lin.dSYM
	
clean_MOSEK:
	make clean -C $(MOSEKPATH)/platform/osx64x86/src/fusion_cxx
