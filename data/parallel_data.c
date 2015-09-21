#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/* Parallel 12 x 12 matrix with 4 processes.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    int size = 0;
    int tag = 0;
    // The sum of errors
    int total_errors = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = size * 5;

    // Two extra rows
    int x[n/size + 2][n];

    // Set matrix values
    for (int i = 1; i <= n/size; ++i) {
        for (int j = 0; j < n; ++j) {
            x[i][j] = rank;
        }
    }
    // Top and bottom row has ghost points
    for (int i = 0; i < n; ++i) {
        x[0][i] = -1;
        x[n/size+1][i] = -1;
    }

    // Get new values from by sending and receiving
    // rank = size - 1 only sends and receives data from rank = size - 2
    // rank = 0 only sends and receives data from rank = 1
    if (rank == 0) {
        MPI_Send(x[n/size], n, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
        MPI_Recv(x[n/size+1], n, MPI_INT, rank + 1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    } else if (rank < size - 1) {
        MPI_Send(x[1], n,        MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
        MPI_Recv(x[0], n,        MPI_INT, rank - 1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        MPI_Send(x[n/size], n,   MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
        MPI_Recv(x[n/size+1], n, MPI_INT, rank + 1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    } else {
        MPI_Send(x[1], n, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
        MPI_Recv(x[0], n, MPI_INT, rank - 1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    }

    int errors = 0;
    // Check values
    for (int i = 1; i <= n/size; ++i) {
        for (int j = 0; j < n; ++j) {
            errors += (x[i][j] != rank) * 1;
        }
    }
    for (int i = 0; i < n; ++i) {
        errors += (x[0][i] != rank-1) * 1;
        if (rank < size - 1) {
            errors += (x[n/size+1][i] != rank+1) * 1;
        }
    }

    MPI_Reduce(&errors, &total_errors, 1, MPI_INT, MPI_SUM,
        0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Errors: %d \n", total_errors);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}

