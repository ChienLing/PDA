all: Lab5

CXX = g++

OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

LINKFLAGS = -static 

Lab5: main.o
	$(CXX) $(LINKFLAGS) main.o -o Lab5

main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

clean:
	rm -rf *.o *.gch Lab5
