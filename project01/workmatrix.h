/*workmatrix.h*/

//
// Header file for WorkMatrix class. Each cell of the work matrix takes 
// a random amount of time, so there's no predictable pattern to the
// overall workload.
//
// Prof. Joe Hummel
// Northwestern University
//

#pragma once

class WorkMatrix {
    public:
      
      // default constructor:
      WorkMatrix();

      // copy constructor:
      WorkMatrix(const WorkMatrix& other);

      // destructor:
      ~WorkMatrix();

      // matrix info:
      int num_rows();
      int num_cols();

      // do_work: solve the work in the given cell [row][col]
      bool do_work(int row, int col);
};
