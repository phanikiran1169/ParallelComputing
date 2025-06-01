# Floyd-Warshall Algorithm Implementation

A modular C++ implementation of the Floyd-Warshall algorithm for finding shortest paths in a directed graph, designed with MPI parallelization in mind.

## Features

- **Modular Design**: Separate classes for Graph representation and Floyd-Warshall algorithm
- **Adjacency Matrix Representation**: Efficient matrix-based graph storage
- **Path Reconstruction**: Find not just shortest distances but actual paths
- **Performance Measurement**: Detailed timing and memory usage statistics  
- **MPI-Ready Structure**: Prepared for easy MPI parallelization
- **Interactive Queries**: Command-line interface for path queries
- **Verification**: Built-in algorithm correctness checking

## Files Structure

```
├── Graph.h              # Graph class header
├── Graph.cpp            # Graph class implementation  
├── FloydWarshall.h      # Floyd-Warshall algorithm header
├── FloydWarshall.cpp    # Floyd-Warshall algorithm implementation
├── main.cpp             # Main driver program
├── Makefile             # Build configuration
├── data.txt             # Input graph data (25,572 edges, 1,005 vertices)
└── README.md            # This documentation
```

## Graph Data Format

The `data.txt` file contains directed edges in the format:
```
source_vertex destination_vertex
```

Example:
```
0 1
2 3
5 6
...
```

- Vertices are numbered from 0 to 1004 (1,005 total vertices)
- Each line represents a directed edge with weight 1
- The graph has 25,572 edges

## Building and Running

### Prerequisites
- C++17 compatible compiler (g++, clang++)
- Make build system

### Build Commands

```bash
# Build the project
make

# Build with debug information
make debug

# Build optimized release version
make release

# Clean build artifacts
make clean

# Check if data.txt exists
make check-data

# Show all available targets
make help
```

### Running the Program

```bash
# Run with default settings (both sequential and parallel-ready versions)
make run
# or
./floyd_warshall

# Run only sequential version
make run-sequential
# or  
./floyd_warshall -s

# Run only parallel-ready version
make run-parallel
# or
./floyd_warshall -p

# Run with specific path query
make run-query
# or
./floyd_warshall -q 0 100

# Show usage information
./floyd_warshall -h
```

### Command Line Options

```
Usage: ./floyd_warshall [options]
Options:
  -f <filename>   Input file (default: data.txt)
  -v <vertices>   Number of vertices (default: 1005)
  -s              Run sequential algorithm only
  -p              Run parallel-ready algorithm only  
  -q <from> <to>  Query shortest path between vertices
  -h              Show help message
```

## Algorithm Details

### Floyd-Warshall Algorithm

The implementation uses the classic Floyd-Warshall algorithm:

```cpp
for (int k = 0; k < numVertices; k++) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (distMatrix[i][k] + distMatrix[k][j] < distMatrix[i][j]) {
                distMatrix[i][j] = distMatrix[i][k] + distMatrix[k][j];
                nextMatrix[i][j] = nextMatrix[i][k];
            }
        }
    }
}
```

### Time Complexity
- **Time**: O(V³) where V is the number of vertices
- **Space**: O(V²) for distance and next matrices

### MPI Parallelization Strategy

The code is structured for row-wise distribution across MPI processes:

1. **Data Distribution**: Each process handles a subset of rows (i values)
2. **Communication Pattern**: 
   - Broadcast row k to all processes before each k iteration
   - Gather updated rows after each k iteration
3. **Synchronization**: All processes must complete iteration k before moving to k+1

## Example Usage

### Basic Run
```bash
$ make run
=== Floyd-Warshall Algorithm Implementation ===
Input file: data.txt
Number of vertices: 1005

Loading graph from file...
Loaded 25572 edges from data.txt
Graph loaded successfully!
Graph has 1005 vertices

=== Running Sequential Floyd-Warshall ===
Starting sequential Floyd-Warshall algorithm...
Processing intermediate vertex 0/1005
Processing intermediate vertex 100/1005
...
Sequential Floyd-Warshall completed!
Total time: 2.45 seconds
Compute time: 2.31 seconds
Memory usage: 7.67 MB
```

### Path Query
```bash
$ ./floyd_warshall -q 0 100
Shortest path from 0 to 100 (distance 3): 0 -> 1 -> 85 -> 100
```

### Interactive Queries
```bash
$ ./floyd_warshall
...
Would you like to run interactive path queries? (y/n): y

=== Interactive Path Queries ===
Enter path queries (format: <from> <to>, or -1 -1 to exit):
Query (from to): 0 50
Shortest path from 0 to 50 (distance 2): 0 -> 1 -> 50
Query (from to): -1 -1
```

## Performance Characteristics

For the given dataset (1,005 vertices):
- **Memory Usage**: ~8 MB for distance and next matrices
- **Sequential Time**: ~2-3 seconds on modern hardware
- **Verification**: Built-in correctness checking

## MPI Parallelization Next Steps

To add full MPI support:

1. **Include MPI headers**: `#include <mpi.h>`
2. **Initialize MPI**: `MPI_Init(&argc, &argv)`
3. **Add communication**:
   ```cpp
   // Broadcast row k before each iteration
   MPI_Bcast(distMatrix[k], numVertices, MPI_INT, k % size, MPI_COMM_WORLD);
   
   // Gather results after each iteration 
   MPI_Allgather(localRows, rowsPerProcess * numVertices, MPI_INT,
                 distMatrix, rowsPerProcess * numVertices, MPI_INT, MPI_COMM_WORLD);
   ```
4. **Compile with MPI**: `mpic++ -o floyd_warshall_mpi *.cpp`
5. **Run with MPI**: `mpirun -np 4 ./floyd_warshall_mpi`

## Verification

The implementation includes automatic verification:
- Diagonal elements are zero (distance from vertex to itself)
- Triangle inequality holds for sample vertices
- Path reconstruction consistency

## License

This implementation is provided for educational and research purposes. 