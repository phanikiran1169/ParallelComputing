#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define INF std::numeric_limits<int>::max()

Graph::Graph(int vertices) : numVertices(vertices), numEdges(0) {
    adjMatrix = New2dMatrix<int>(numVertices, numVertices);
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (i == j) {
                // Distance from vertex to itself is 0
                adjMatrix[i][j] = 0;
            } else {
                adjMatrix[i][j] = INF;
            }
        }
    }
}

Graph::~Graph() {
    Delete2dMatrix(adjMatrix);
}

bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    numEdges = 0;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int u, v;
        
        if (iss >> u >> v) {
            if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
                addEdge(u, v, 1);
                numEdges++;
            } else {
                std::cout << "Warning: Invalid edge (" << u << ", " << v << ") ignored" << std::endl;
            }
        }
    }
    
    file.close();
    std::cout << "Loaded data from " << filename << std::endl;
    return true;
}

void Graph::addEdge(int u, int v, int weight) {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
        if (u != v) {
            adjMatrix[u][v] = weight;
        }
    }
}

int Graph::getNumVertices() const {
    return numVertices;
}

int Graph::getNumEdges() const {
    return numEdges;
}

int** Graph::getAdjMatrix() {
    return adjMatrix;
}


bool Graph::hasEdge(int u, int v) const {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
        return adjMatrix[u][v] != INF && adjMatrix[u][v] != 0;
    }
    return false;
}

int Graph::getEdgeWeight(int u, int v) const {
    return adjMatrix[u][v];
}
