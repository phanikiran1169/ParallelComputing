/*workgraph.h*/

//
// Header file for WorkGraph class. Each vertex in the work graph takes 
// a random amount of time, so there's no predictable pattern to the
// overall workload.
// 
// The graph is a directed graph, no multi-edges but it could have
// cycles. No "multi-edges" means if there is an edge from src to dest,
// then there's only one edge, not multiple edges from src to dest.
// Possible "cycles" means there could be an edge from a vertex back
// to itself, i.e. from src to src. You graph is connected, and you 
// are guaranteed there's a path from the starting vertex to all other
// vertices. The # of vertices is not fixed, and the vertices are
// identified by randomly-generated (i.e. unpredictable) integers.
//
// Prof. Joe Hummel
// Northwestern University
//

#pragma once

#include <vector>

class WorkGraph {
    public:
      
      // default constructor: must be called from sequential code
      WorkGraph();

      // copy constructor:
      WorkGraph(const WorkGraph& other);

      // destructor:
      ~WorkGraph();

      // # of vertices:
      int num_vertices();

      // start vertex:
      int start_vertex();

      //
      // do_work: solve the work in the given vertex, 
      // which as part of the computation generates
      // and returns the vertex's neighbors.
      //
      std::vector<int> do_work(int vertex);
};
