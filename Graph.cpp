//=========================================================
// Graph.cpp
// Andrew McCutchan, Huy Phan, Namu Kim
// 07/12/2024
// Contains the implementation file for Graph class
//=========================================================
#include "Graph.hpp"


//===================================================================
// Graph::Graph()
// Default constructor to initialize an empty graph
// PARAMETERS: None
// RETURN VALUE: None
//===================================================================
Graph::Graph() {
    listSize = 0;
    adjacencyList = new vector<int>[listSize];
}


//===================================================================
// Graph::Graph(const Graph &other)
// Copy constructor to create a deep copy of another Graph object
// PARAMETERS: 
// - other: const Graph&, the graph object to copy
// RETURN VALUE: None
//===================================================================
Graph::Graph(const Graph &other) {
    listSize = other.listSize;
    adjacencyList = new vector<int>[listSize];

    for (int i = 0; i < listSize; i++) {
        adjacencyList[i] = other.adjacencyList[i];
    }
    sorted = other.sorted; // copying sorted
}


//===================================================================
// ~Graph()
// Destructor to release dynamically allocated memory
// PARAMETERS: None
// RETURN VALUE: None
//===================================================================
Graph::~Graph() {
    delete[] adjacencyList;
}


//===================================================================
// Graph::operator=(const Graph &other)
// Assignment operator to assign one Graph object to another
// PARAMETERS: 
// - other: const Graph&, the graph object to assign from
// RETURN VALUE: 
// - Graph: the current graph object after assignment
//===================================================================
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

//===================================================================
// addEdge
// Adds a directed edge from vertex u to vertex v
// PARAMETERS: 
// - u: int, starting vertex of the edge
// - v: int, ending vertex of the edge
// RETURN VALUE: None
//===================================================================
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


//===================================================================
// removeEdge
// Removes a directed edge from vertex u to vertex v
// PARAMETERS: 
// - u: int, starting vertex of the edge
// - v: int, ending vertex of the edge
// RETURN VALUE: None
//===================================================================
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


//===================================================================
// edgeIn
// Checks if an edge exists between vertices u and v
// PARAMETERS: 
// - u: int, starting vertex of the edge
// - v: int, ending vertex of the edge
// RETURN VALUE: 
// - bool: true if the edge exists, false otherwise
//===================================================================
bool Graph::edgeIn(int u, int v) {
    u -= 1;
    v -= 1;

    if (u < 0 || u >= listSize || v < 0 || v >= listSize) {
        return false;
    }

    return find(adjacencyList[u].begin(), adjacencyList[u].end(), v) != adjacencyList[u].end();
}


//===================================================================
// deleteVertex
// Deletes a vertex and all associated edges
// PARAMETERS: 
// - u: int, the vertex to delete
// RETURN VALUE: None
//===================================================================
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


//===================================================================
// addVertex
// Adds a new vertex to the graph
// PARAMETERS: 
// - u: int, the new vertex to add
// RETURN VALUE: None
//===================================================================
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
    color[u] = 0;

    for (int v : adjacencyList[u]) {
        if (color[v] == -1){
            p[v] = u;
            dfsVisit(color, p, dist, d, f, time, v);
        }
    }
    time++;
    f[u] = time;
    color[u] = 1;
}

unordered_map<int, tuple<int, int, int>> Graph::depthFirstSearch(bool enableSort) {
    unordered_map<int, tuple<int, int, int>> dfsResult;
    vector<int> color(listSize, -1); // -1 = white, 0 = gray, 1 = black
    vector<int> dist(listSize, -1);
    vector<int> p(listSize, -1);
    int time = 0;
    vector<int> f(listSize, 0); // Finish times
    vector<int> d(listSize, 0); // Discovery times

    // Perform DFS on each unvisited vertex
    for (int u = 0; u < listSize; u++) {
        if (color[u] == -1) {
            dfsVisit(color, p, dist, d, f, time, u);
        }
    }

    // Populate the DFS result
    for (int u = 0; u < listSize; u++) {
        dfsResult[u + 1] = make_tuple(d[u], f[u], p[u] == -1 ? -1 : p[u] + 1);
    }

    // Populate `sorted` if sorting is requested
    if (enableSort) {
        vector<pair<int, int>> finishOrder; // Pair of (finish time, vertex index)
        for (int u = 0; u < listSize; u++) {
            finishOrder.emplace_back(f[u], u + 1); // Use 1-based indexing
        }
        std::sort(finishOrder.rbegin(), finishOrder.rend()); // Sort by finish time descending

        sorted.clear();
        for (const auto& pair : finishOrder) {
            sorted.push_back(pair.second); // Store vertex indices in sorted order
        }
    }

    return dfsResult;
}

//===================================================================
// getOrdering
// Retrieves the current topological ordering of the graph (if sorted)
// PARAMETERS: None
// RETURN VALUE:
// - vector<int>: a vector representing the topological order
//===================================================================
vector<int> Graph::getOrdering() {
    return sorted;
}

//===================================================================
// readFromSTDIN
// Reads a graph from standard input in adjacency list format
// PARAMETERS: None
// RETURN VALUE:
// - Graph: a new Graph object populated with the input adjacency list
// INPUT FORMAT:
// - The first line contains two integers, n (number of vertices) and m (number of edges).
// - The next m lines each contain two integers, u and v, representing a directed edge from u to v.
//===================================================================
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


//===================================================================
// printAdjacencyList
// Prints the graph's adjacency list representation to standard output
// PARAMETERS: None
// RETURN VALUE: None
// OUTPUT FORMAT:
// - For each vertex, prints its outgoing edges in the format:
//   vertex: neighbor1 -> neighbor2 -> ... -> /
//===================================================================
void Graph::printAdjacencyList() const {
    for (int i = 0; i < listSize; i++) {
        cout << i + 1 << ": ";
        for (int neighbor : adjacencyList[i]) {
            cout << neighbor + 1 << " -> ";
        }
        cout << "/" << endl;
    }
}
