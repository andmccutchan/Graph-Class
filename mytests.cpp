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

void testDepthFirstSearch() {
    Graph g = Graph::readFromSTDIN();
    auto result = g.depthFirstSearch(true);
    
    for (const auto& [vertex, tuple] : result) {
    cout << "Vertex: " << vertex
         << ", Discovery: " << get<0>(tuple)
         << ", Finish: " << get<1>(tuple)
         << ", Parent: " << get<2>(tuple) << endl;
}
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
    // testDefaultConstructor();
    // testAddEdge();
    // testRemoveEdge();
    // testAddVertex();
    // testDeleteVertex();
    // testBreadthFirstSearch();

    // Tests for redirected input and adjacency list
    // cout << "\nTesting with input redirection (e.g., < myGraph.txt):" << endl;
    // testReadFromFileInput();
    testDepthFirstSearch();


    // Print summary
    cout << "\nTest Summary:" << endl;
    cout << "Tests Passed: " << testsPassed << endl;
    cout << "Tests Failed: " << testsFailed << endl;

    return 0;
}