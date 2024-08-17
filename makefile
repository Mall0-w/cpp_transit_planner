CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Targets and dependencies
all: transit

transit: transit.o filehandler.o transitgraph.o
	$(CXX) $(CXXFLAGS) transit.o filehandler.o transitgraph.o -o transit.exe

transit.o: transit.cpp filehandler.hpp transitgraph.hpp timehandler.hpp
	$(CXX) $(CXXFLAGS) -c transit.cpp

filehandler.o: filehandler.cpp filehandler.hpp transitgraph.hpp timehandler.hpp
	$(CXX) $(CXXFLAGS) -c filehandler.cpp

transitgraph.o: transitgraph.cpp transitgraph.hpp
	$(CXX) $(CXXFLAGS) -c transitgraph.cpp

clean:
	rm -f *.o transit.exe

.PHONY: clean
