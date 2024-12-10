//=========================================================
// mytests.cpp
// Andrew McCutchan, Huy Phan, Namu Kim
// 07/12/2024
// Contains the test cases for the Graph class
//=========================================================
#include <iostream>
#include <string>
#include <sstream>
#include "Graph.hpp"

using namespace std;

// Global counters for passed and failed tests
int testsPassed = 0;
int testsFailed = 0;

// Utility function for running tests
void assertTest(bool condition, const string &testName) {
    if (condition) {
        testsPassed++;
        cout << "[PASS] " << testName << endl;
    } else {
        testsFailed++;
        cout << "[FAIL] " << testName << endl;
    }
}

// Test Default Constructor
void testDefaultConstructor() {
    Graph g;
    g.addVertex(1);
    assertTest(!g.edgeIn(1, 2), "Default Constructor: Graph initialized correctly");
}

// Test Add Edge
void testAddEdge() {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    assertTest(g.edgeIn(1, 2), "Add Edge: Edge added successfully");
    assertTest(!g.edgeIn(2, 1), "Add Edge: Direction matters");
}

// Test Remove Edge
void testRemoveEdge() {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.removeEdge(1, 2);
    assertTest(!g.edgeIn(1, 2), "Remove Edge: Edge removed successfully");
    try {
        g.removeEdge(1, 2);
        assertTest(false, "Remove Edge: Exception thrown for non-existing edge");
    } catch (...) {
        assertTest(true, "Remove Edge: Exception correctly thrown");
    }
}

// Test Add Vertex
void testAddVertex() {
    Graph g;
    g.addVertex(1);
    try {
        g.addVertex(1);
        assertTest(false, "Add Vertex: Exception thrown for duplicate vertex");
    } catch (...) {
        assertTest(true, "Add Vertex: Exception correctly thrown");
    }
}

// Test Delete Vertex
void testDeleteVertex() {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addEdge(1, 2);
    g.deleteVertex(2);
    assertTest(!g.edgeIn(1, 2), "Delete Vertex: Edge removed with vertex");
    try {
        g.deleteVertex(3);
        assertTest(false, "Delete Vertex: Exception thrown for non-existing vertex");
    } catch (...) {
        assertTest(true, "Delete Vertex: Exception correctly thrown");
    }
}

// Test Breadth First Search
void testBreadthFirstSearch() {
    Graph g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    auto bfsResult = g.breadthFirstSearch(1);
    assertTest(bfsResult[1].first == 0, "BFS: Distance to self is 0");
    assertTest(bfsResult[2].first == 1, "BFS: Distance to 2 is 1");
    assertTest(bfsResult[3].first == 2, "BFS: Distance to 3 is 2");
}

// Test Depth First Search
void testDepthFirstSearch() {
    Graph g;

    // Add vertices and edges to form a DAG
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addVertex(4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    // Perform DFS with sorting enabled
    unordered_map<int, tuple<int, int, int>> dfsResult = g.depthFirstSearch(true);

    // Validate discovery and finish times
    assertTest(dfsResult[1] == make_tuple(1, 8, -1), "DFS: Correct discovery, finish, and parent for vertex 1");
    assertTest(dfsResult[2] == make_tuple(2, 3, 1), "DFS: Correct discovery, finish, and parent for vertex 2");
    assertTest(dfsResult[3] == make_tuple(4, 7, 1), "DFS: Correct discovery, finish, and parent for vertex 3");
    assertTest(dfsResult[4] == make_tuple(5, 6, 3), "DFS: Correct discovery, finish, and parent for vertex 4");

    // Validate topological ordering
    vector<int> ordering = g.getOrdering();
    vector<int> expectedOrder = {1, 3, 4, 2};
    assertTest(ordering == expectedOrder, "DFS: Correct topological ordering");
}


// Test Get Ordering
void testGetOrdering() {
    Graph g;

    // Add vertices and edges to form a directed acyclic graph (DAG)
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addVertex(4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    // Perform DFS with sorting enabled
    g.depthFirstSearch(true); // This populates the `sorted` vector

    // Retrieve the ordering
    vector<int> ordering = g.getOrdering();

    // Validate the ordering
    assertTest(ordering.size() == 4, "Get Ordering: Correct number of vertices in ordering");
    
    // Check the topological order (example: 1 → 3 → 4 → 2 is valid)
    bool isValidOrder = (ordering[0] == 1 && ordering[1] == 3 && ordering[2] == 4 && ordering[3] == 2);
    assertTest(isValidOrder, "Get Ordering: Correct topological order");
}


// Test Read from File Input via STDIN
void testReadFromFileInput() {
    Graph g = Graph::readFromSTDIN(); 
    g.readFromSTDIN(); // Read graph from redirected input
    g.printAdjacencyList();
    // Example assertions to validate edges from the input
    assertTest(g.edgeIn(12, 1), "Read From File: Edge 12 -> 1");
    assertTest(g.edgeIn(7, 17), "Read From File: Edge 7 -> 17");
    assertTest(!g.edgeIn(1, 20), "Read From File: No edge 1 -> 20");
}

// Main Function to Run All Tests
int main() {
    cout << "Running Graph Tests..." << endl;

    // Functional tests
    testDefaultConstructor();
    testAddEdge();
    testRemoveEdge();
    testAddVertex();
    testDeleteVertex();
    testBreadthFirstSearch();
    testDepthFirstSearch();
    testGetOrdering();

    // Tests for redirected input and adjacency list
    cout << "\nTesting with input redirection (e.g., < myGraph.txt):" << endl;
    testReadFromFileInput();

    // Print summary
    cout << "\nTest Summary:" << endl;
    cout << "Tests Passed: " << testsPassed << endl;
    cout << "Tests Failed: " << testsFailed << endl;

    return 0;
}
