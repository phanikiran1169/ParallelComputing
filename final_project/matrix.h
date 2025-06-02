#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>

template <class T>
T **New2dMatrix(int ROWS, int COLS)
{
    T **matrix; 
    T  *elements;

    matrix = new T*[ROWS];
    elements = new T[ROWS * COLS];

    for (int r = 0; r < ROWS; r++)
        matrix[r] = &elements[r * COLS];

    return matrix;
}

template <class T>
void Delete2dMatrix(T **matrix)
{
    if (matrix != nullptr) {
        delete[] matrix[0];
        delete[] matrix;
    }
}

template <class T>
void CopyMatrix(T **source, T **dest, int ROWS, int COLS)
{
    if (source != nullptr && dest != nullptr) {
        memcpy(dest[0], source[0], ROWS * COLS * sizeof(T));
    }
}


#endif // MATRIX_H 