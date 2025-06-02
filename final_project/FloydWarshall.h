#include <iostream>
#include "matrix.h"
#include <mpi.h>
#include <cstring>

extern int myRank, numProcs, numThreads;
void fw_helper(int** chunkMatrix, int numVertices, int actualChunkSize);

void floydWarshall(int** distMatrix, int numVertices) {

    int chunkSize = numVertices / numProcs;
    int extraRows = numVertices % numProcs;
    int sender = 0;
    int receiver = 0;

    int* sendcounts = new int[numProcs];
    int* recvcounts = new int[numProcs];
    int* displs = new int[numProcs];

    // Process 0 gets extraRows + chunkSize rows, others get chunkSize rows
    sendcounts[0] = (extraRows + chunkSize) * numVertices;
    recvcounts[0] = (extraRows + chunkSize) * numVertices;
    displs[0] = 0;

    for (int i = 1; i < numProcs; i++) {
        sendcounts[i] = chunkSize * numVertices;
        recvcounts[i] = chunkSize * numVertices;
        displs[i] = (extraRows + i * chunkSize) * numVertices;
    }

    // Determine receive count and chunk matrix for this process
    int recvCount = (myRank == 0) ? (extraRows + chunkSize) * numVertices : chunkSize * numVertices;
    int** chunkMatrix = (myRank == 0) ? New2dMatrix<int>(extraRows + chunkSize, numVertices) : New2dMatrix<int>(chunkSize, numVertices);

    int* sendBuffer = (myRank == 0) ? distMatrix[0] : nullptr;

    MPI_Scatterv(sendBuffer, sendcounts, displs, MPI_INT,
                 chunkMatrix[0], recvCount, MPI_INT,
                 sender, MPI_COMM_WORLD);
    
    int actualChunkSize = (myRank == 0) ? chunkSize + extraRows : chunkSize;
    fw_helper(chunkMatrix, numVertices, actualChunkSize);
    
    int* recvBuffer = (myRank == 0) ? distMatrix[0] : nullptr;

    MPI_Gatherv(chunkMatrix[0], recvCount, MPI_INT,
                recvBuffer, recvcounts, displs, MPI_INT,
                receiver, MPI_COMM_WORLD);

    delete[] sendcounts;
    delete[] recvcounts;
    delete[] displs;
    Delete2dMatrix(chunkMatrix);

}

void fw_helper(int** chunkMatrix, int numVertices, int actualChunkSize) {

    int* row_k = new int[numVertices];
    int chunkSize = numVertices/numProcs;
    int extraRows = numVertices % numProcs;

    int sender = 0;
    int offset = 0;
    int adjusted_k = 0;

    // Floyd-Warshall algorithm
    for (int k = 0; k < numVertices; k++) {

        if (k < extraRows + chunkSize) {
            sender = 0;
            offset = k;
        } else {
            adjusted_k = k - (extraRows + chunkSize);
            sender = 1 + adjusted_k / chunkSize;
            offset = adjusted_k % chunkSize;
        }

        if (myRank == sender) {
            memcpy(row_k, chunkMatrix[offset], numVertices * sizeof(int));
        }

        MPI_Bcast(row_k, numVertices, MPI_INT, sender, MPI_COMM_WORLD);
        
        for (int i = 0; i < actualChunkSize; i++) {
            for (int j = 0; j < numVertices; j++) {
                // Skip if no path through k
                if (chunkMatrix[i][k] == -1 || row_k[j] == -1) continue;
                
                // Check if path through k is shorter
                if (chunkMatrix[i][k] + row_k[j] < chunkMatrix[i][j]) {
                    chunkMatrix[i][j] = chunkMatrix[i][k] + row_k[j];
                }
            }
        }
    }

    delete[] row_k;
}