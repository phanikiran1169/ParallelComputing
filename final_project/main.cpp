/* main.cpp */

//
// Performs Floyd-Warshall algorithm on a graph file and returns the distance matrix
//
// Usage: floyd_warshall infile.txt outfile.txt num_vertices num_threads
//
// Author:
//   Phani Kiran V
//

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include "Graph.h"
#include "matrix.h"
#include "debug.h"
#include "FloydWarshall.h"
#include <mpi.h>

int myRank, numProcs, numThreads;

int main(int argc, char* argv[]) {


    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (argc != 5) {
        std::cout << std::endl;
        std::cout << "Usage: floyd_warshall <input_file> <output_file> <num_vertices> <num_threads>" << std::endl;
        MPI_Finalize();
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int numVertices = std::stoi(argv[3]);
    numThreads = std::stoi(argv[4]);

    int** distMatrix = nullptr;
    int** adjMatrix = nullptr;
        
    // Master process
    if (myRank == 0) {
        std::cout << "------ Floyd-Warshall Algorithm ------" << std::endl;
        // Create and load graph
        Graph graph(numVertices);
        std::cout << "\nLoading graph from file " << inputFile << "..." << std::endl;
        
        if (!graph.loadFromFile(inputFile)) {
            std::cout << "Failed to load graph from " << inputFile << std::endl;
            std::cout.flush();
            MPI_Finalize();
            return 1;
        }
        
        std::cout << "Graph loaded successfully!" << std::endl;
        std::cout << "Graph has " << graph.getNumVertices() << " vertices and " << graph.getNumEdges() << " edges" << std::endl;
        // Compute shortest distances
        std::cout << "\nComputing shortest distances..." << std::endl;
        std::cout.flush();

        distMatrix = New2dMatrix<int>(numVertices, numVertices);
        adjMatrix = graph.getAdjMatrix();
        CopyMatrix(adjMatrix, distMatrix, numVertices, numVertices);
        
    }
    
    // Synchronize all processes before timing
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();
    
    if (myRank == 0) {
        std::cout << "Starting Floyd-Warshall computation..." << std::endl;
        std::cout.flush();
    }
    
    floydWarshall(distMatrix, numVertices);
    
    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // Synchronize all processes after timing
    MPI_Barrier(MPI_COMM_WORLD);

    if (myRank == 0) {
        // Calculate and display execution time
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        double seconds = duration.count() / 1000.0;
        
        std::cout << "Computation completed!" << std::endl;
        std::cout << "\n=== Performance Results ===" << std::endl;
        std::cout << "Floyd-Warshall execution time: " << std::fixed << std::setprecision(3) 
                  << seconds << " seconds (" << duration.count() << " ms)" << std::endl;
        std::cout << "Number of MPI processes: " << numProcs << std::endl;
        std::cout << "Number of threads: " << numThreads << std::endl;
        std::cout << "Matrix size: " << numVertices << "x" << numVertices << std::endl;
        std::cout << "=============================" << std::endl;
        
        // Save distance matrix to output file
        std::cout << "\nSaving distance matrix to " << outputFile << "..." << std::endl;
        if (saveMatrix(distMatrix, numVertices, outputFile)) {
            std::cout << "Distance matrix saved successfully!" << std::endl;
        } else {
            std::cout << "Failed to save distance matrix to " << outputFile << std::endl;
        }
        
        // Delete matrix
        Delete2dMatrix(distMatrix);
    }

    MPI_Finalize();
    return 0;

} 