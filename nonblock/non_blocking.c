#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#ifdef CODE_1

int main(int argc, char **argv ) {
    int message_length = 20;
    char message[message_length];
    int size = 0;
    int rank = 0;
    int tag_1 = 0;
    int tag_2 = 1;

    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        strcpy(message, "Hello, world");
        for (int i=1; i < size; i++) {
            MPI_Isend(message, message_length, MPI_CHAR, i, tag_1,
                MPI_COMM_WORLD, &request);
        }
        printf("Process %d: %s \n", rank, message);
        int process = 0;
        for (int i=1; i < size; i++) {
            MPI_Irecv(&process, 1, MPI_INT, MPI_ANY_SOURCE, tag_1,
                MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            printf("Process %d: Hello back\n", process);
        }
        for (int i=1; i < size; i++) {
            MPI_Irecv(message, message_length, MPI_CHAR, MPI_ANY_SOURCE,
                tag_2, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            printf("Process %d: %s \n", i, message);
        }
    } else {
        MPI_Irecv(message, message_length, MPI_CHAR, 0, tag_1,
            MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        strcpy(message, "I am here!");
        MPI_Isend(&rank, 1, MPI_INT, 0, tag_1, MPI_COMM_WORLD, &request);
        MPI_Isend(message, message_length, MPI_CHAR, 0, tag_2,
            MPI_COMM_WORLD, &request);
    }
    MPI_Finalize();
    return 0;
}

#endif

#ifdef CODE_2

/* Parallel 12 x 12 matrix with 4 processes.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;

    int tag = 0;
    int size = 0;

    // The sum of errors
    int total_errors = 0;

    MPI_Status status;

    double start_time, end_time = 0.0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 3 * size;

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

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Get new values from by sending and receiving
    // rank = size - 1 only sends and receives data from rank = size - 2
    // rank = 0 only sends and receives data from rank = 1
    if (rank == 0) {
        MPI_Sendrecv(x[n/size], n, MPI_INT, rank + 1, tag,
            x[n/size+1], n, MPI_INT, rank +1, tag, MPI_COMM_WORLD, &status);
    } else if (rank < size - 1) {
        MPI_Sendrecv(x[1], n, MPI_INT, rank - 1, tag,
            x[0], n, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(x[n/size], n, MPI_INT, rank + 1, tag,
            x[n/size+1], n, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
    } else {
        MPI_Sendrecv(x[1], n, MPI_INT, rank - 1, tag,
            x[0], n, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

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

    // Terminate the MPI execution environment
    MPI_Finalize();

    if (rank == 0) {
        printf("Errors: %d \n", total_errors);
        printf("Processes: %d, Msize: %d, Psize: %d \n", size, n, n/size);
        printf("Execution time: %lf s \n", end_time-start_time);
    }
}

#endif

#ifdef CODE_3

/* Parallel 12 x 12 matrix with 4 processes.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;

    int tag = 0;
    int size = 0;

    // The sum of errors
    int total_errors = 0;

    MPI_Status status;

    double start_time, end_time = 0.0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 3 * size;

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

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Process 0 and 1 exchange
    // Process 2 and 3 exchange
    // Process 1 and 2 exchange
    // Process 3 and 4 exchange
    int dest_up = rank + 1 >= size ? MPI_PROC_NULL : rank + 1;
    int dest_down = rank - 1 < 0   ? MPI_PROC_NULL : rank - 1;

    if (rank % 2 == 0) {
       // printf("1 Process: %d, Dest up: %d\n", rank, dest_up);
        MPI_Sendrecv(x[n/size], n, MPI_INT, dest_up, tag,
            x[n/size+1], n, MPI_INT, dest_up, tag, MPI_COMM_WORLD, &status);
        // printf("2 Process: %d, Dest down: %d\n", rank, dest_down);
        MPI_Sendrecv(x[1], n, MPI_INT, dest_down, tag,
            x[0], n, MPI_INT, dest_down, tag, MPI_COMM_WORLD, &status);
    } else {
        // printf("1 Process: %d, Dest down:%d\n", rank, dest_down);
        MPI_Sendrecv(x[1], n, MPI_INT, dest_down, tag,
            x[0], n, MPI_INT, dest_down, tag, MPI_COMM_WORLD, &status);
        // printf("2 Process: %d, Dest up: %d\n", rank, dest_up);
        MPI_Sendrecv(x[n/size], n, MPI_INT, dest_up, tag,
            x[n/size+1], n, MPI_INT, dest_up, tag, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

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
        printf("Processes: %d, Msize: %d, Psize: %d \n", size, n, n/size);
        printf("Execution time: %lf s \n", end_time-start_time);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}
#endif
