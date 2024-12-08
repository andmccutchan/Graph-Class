//========================================================
// customexceptions.hpp
// DATE: October 26th
// AUTHORS: Andrew McCutchan, Liam Kaznelson, Esther Zhang
// DESCRIPTION: Provides names and definitions for the
// exceptions in the customexceptions class.
//========================================================


#ifndef CUSTOMEXCEPTIONS_HPP
#define CUSTOMEXCEPTIONS_HPP

#include <exception>
using namespace std;

class edge_exception : public std::exception {
public:
    const char* what() const noexcept override {
        return "Edge/Vertex does not exist in the graph.";
    }
};

class vertex_exception : public runtime_error {
public:
    vertex_exception(int u) : runtime_error("Vertex " + to_string(u) + " in the graph.") {}
};


#endif 
