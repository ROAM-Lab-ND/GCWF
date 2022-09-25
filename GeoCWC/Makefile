
CC=clang++
	
LCL: LongestCenteredLine.cpp
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o LCL LongestCenteredLine.cpp
	
EAV: EnumerateAllvertices.cpp
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o EAV EnumerateAllvertices.cpp

Grid: Grid.cpp
	$(CC) -Wl,-headerpad_max_install_names -std=c++11 -stdlib=libc++ -O3 -o Grid Grid.cpp

all: clean test LCL EAV Grid
	
clean:
	rm -f test LCL EAV Grid
