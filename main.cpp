#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<iostream>
#include<stdint.h>
#include<math.h>
#include<omp.h>
extern "C" {
    #include "timer.h"
    #include "Lab3IO.h"
}

using namespace std;

void gauss_jordan_elimination(int threadcount){
    double** mat;
    int mat_size;
    double* sol;
    int index_of_curr_row; // Index of the current row we are assessing
    int column_num; // Incrementing variable
    // The maximum abs. value in column, the index # of the max row, and the value of the current row.
    int index_of_max, index_of_temp_max; 
    int i, j;
    double start_time,end_time, temp_max,max;
    double* swap;

    // Load matrix 
    if (Lab3LoadInput(&mat, &mat_size) == 1){
        cout << "Error loading input matrix" << endl;
        exit(1);
    }


    sol = CreateVec(mat_size);

    // Start with our matrix
    // Starting with the first column, find the ROW with the largest ABSOLUTE VALUE. Keep track of this ROW. SWAP this row with current row if not already the current row.

    // From algorithm in lab manual:
    // For column_num = 0 to mat_size - 1: 
    // From column_num to mat_size, find the row index_of_max_row that has maximum absolute value of the element in the k'th column
    // Swap row column_num and row index_of_max_row
    
    // GAUSSIAN ELIMINATION
    //For each column, from left to right
    omp_set_num_threads(threadcount);
    GET_TIME(start_time);
    //initializing threads based on thread_count, sharing the matrix as of right now. 
    #pragma omp parallel num_threads(threadcount) default(none) shared(mat,mat_size,sol,max,index_of_max) private(i,j,temp_max,column_num,index_of_curr_row, swap,index_of_temp_max)
    {
    for (column_num = 0; column_num < mat_size -1 ; column_num++){
        // Set default values before we check anything
        max = 0;
        index_of_max = 0;
        #pragma omp for
        // Find the row index_of_max_row that has maximum absolute value in specified column
        for (index_of_curr_row = column_num; index_of_curr_row < mat_size; index_of_curr_row++){
            if (fabs(mat[index_of_curr_row][column_num]) > max) {

                // If the absolute value of the current index is larger than the max, set new max
                index_of_temp_max = index_of_curr_row;
                temp_max = fabs(mat[index_of_curr_row][column_num]);
                //printf("max = %d\n",max);

                
            }
        }
        
        if (temp_max > max){
            #pragma omp critical
            {

            max = temp_max;
            index_of_max = index_of_temp_max;
                

            }
        }

        // Now that we have the index value of the max row, we can swap it with the top row that we are assessing
        // (This top row is just the column number since the 'top' row decreases diagonally)
        // However we only swap if its not the top row
        //using pragma omp single since theres only going to be 1 swap at a time. So multithreading wont be needed in this scenerio. 


        #pragma omp barrier

        #pragma omp single 
        {
            if (index_of_max != column_num){
                //printf("%d %d\n",index_of_max,column_num);
                swap = mat[column_num];
                mat[column_num] = mat[index_of_max];
                mat[index_of_max] = swap;
            }
            //printf("SWAP\n");
            //PrintMat(mat,mat_size,mat_size+1);
        }

        // Perform elimination
        // From Lab Manual:
        // For i = column_num + 1 to mat_size:
        //      temp = mat[i][column_num] / mat[column_num][column_num]
        //          For j = column_num to mat_size + 1:
        //              replace mat[i][j] with (mat[i][j] - temp*mat[column_num][j])

        #pragma omp for
        for (i = column_num+1; i < mat_size;i++){
            double temp = mat[i][column_num] / mat[column_num][column_num];
            for (j = column_num; j < mat_size + 1;j++){
                mat[i][j] = (mat[i][j] - (temp * mat[column_num][j]));
            }
        }

    }
    

    // JORDAN ELIMINATION
    // Eliminate elements above the main diagonal
    // Only d[i][column_num] and d[i][mat_size+1] need to be updated
    // From lab manual:
    // For k = n to 2: (which is n - 1 to 1 because lab manual indices start at 1)
    //      For i = 1 to k - 1:
    //          d[i][n+1] = d[i][n+1] - d[i][k] / d[k][k] * d[k][n+1]
    //          d[i][k] = 0

        for(column_num = mat_size - 1; column_num > 0; column_num--){
            #pragma omp for
            for(i = 0; i < column_num;i++){
                mat[i][mat_size] = mat[i][mat_size] - mat[i][column_num] / 
                    mat[column_num][column_num] * mat[column_num][mat_size];
                mat[i][column_num] = 0;
                
            }

        }

        #pragma omp single 
        {
            //printf("\nJordan\n");
            //PrintMat(mat,mat_size,mat_size+1);
        }

        // Obtain desired solution:
        #pragma omp for
        for(i = 0; i < mat_size; i++){
            sol[i] = mat[i][mat_size] / mat[i][i];
        }
    }
    
        GET_TIME(end_time);
        Lab3SaveOutput(sol,mat_size,end_time-start_time);
        
        // clean up 
        DestroyVec(sol);
        DestroyMat(mat,mat_size);
        


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