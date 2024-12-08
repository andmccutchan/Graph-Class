#include <iostream>
#include "Graph.hpp"
using namespace std;


void testAddEdge() {
    Graph g;
    g.addEdge(1, 6);
    g.addEdge(6, 4);
    g.addEdge(3, 7);

    g.printAdjacencyList();
}

int main(void) {
    testAddEdge();
}