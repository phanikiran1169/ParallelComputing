/* main.cpp */

//
// Parallelizes a generic "work graph" where work is randomly
// distributed across the vertices in the graph. Naive 
// parallelization works, but doesn't scale. A much more 
// dynamic solution is needed.
// 
// Usage:
//   work [-?] [-t NumThreads]
//
// Author:
//   <<Your Name>>
//   Northwestern University
// 
// Initial template:
//   Prof. Joe Hummel
//   Northwestern University
//

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <random>
#include <unordered_set>
#include <sys/sysinfo.h>
#include <omp.h>
#include <mutex>

#include "workgraph.h"

using namespace std;


//
// Globals:
//
static int _numThreads = 1;  // default to sequential execution
static std::mutex visited_mutex;   
static std::unordered_set<int> visited;

//
// Function prototypes:
//
static void ProcessCmdLineArgs(int argc, char* argv[]);

// Here we parallelize the BFS traversal using recursion
// The idea is to visit all reachable vertices in parallel 
// until we exhaust all the threads

// If the vertex was already visited, immediately return avoiding
// redundant processing

// Since multiple threads access the visited set, its access must be  
// protected using a mutex
void do_work(WorkGraph& wg, int vertex) 
{
	// Mutex to prevent concurrent access of visited set
	// by multiple threads
	{
		lock_guard<mutex> lock(visited_mutex);
        if (!visited.insert(vertex).second)
            return;
	}

	// Do work and get the neighbors
	vector<int> neighbors = wg.do_work(vertex);

	// Recursively do work for neighbors
	for (int neighbor : neighbors) {
        #pragma omp task
        do_work(wg, neighbor);
    }
}

//
// main:
//
int main(int argc, char *argv[])
{
	cout << "** Work Graph Application **" << endl;
	cout << endl;

	//
	// Set defaults, process environment & cmd-line args:
	//
	ProcessCmdLineArgs(argc, argv);

	WorkGraph wg;  // NOTE: wg MUST be created in sequential code

	cout << "Graph size:   " << wg.num_vertices() << " vertices" << endl;
	cout << "Start vertex: " << wg.start_vertex() << endl;
	cout << "# of threads: " << _numThreads << endl;
	cout << endl;

	cout << "working";
	cout.flush();

  	auto start = chrono::high_resolution_clock::now();
	
	// Parallel block with the specified number of threads
	#pragma omp parallel num_threads(_numThreads)
	{
		// Limit to a single thread because the
		// traversal begins from a single starting vertex
		#pragma omp single
		{
			do_work(wg, wg.start_vertex());
		}
	}


	auto stop = chrono::high_resolution_clock::now();
	auto diff = stop - start;
	auto duration = chrono::duration_cast<chrono::milliseconds>(diff);

	cout << endl;
	cout << endl;

	cout << endl;
	cout << "** Done!  Time: " << duration.count() / 1000.0 << " secs" << endl;
	cout << "** Execution complete **" << endl;
  	cout << endl;

	return 0;
}


//
// processCmdLineArgs:
//
static void ProcessCmdLineArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{

		if (strcmp(argv[i], "-?") == 0)  // help:
		{
			cout << "**Usage: work [-?] [-t NumThreads]" << endl << endl;
			exit(0);
		}
		else if ((strcmp(argv[i], "-t") == 0) && (i+1 < argc))  // # of threads:
		{
			i++;
			_numThreads = atoi(argv[i]);
		}
		else  // error: unknown arg
		{
			cout << "**Unknown argument: '" << argv[i] << "'" << endl;
			cout << "**Usage: work [-?] [-t NumThreads]" << endl << endl;
			exit(0);
		}

	}//for
}