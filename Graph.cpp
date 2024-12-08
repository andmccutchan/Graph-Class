#include "Graph.hpp"

Graph::Graph() {
    listSize = 0;
    adjacencyList = new vector<int>[listSize];
}

Graph::Graph(const Graph &other) {
    listSize = other.listSize;
    adjacencyList = new vector<int>[listSize];

    for (int i = 0; i < listSize; i++) {
        adjacencyList[i] = other.adjacencyList[i];
    }
}

Graph::~Graph() {
    delete[] adjacencyList;
}

Graph Graph::operator=(const Graph &other) {
    delete[] adjacencyList;

    listSize = other.listSize;
    adjacencyList = new vector<int>[listSize];

    for (int i = 0; i < listSize; i++) {
        adjacencyList[i] = other.adjacencyList[i];
    }

    return *this;
}


void Graph::addEdge(int u, int v) {
    u -= 1;
    v -= 1;

    if (v >= listSize) {
        int newSize = v + 1;
        vector<int>* newList = new vector<int>[newSize];

        for (int i = 0; i < listSize; i++) {
            newList[i] = adjacencyList[i];
        }

        delete[] adjacencyList;
        adjacencyList = newList;
        listSize = newSize;
    }
    
    adjacencyList[u].push_back(v);
}

void Graph::removeEdge(int u, int v) {
    u -= 1; 
    v -= 1;

    auto iterator = find(adjacencyList[u].begin(), adjacencyList[u].end(), v);

    if (iterator != adjacencyList[u].end()) {
        adjacencyList[u].erase(iterator);
    } else {
        throw edge_exception();
    }
}

bool Graph::edgeIn(int u, int v) {
    u -= 1;
    v -= 1;

    if (u < 0 || u >= listSize || v < 0 || v >= listSize) {
        return false;
    }

    return find(adjacencyList[u].begin(), adjacencyList[u].end(), v) != adjacencyList[u].end();
}

void Graph::deleteVertex(int u) {
    u -= 1;  

    if (u > listSize) {
        throw edge_exception();
    }

    adjacencyList[u].clear();

    for (int i = 0; i < listSize; ++i) {
        if (i != u) {
            auto it = find(adjacencyList[i].begin(), adjacencyList[i].end(), u);
            if (it != adjacencyList[i].end()) {
                adjacencyList[i].erase(it);
            }
        }
    }
}

void Graph::addVertex(int u) {
    u -= 1;

    if (u < listSize) {
        throw vertex_exception(u + 1);
    }

    int newSize = u + 1;

    if (newSize > listSize) {
        vector<int>* newAdjacencyList = new vector<int>[newSize];

        for (int i = 0; i < listSize; ++i) {
            newAdjacencyList[i] = adjacencyList[i];
        }

        delete[] adjacencyList;

        adjacencyList = newAdjacencyList;
        listSize = newSize;
    }

}

unordered_map<int, pair<int, int> > Graph::breadthFirstSearch(int s) {

}

// unordered_map<int, tuple<int, int, int> > Graph::depthFirstSearch(bool sort=false) {

// }

vector<int> Graph::getOrdering() {
    return sorted;
}

void Graph::readFromSTDIN() {
    int n, m;
    cin >> n >> m;

    delete[] adjacencyList;

    listSize = n;
    adjacencyList = new vector<int>[listSize];

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
}

void Graph::printAdjacencyList() const {
    for (int i = 0; i < listSize; i++) {
        cout << i + 1 << ": ";
        for (int neighbor : adjacencyList[i]) {
            cout << neighbor + 1 << " -> ";
        }
        cout << "/" << endl;
    }
}
