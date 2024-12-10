//=========================================================
// Graph.cpp
// Andrew McCutchan, Huy Phan, Namu Kim
// 07/12/2024
// Contains the implementation file for Graph class
//=========================================================
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
    sorted = other.sorted; // copying sorted
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
    sorted = other.sorted; // copying sorted

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

    if (u >= listSize) {
        throw edge_exception();
    }

    adjacencyList[u].clear();

    for (int i = 0; i < listSize; ++i) {
        if (i != u) {
            auto it = std::find(adjacencyList[i].begin(), adjacencyList[i].end(), u);
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

//==============================================================
// Breadth-First Search (BFS)
// Huy Phan
// Performs BFS traversal starting from a given source vertex (1-based).
// Computes the shortest distances and parent relationships for all
// reachable vertices in the graph. Uses a queue for level-by-level
// traversal.
//
// PARAMETERS:
//    s - The source vertex (1-based index) from which BFS starts.
//        Must be a valid vertex in the graph.
//
// RETURN VALUE:
//    unordered_map<int, pair<int, int>>:
//==============================================================
unordered_map<int, pair<int, int>> Graph::breadthFirstSearch(int s) {
    // Adjust for 0-based indexing
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

void Graph::dfsVisit(vector<int>& color, vector<int>& p, vector<int>& dist, vector<int>& d, vector<int>& f, int& time, int u){
    //air<int, int> times;
    time++;
    d[u] = time;
    dist[u] = d[u];
    color[u] = 0;

    for (int v : adjacencyList[u]) {
        if (color[v] == -1){
            p[v] = u;
            dfsVisit(color, p, dist, d, f, time, u);
            //times.first = d[u];
            //times.second = f[u];
        }
    }
    time++;
    f[u] = time;
    color[u] == 1;
    
    //return times;

}

unordered_map<int, tuple<int, int, int>> Graph::depthFirstSearch(bool sort=false) {
    
    unordered_map <int, tuple<int, int, int>> dfsResult;
    
    vector<int> color(listSize, -1); //-1 = white, 0 = gray, 1 = black.
    vector<int> dist(listSize, -1);
    vector<int> p(listSize, -1);
    int time = 0;
    vector<int> f(listSize, 0);
    vector<int> d(listSize, 0);

    int u=0;

    for (int v : adjacencyList[u]) {
        if ( color[u] == -1){
            dfsVisit(color, p, dist, d, f, time, u);
        }
        
        dfsResult[u] = make_tuple(d[u], f[u], p[u]);
        u++;
    }
    return dfsResult;
}

vector<int> Graph::getOrdering() {
    return sorted;
}

void Graph::readFromSTDIN() {
    int n, m;
    cin >> n >> m;

    delete[] adjacencyList;
    adjacencyList = nullptr;

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
