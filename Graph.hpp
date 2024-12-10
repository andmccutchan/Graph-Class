//=========================================================
// Graph.hpp
// Andrew McCutchan, Huy Phan, Namu Kim
// 07/12/2024
// Contains the header file for graph class
//=========================================================
#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include "customexceptions.hpp"
#include <queue>
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
        void dfsVisit(vector<int>* color, vector<int>* p, vector<int>* dist, vector<int>* d, vector<int>* f, int* time, int u);
        unordered_map<int, pair<int, int> > breadthFirstSearch(int s);
        unordered_map<int, tuple<int, int, int> > depthFirstSearch(bool sort = false);
        vector<int> getOrdering();
		    void printAdjacencyList() const;
        void    readFromSTDIN();
};

#endif