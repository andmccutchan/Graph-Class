mytests: Graph.o mytests.o
	g++ -std=c++17 -o mytests Graph.o mytests.o

Graph.o: Graph.cpp Graph.hpp
	g++ -std=c++17 -c Graph.cpp

mytests.o: mytests.cpp Graph.hpp
	g++ -std=c++17 -c mytests.cpp

clean:
	rm -f *.o mytests