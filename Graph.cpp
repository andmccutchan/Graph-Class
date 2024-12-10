#include "Graph.hpp"

//=================================
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

    sorted = other.sorted;
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
    sorted = other.sorted;

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
    s -= 1;

    if (s < 0 || s >= listSize) {
        throw edge_exception(); // Invalid starting vertex
    }

    unordered_map<int, pair<int, int>> bfsResult;

    // Initialize visited, distance, and parent vectors
    vector<bool> visited(listSize, false);
    vector<int> distance(listSize, -1); // Distance from source
    vector<int> parent(listSize, -1);  // Parent in BFS tree

    // BFS queue
    queue<int> q;

    // Initialize source
    visited[s] = true;
    distance[s] = 0;
    q.push(s);

    // Perform BFS
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adjacencyList[u]) { // Traverse neighbors of u
            if (!visited[v]) {
                visited[v] = true;
                distance[v] = distance[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Populate the result map with 1-based vertex indexing
    for (int i = 0; i < listSize; i++) {
        if (visited[i]) { // Include only reachable vertices
            bfsResult[i + 1] = {distance[i], parent[i] == -1 ? -1 : parent[i] + 1};
        }
    }

    return bfsResult;
}

//==============================================================
// dfsVisit 
// Helper function of DepthFirstSearch. Recursively explores a vertex and its neighbors.
//
// PARAMETERS:
//  color: Vector tracking the state of nodes.
//  p: Vector storing the parent of each node.
//  d: Vector storing discovery times of nodes.
//  f: Vector storing finish times of nodes.
//  time: Global time counter for DFS.
//  u: The current node being explored.
//  sorted: Vector to store topological order.
// RETURN VALUE:
//  none
//==============================================================
void Graph::dfsVisit(vector<int>& color, vector<int>& p, vector<int>& d, vector<int>& f, int& time, int u, vector<int>& sorted) {
    time++;
    d[u] = time; //discovery time
    color[u] = 0; // gray

    for (int v : adjacencyList[u]) {
        if (color[v] == -1) { // White
            p[v] = u;
            dfsVisit(color, p, d, f, time, v, sorted);
        }
    }

    color[u] = 1; //black
    time++;
    f[u] = time; //finish time
    sorted.push_back(u); 
}

//==============================================================
// depthFirstSearch() 
// Performs DFS on graph. It records the discovery, finish, and
// parent value for each node and returns it an unordered map.
// Can optionally perform topological sort.
//
// PARAMETERS:
//  sort: Boolean deciding to perform topological sort or not.
// RETURN VALUE:
//  unordered_map<int tuple<int, int, int>>: unordered map where
//  key is the vertex, and each value in the tuple represents
//  the discovery time, finish time, and parent of each vertex.
//==============================================================
unordered_map<int, tuple<int, int, int>> Graph::depthFirstSearch(bool sort) {
    unordered_map<int, tuple<int, int, int>> dfsResult;

    vector<int> color(listSize, -1); // -1 = white, 0 = gray, 1 = black
    vector<int> p(listSize, -1);    // Parent array
    vector<int> d(listSize, 0);     // Discovery time
    vector<int> f(listSize, 0);     // Finish time
    vector<int> ordered;           // For topological sorting
    int time = 0;                  // counter for time aspect.

    // Perform DFS for all vertices
    for (int u = 0; u < listSize; ++u) {
        if (color[u] == -1) { // Start DFS only for unvisited vertices
            dfsVisit(color, p, d, f, time, u, ordered);
        }
    }
    // Insert values into unordered map and return
    for (int u = 0; u < listSize; u++) {
        dfsResult[u + 1] = make_tuple(d[u], f[u], p[u] == -1 ? -1 : p[u] + 1);
    }
    // Topological sort
    if (sort) {
        reverse(ordered.begin(), ordered.end());
        this->sorted = ordered;
    }

    return dfsResult;
}



vector<int> Graph::getOrdering() {
    return sorted;
}

Graph Graph::readFromSTDIN() {
    int n, m;
    cin >> n >> m;

    Graph g;
    g.adjacencyList = new vector<int>[n];
    g.listSize = n;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    return g;
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
