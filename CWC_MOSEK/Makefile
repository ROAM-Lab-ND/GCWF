
#
# Compile and run MOSEK
#

CC=clang++
IPATHS=-I/.../mosek/9.3/tools/platform/osx64x86/h -I/.../mosek/9.3/tools/platform/osx64x86/include
LPATHS=-L/.../mosek/9.3/tools/platform/osx64x86/bin

fusion:
	make install -C /.../mosek/9.3/tools/platform/osx64x86/src/fusion_cxx


CWC: fusion CWC.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o CWC CWC.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib `pwd`/.../mosek/9.3/tools/platform/osx64x86/bin/libfusion64.9.3.dylib CWC || rm -f CWC
	install_name_tool -change libmosek64.9.3.dylib `pwd`/.../mosek/9.3/tools/platform/osx64x86/bin/libmosek64.9.3.dylib CWC || rm -f CWC

CWC_Lin: fusion CWC_Lin.cc
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -g $(IPATHS) $(LPATHS) -o CWC_Lin CWC_Lin.cc -lfusion64 -lmosek64
	install_name_tool -change libfusion64.9.3.dylib `pwd`/.../mosek/9.3/tools/platform/osx64x86/bin/libfusion64.9.3.dylib CWC_Lin || rm -f CWC_Lin
	install_name_tool -change libmosek64.9.3.dylib `pwd`/.../mosek/9.3/tools/platform/osx64x86/bin/libmosek64.9.3.dylib CWC_Lin || rm -f CWC_Lin


all: CWC CWC_Lin

clean:
	make clean -C /.../mosek/9.3/tools/platform/osx64x86/src/fusion_cxx
	rm -f CWC CWC_Lin
