CUTOFF?=658
CXX = g++
CXXFLAGS = -Wall -g -O3

all: strassen

strassen: strassen.o
	$(CXX) $(CXXFLAGS) -o strassen strassen.o -DCUTOFF=$(CUTOFF)

strassen.o: strassen.cpp
	$(CXX) $(CXXFLAGS) -c strassen.cpp -DCUTOFF=$(CUTOFF)

clean:
	rm strassen *.o