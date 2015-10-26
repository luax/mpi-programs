#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


#define TIME_LIMIT 5
#define NUM_OF_LOCATIONS 200

#define MASTER 0

int master_worker();
void slave_worker();

int LOCATIONS[NUM_OF_LOCATIONS];

int RANK = 0;
int SIZE = 0;

int TAG = 0;

int main(int argc, char **argv) {
    double start_time, end_time = 0.0;
    int total_strikes = 0;

    start_time = MPI_Wtime();

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &SIZE);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);

    // Seed random generator
    srand(RANK);

    while ((end_time - start_time) < TIME_LIMIT) {
        if (RANK != MASTER) {
            slave_worker();
        } else {
            total_strikes += master_worker();
        }
        fflush(stdout);
        end_time = MPI_Wtime();
    }

    printf("Process: %d, time: %f\n", RANK, end_time-start_time);

    if (RANK == MASTER) {
        printf("Strikes: %d\n", total_strikes);
        printf("Execution time: %lf s \n", end_time - start_time);
        fflush(stdout);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
    return 0;
}

int master_worker() {
    int strikes = 0;

    // Receive numbers
    for (int i = MASTER + 1; i < SIZE; i++) {
        int number = 0;
        MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        LOCATIONS[number]++;
    }

    // Generate a random number between 0 and NUM_OF_LOCATIONS
    int r = rand() % NUM_OF_LOCATIONS;
    LOCATIONS[r]++;

    // Count locations
    for (int i = 0; i < NUM_OF_LOCATIONS; ++i) {
        if (LOCATIONS[i] >= 4)  {
            ++strikes;
        }
        LOCATIONS[i] = 0;
    }

    return strikes;
}

void slave_worker() {
    // Generate a random number between 0 and NUM_OF_LOCATIONS
    int r = rand() % NUM_OF_LOCATIONS;

    // Send number
    // MPI_Request request;
    // MPI_Isend(&r, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &request);
    MPI_Send(&r, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
}
