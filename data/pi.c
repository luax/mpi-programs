#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/* Approximates PI using multiple processes
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    // The size of the group associated with a communicator
    int size = 0;
    int tag = 0;
    // An integer from the user
    int intervals = 0;

    double pi = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the size of the group
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Number of intervals: \n");
        fflush(stdout);
        int ret = scanf("%d", &intervals);
        if (ret < 0) { printf("Error reading input"); return 1; }
        MPI_Send(&intervals, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    }

    // Broadcasts intervals from the process with rank 0 to
    // all other processes of the communicator
    MPI_Bcast(&intervals, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double sum = 0.0;
    double step = 1.0 / (double) intervals;
    for (int i = rank; i < intervals; i += size) {
        double x = ((double) i)* step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi_part = sum * step;

    MPI_Reduce(&pi_part, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("pi: %lf \n", pi);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}

