CXX = g++
CXXFLAGS = -std=c++17

# Targets and dependencies
all: transit

transit: transit.cpp filehandler.o
	$(CXX) $(CXXFLAGS) transit.cpp filehandler.o -o transit.exe

filehandler.o: filehandler.cpp
	$(CXX) $(CXXFLAGS) -c filehandler.cpp

clean:
	rm -f *.o transit.exe

.PHONY: clean
