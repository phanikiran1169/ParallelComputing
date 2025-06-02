#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "matrix.h"

class Graph {
private:
    int numVertices;
    int numEdges;
    int **adjMatrix;
public:
    // Constructor
    Graph(int vertices);
    
    // Destructor
    ~Graph();
    
    // Load graph from file
    bool loadFromFile(const std::string& filename);
    
    // Add edge to the graph
    void addEdge(int u, int v, int weight = 1);
    
    // Get number of vertices
    int getNumVertices() const;
    
    // Get number of edges
    int getNumEdges() const;
    
    // Get adjacency matrix (non-const version)
    int** getAdjMatrix();
    
    // Print adjacency matrix
    void printMatrix() const;
    
    // Check if edge exists
    bool hasEdge(int u, int v) const;
    
    // Get edge weight
    int getEdgeWeight(int u, int v) const;
    
};

#endif // GRAPH_H 