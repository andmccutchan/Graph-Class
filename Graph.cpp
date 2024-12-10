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

void Graph::dfsVisit(vector<int>& color, vector<int>& p, vector<int>& dist, vector<int>& d, vector<int>& f, int& time, int u){
    time++;
    d[u] = time;    
    dist[u] = d[u];
    color[u] = 0;   

    for (int v : adjacencyList[u]) {
        if (color[v] == -1) {   
            p[v] = u;           
            dfsVisit(color, p, dist, d, f, time, v);  
        }
    }

    time++;
    f[u] = time;        
    color[u] = 1;

}

unordered_map<int, tuple<int, int, int>> Graph::depthFirstSearch(bool sort) {
    unordered_map<int, tuple<int, int, int>> dfsResult;

    vector<int> color(listSize, -1);  // -1 = WHITE, 0 = GRAY, 1 = BLACK
    vector<int> dist(listSize, -1);
    vector<int> p(listSize, -1);
    int time = 0;
    vector<int> f(listSize, 0);
    vector<int> d(listSize, 0);

    // Perform DFS for every unvisited vertex.
    for (int u = 0; u < listSize; ++u) {
        if (color[u] == -1) {  // If vertex `u` is WHITE
            dfsVisit(color, p, dist, d, f, time, u);
        }
    }

    // Populate the DFS result
    for (int u = 0; u < listSize; ++u) {
        dfsResult[u + 1] = make_tuple(d[u], f[u], p[u] + 1);
    }

    if (sort) {
        sorted.clear();

        vector<int> topologicalOrder;
        for (int u = 0; u < listSize; u++) {
            topologicalOrder.push_back(f[u]);
        }
        
        std::sort(topologicalOrder.begin(), topologicalOrder.end());
        for (int num : topologicalOrder) {
            sorted.push_back(num);
        }
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
