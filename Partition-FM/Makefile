all: Lab1

CXX = g++

OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

LINKFLAGS = -static 

Lab1: main.o
	$(CXX) $(LINKFLAGS) main.o -o Lab2

main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

clean:
	rm -rf *.o *.gch Lab2
