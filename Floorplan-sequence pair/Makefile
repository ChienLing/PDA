CXX		=	g++ 
EXE		=   lab3.o	
CXXFLAGS = -o -std=c++11

all: $(EXE)

$(EXE): main.o
	$(CXX) $^ $(CXXFLAGS) -o $@ 
	
$main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.gch *.o 
