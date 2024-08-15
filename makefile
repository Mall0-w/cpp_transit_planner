CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Targets and dependencies
all: transit

transit: transit.o filehandler.o
	$(CXX) $(CXXFLAGS) transit.o filehandler.o -o transit.exe

transit.o: transit.cpp filehandler.hpp transitgraph.hpp
	$(CXX) $(CXXFLAGS) -c transit.cpp

filehandler.o: filehandler.cpp filehandler.hpp transitgraph.hpp
	$(CXX) $(CXXFLAGS) -c filehandler.cpp

clean:
	rm -f *.o transit.exe

.PHONY: clean
