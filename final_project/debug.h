#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

void verifyResult(int** distMatrix1, int** distMatrix2, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (distMatrix1[i][j] != distMatrix2[i][j]) {
                std::cout << "Verification failed: distMatrix1[" << i << "][" << j << "] = " << distMatrix1[i][j] << " != distMatrix2[" << i << "][" << j << "] = " << distMatrix2[i][j] << std::endl;
                std::cout.flush();
            }
        }
    }
}

#endif // DEBUG_H 