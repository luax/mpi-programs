#include <string.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv ) {
    int message_length = 20;
    char message[message_length];
    int tag_1 = 0;
    int tag_2 = 0;
    int size = 0;
    int rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        strcpy(message, "Hello, world");
        for (int i=1; i < size; i++) {
            MPI_Send(message, message_length, MPI_CHAR, i, tag_1, MPI_COMM_WORLD);
            MPI_Send(message, message_length, MPI_CHAR, i, tag_2, MPI_COMM_WORLD);
        }
        printf("Process %d: %s \n", rank, message);
        for (int i=1; i < size; i++) {
            int process = 0;
            MPI_Recv(&process, 1, MPI_INT,MPI_ANY_SOURCE, tag_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d: Hello back\n", process);
        }
        for (int i=1; i < size; i++) {
            MPI_Recv(message, message_length, MPI_INT,MPI_ANY_SOURCE, tag_2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d: %s \n", i, message);
        }
    } else {
        MPI_Recv(message, message_length, MPI_CHAR, 0, tag_2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(message, message_length, MPI_CHAR, 0, tag_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        strcpy(message, "I am here!");
        MPI_Send(&rank, 1, MPI_INT, 0, tag_1, MPI_COMM_WORLD);
        MPI_Send(message, message_length, MPI_CHAR, 0, tag_2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return (0);
}
