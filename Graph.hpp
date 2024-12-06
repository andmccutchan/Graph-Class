#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
using namespace std;

class Graph {
    private:

    public:
        Graph();
        void addEdge(int u, int v);
        void removeEdge(int u, int v);
        bool edgeIn(int u, int v);
        void deleteVertex(int u);
        void addVertex(int u);
        unordered_map<int, pair<int, int> > breadthFirstSearch(int s);
        unordered_map<int, tuple<int, int, int> > depthFirstSearch(bool sort=false);
        vector<int> getOrdering();
        void readFromSTDIN();
};

#endif