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
#include <queue>
#include <sys/sysinfo.h>
#include <omp.h>

#include "workgraph.h"

using namespace std;


//
// Globals:
//
static int _numThreads = 1;  // default to sequential execution

//
// Function prototypes:
//
static void ProcessCmdLineArgs(int argc, char* argv[]);


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
	
	// Starting vertex
	int start_vertex = wg.start_vertex();
	
	// Set to track the visited nodes
	unordered_set<int> visited;
	visited.insert(start_vertex);

	// Queue for BFS traversal. Stores which nodes shall be processed next
	queue<int> bfs;
	bfs.push(start_vertex);
	
	// Buffer to keep track of vertices to process for the current iteration
	vector<int> current_level;
	
	// Counter to track the number of processed cells
	int cells = 0;

	// Size of the current level
	int current_level_size = 0;

	/*
		To solve the problem, BFS traversal is used to access all the vertices and the traversal is parallelized.
		The idea is to 
			- extract all the vertices that need to be processed at the current level,
			- perform the required work for each vertex, 
			- check their unvisited neighbors and,
			- push the unvisited neighbors into the queue for future processing.

		The work done on the vertices at the current level i.e. calling of do_work
		(this is the most time-consuming operation) is parallelized
		Other operations like pushing into the BFS queue or marking vertices as visited are lightweight 
		and can be handled sequentially.

		The BFS queue, the visited vertices set, and the cell counter are shared resources among multiple threads,
		they must be protected using critical sections or atomic operations to ensure that updating these 
		resources is thread safe.
	*/
	while(not bfs.empty()) {
		// Clear the vertices from previous level
		current_level.clear();

		// Get the number of vertices to process at the current level
		current_level_size = bfs.size();

		// Get the vertices to be processed into the current level buffer
		for (int i = 0; i < current_level_size; ++i) {
			current_level.push_back(bfs.front());
			bfs.pop();
		}

		// Parallelizing the visiting of vertices, doing the work, and getting the unvisited neighbors
		#pragma omp parallel for num_threads(_numThreads) schedule(dynamic)
		for (int i = 0; i < current_level_size; ++i) {
			int vertex = current_level[i];
			vector<int> neighbors = wg.do_work(vertex);

			// Visit each neighbor for the current vertex
			for (auto neighbor : neighbors) {

				// Flag to check if a vertex was visited
				bool vertex_visited = false;

				// Critical pragma because visited set and BFS queue are shared
				// and must be accessed safely by only one thread at a time
				#pragma omp critical
				{	
					// If the neighbor is unvisited, then mark it visited and add it to the queue
					if (visited.find(neighbor) == visited.end()) {
						visited.insert(neighbor);
						bfs.push(neighbor);
						vertex_visited = true;
					}
				}

				// Increment the counter only if a new vertex was visited
				// otherwise it will show false progress
				if (vertex_visited) {
					#pragma omp atomic
					cells++;
				}

				// Display progress for every 500 vertices visited
				if(cells % 500 == 0) {
					cout << ".";
					cout.flush();
				}
			}
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
