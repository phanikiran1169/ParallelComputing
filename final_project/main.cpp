#include <iostream>
#include <string>
#include "Graph.h"
#include "matrix.h"
#include "debug.h"
#include "FloydWarshall.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cout << std::endl;
        std::cout << "Usage: floyd_warshall <input_file> <num_vertices> <num_threads>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int numVertices = std::stoi(argv[2]);
    int numThreads = std::stoi(argv[3]);

    if (numThreads < 1) {
        std::cout << "Number of threads must be at least 1" << std::endl;
        return 1;
    }
    
    std::cout << "=== Floyd-Warshall Algorithm ===" << std::endl;
    // Create and load graph
    Graph graph(numVertices);
    std::cout << "\nLoading graph from file " << filename << "..." << std::endl;
    
    if (!graph.loadFromFile(filename)) {
        std::cout << "Failed to load graph from " << filename << std::endl;
        std::cout.flush();
        return 1;
    }
    
    std::cout << "Graph loaded successfully!" << std::endl;
    std::cout << "Graph has " << graph.getNumVertices() << " vertices and " << graph.getNumEdges() << " edges" << std::endl;
    // Compute shortest distances
    std::cout << "\nComputing shortest distances..." << std::endl;
    std::cout.flush();
    int** distMatrix = floydWarshall(graph.getAdjMatrix(), graph.getNumVertices());
    std::cout << "Computation completed!" << std::endl;    
    // Delete matrix
    Delete2dMatrix(distMatrix);
    return 0;
} 