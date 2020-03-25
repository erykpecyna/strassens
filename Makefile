CXX = g++
CXXFLAGS = -Wall -g -O0

all: strassen

strassen: strassen.o matrix.o
	$(CXX) $(CXXFLAGS) -o strassen strassen.o Matrix.o

strassen.o: strassen.cpp
	$(CXX) $(CXXFLAGS) -c strassen.cpp

matrix.o: Matrix.h
	$(CXX) $(CXXFLAGS) -c Matrix.cpp

clean:
	rm strassen *.o