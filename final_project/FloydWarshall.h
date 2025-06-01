#include <iostream>
#include "matrix.h"

#define INF std::numeric_limits<int>::max()

int** floydWarshall(int** adjMatrix, int numVertices) {
    std::cout << "Starting Floyd-Warshall algorithm..." << std::endl;
    int** distMatrix = New2dMatrix<int>(numVertices, numVertices);
    CopyMatrix(adjMatrix, distMatrix, numVertices, numVertices);

    // Floyd-Warshall algorithm
    for (int k = 0; k < numVertices; k++) {
        if (k % 100 == 0) {
            std::cout << "Processing intermediate vertex " << k << "/" << numVertices << std::endl;
        }
        
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                // Skip if no path through k
                if (distMatrix[i][k] == INF || 
                    distMatrix[k][j] == INF) {
                    continue;
                }
                
                // Check if path through k is shorter
                if (distMatrix[i][k] + distMatrix[k][j] < distMatrix[i][j]) {
                    distMatrix[i][j] = distMatrix[i][k] + distMatrix[k][j];
                }
            }
        }
    }
    
    std::cout << "Floyd-Warshall completed!" << std::endl;
    return distMatrix;
}
