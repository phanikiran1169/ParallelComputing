#include <iostream>
#include <string>
#include "Graph.h"
#include "matrix.h"
#include "debug.h"
#include "FloydWarshall.h"

int main(int argc, char* argv[]) {

    if (argc != 5) {
        std::cout << std::endl;
        std::cout << "Usage: floyd_warshall <input_file> <output_file> <num_vertices> <num_threads>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int numVertices = std::stoi(argv[3]);
    int numThreads = std::stoi(argv[4]);

    if (numThreads < 1) {
        std::cout << "Number of threads must be at least 1" << std::endl;
        return 1;
    }
    
    std::cout << "=== Floyd-Warshall Algorithm ===" << std::endl;
    // Create and load graph
    Graph graph(numVertices);
    std::cout << "\nLoading graph from file " << inputFile << "..." << std::endl;
    
    if (!graph.loadFromFile(inputFile)) {
        std::cout << "Failed to load graph from " << inputFile << std::endl;
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
    
    // Save distance matrix to output file
    std::cout << "\nSaving distance matrix to " << outputFile << "..." << std::endl;
    if (saveMatrix(distMatrix, numVertices, outputFile)) {
        std::cout << "Distance matrix saved successfully!" << std::endl;
    } else {
        std::cout << "Failed to save distance matrix to " << outputFile << std::endl;
    }
    
    // Delete matrix
    Delete2dMatrix(distMatrix);
    return 0;
} 