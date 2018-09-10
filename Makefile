# (c) Daniel Desmond Dennis 2018

CXX = clang++
CXXFLAGS = -std=c++11

#BEGIN

COMP = main.o instructions.o machine.o memory.o errorhandler.o
main: $(COMP)
	$(CXX) $(COMP) -o main

MAIN = main.cpp
main.o: $(MAIN)
	$(CXX) $(CXXFLAGS) $(MAIN) -c main.cpp

INSTRUCTIONS = instructions.cpp instructions.hpp
instructions.o: $(INSTRUCTIONS)
	$(CXX) $(CXXFLAGS) $(INSTRUCTIONS) -c instructions.cpp

MACHINE = machine.cpp machine.hpp
machine.o: $(MACHINE)
	$(CXX) $(CXXFLAGS) $(MACHINE) -c machine.cpp

MEMORY = memory.cpp memory.hpp
memory.o: $(MEMORY)
	$(CXX) $(CXXFLAGS) $(MEMORY) -c memory.cpp

ERRORHANDLER = errorhandler.cpp errorhandler.hpp
errorhandler.o: $(ERRORHANDLER)
	$(CXX) $(CXXFLAGS) $(ERRORHANDLER) -c errorhandler.cpp

#CLEAN UP

clean:
	rm *.o
	rm *.gch
