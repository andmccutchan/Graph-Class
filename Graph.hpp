#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include "customexceptions.hpp"
using namespace std;

class Graph {
    private:
		vector<int> sorted;
		vector<int>* adjacencyList;
		int listSize;
    public:
                Graph();
                Graph(const Graph &other);
                ~Graph();
        Graph   operator=(const Graph &other);
        void    addEdge(int u, int v);
        void    removeEdge(int u, int v);
        bool    edgeIn(int u, int v);
        void    deleteVertex(int u);
        void    addVertex(int u);
        unordered_map<int, pair<int, int> > breadthFirstSearch(int s);
        // unordered_map<int, tuple<int, int, int> > depthFirstSearch(bool sort = false);
        vector<int> getOrdering();
		void printAdjacencyList() const;
        void    readFromSTDIN();
};

#endif