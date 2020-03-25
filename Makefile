CXX = g++
CXXFLAGS = -Wall -g -O0

all: strassens

strassens: strassens.o matrix.o
	$(CXX) $(CXXFLAGS) -o strassens strassens.o Matrix.o

strassens.o: strassens.cpp
	$(CXX) $(CXXFLAGS) -c strassens.cpp

matrix.o: Matrix.h
	$(CXX) $(CXXFLAGS) -c Matrix.cpp

clean:
	rm strassens *.o