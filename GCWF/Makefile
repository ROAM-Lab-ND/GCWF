# This makefile is not in use


CC=clang++

BUILDPATH=../build/
HEADERS=helper.hpp loadsave.hpp YawMoment_Geo.hpp
	
LCL: LongestCenteredLine.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)LCL LongestCenteredLine.cpp
	
EAV: EnumerateAllvertices.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)EAV EnumerateAllvertices.cpp

Grid: Grid.cpp $(HEADERS)
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o $(BUILDPATH)Grid Grid.cpp

all: LCL EAV Grid
	
clean:
	rm -f $(BUILDPATH)LCL $(BUILDPATH)EAV $(BUILDPATH)Grid
