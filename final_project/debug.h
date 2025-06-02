#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <fstream>
#include <limits>
#include "matrix.h"

// Save a distance matrix to a file
bool saveMatrix(int** matrix, int numVertices, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    // Write the number of vertices first
    file << numVertices << std::endl;
    
    // Write the matrix
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
                file << matrix[i][j];
            if (j < numVertices - 1) {
                file << " ";
            }
        }
        file << std::endl;
    }
    
    file.close();
    return true;
}

// Load a distance matrix from a file
int** loadMatrix(const std::string& filename, int expectedVertices) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << " for reading" << std::endl;
        return nullptr;
    }
    
    int numVertices;
    if (!(file >> numVertices)) {
        std::cout << "Error: Could not read number of vertices from " << filename << std::endl;
        file.close();
        return nullptr;
    }
    
    if (numVertices != expectedVertices) {
        std::cout << "Error: Matrix size mismatch. Expected " << expectedVertices 
                  << " vertices, found " << numVertices << std::endl;
        file.close();
        return nullptr;
    }
    
    int** matrix = New2dMatrix<int>(numVertices, numVertices);
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            std::string value;
            if (!(file >> value)) {
                std::cout << "Error: Could not read matrix element [" << i << "][" << j << "]" << std::endl;
                Delete2dMatrix(matrix);
                file.close();
                return nullptr;
            }
            
            try {
                matrix[i][j] = std::stoi(value);
            } 
            
            catch (const std::exception& e) {
                std::cout << "Error: Invalid matrix value '" << value << "' at [" << i << "][" << j << "]" << std::endl;
                Delete2dMatrix(matrix);
                file.close();
                return nullptr;
            }
        }
    }
    
    file.close();
    return matrix;
}

void verifyResult(int** distMatrix1, int** distMatrix2, int numVertices) {
    bool allMatch = true;
    int mismatchCount = 0;
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (distMatrix1[i][j] != distMatrix2[i][j]) {
                if (mismatchCount < 10) {
                    std::cout << "Verification failed: distMatrix1[" << i << "][" << j << "] = " << distMatrix1[i][j] << " != distMatrix2[" << i << "][" << j << "] = " << distMatrix2[i][j] << std::endl;
                }
                mismatchCount++;
                allMatch = false;
            }
        }
    }
    
    if (allMatch) {
        std::cout << "Verification passed: All matrix elements match!" << std::endl;
    } else {
        std::cout << "Verification failed: " << mismatchCount << " mismatches found!" << std::endl;
    }
    std::cout.flush();
}

#endif // DEBUG_H 