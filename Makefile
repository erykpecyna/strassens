CXX = g++
CXXFLAGS = -Wall -g -O0

all: strassen

strassen: strassen.o
	$(CXX) $(CXXFLAGS) -o strassen strassen.o

strassen.o: strassen.cpp
	$(CXX) $(CXXFLAGS) -c strassen.cpp

clean:
	rm strassen *.o