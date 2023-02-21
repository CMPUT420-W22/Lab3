#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<iostream>
#include<stdint.h>
#include<math.h>
extern "C" {
    #include "timer.h"
    #include "Lab3IO.h"
}

using namespace std;

int gauss_jordan_elimination(int threadcount){
    double** mat;
    int mat_size;
    double* rowVec;
    int index_of_curr_row; // Index of the current row we are assessing
    int column_num; // Incrementing variable
    // The maximum abs. value in column, the index # of the max row, and the value of the current row.
    int max, index_of_max, temp_max, index_of_temp_max; 

    // Load matrix 
    if (Lab3LoadInput(&mat, &mat_size) == 1){
        cout << "Error loading input matrix" << endl;
        exit(1);
    }

    PrintMat(mat, mat_size, mat_size+1);
    
    rowVec = CreateVec(mat_size);

    // Start with our matrix
    // Starting with the first column, find the ROW with the largest ABSOLUTE VALUE. Keep track of this ROW. SWAP this row with current row if not already the current row.

    // From algorithm in lab manual:
    // For column_num = 0 to mat_size - 1: 
    // From column_num to mat_size, find the row index_of_max_row that has maximum absolute value of the element in the k'th column
    // Swap row column_num and row index_of_max_row
    
    //For each column, from left to right
    for (column_num = 0; column_num < mat_size - 1; column_num++){
        // Set default values before we check anything
        max = 0;
        index_of_max = 0;

        // Find the row index_of_max_row that has maximum absolute value in specified column
        for (index_of_curr_row = column_num; index_of_curr_row < mat_size; index_of_curr_row++){
    
            if (fabs(mat[index_of_curr_row][column_num]) > max) {
                // Critical section
                // If the absolute value of the current index is larger than the max, set new max
                max = fabs(mat[index_of_curr_row][column_num]);
                index_of_max = index_of_curr_row;
            }

        }

        // Now that we have the index value of the max row, we can swap it with the top row that we are assessing
        // (This top row is just the column number since the 'top' row decreases diagonally)
        // However we only swap if its not the top row
        int top_row = column_num;
        int row_to_swap = index_of_max;
        double* temp;
        if (row_to_swap != top_row){
            temp = mat[top_row];
            mat[top_row] = mat[row_to_swap];
            mat[row_to_swap] = temp;
        }
        PrintMat(mat, mat_size, mat_size+1);
    }


}


int main(int argc, char* argv[]){
    int threadcount;
    
    if (argc != 2){
        // Check arguments
        cout << "Call the program like: ./main <threadcount>" << endl;
        exit(0);
    } else {
        // Get thread count from arguments
        threadcount = atoi(argv[1]);
    }

    gauss_jordan_elimination(threadcount);
}