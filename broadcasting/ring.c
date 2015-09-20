#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#ifdef CODE_1

/* Reads data from the user and sends it to all other
 * processes in a ring.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    // The size of the group associated with a communicator
    int size = 0;
    int tag = 0;
    // An integer from the user
    int user_input_integer = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the size of the group
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int recieved_number = 0;
    if (rank == 0) {
        printf("Enter number: \n");
        fflush(stdout);
        int ret = scanf("%d", &user_input_integer);
        if (ret < 0) { printf("Error reading input"); return 1; }
        MPI_Send(&user_input_integer, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    } else if (rank < size - 1) {
        MPI_Recv(&recieved_number, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        printf("Process %d received number %d \n", rank, recieved_number);
        MPI_Send(&recieved_number, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&recieved_number, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        printf("Process %d received number %d \n", rank, recieved_number);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}
#endif

#ifdef CODE_2

/* Reads data from the user and sends it to all other
 * processes in a topology.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    // The size of the group associated with a communicator
    int size = 0;
    int tag = 0;
    // An integer from the user
    int user_input_integer = 0;

    int rank_source = 0;
    int rank_dest = 0;
    int false = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the size of the group
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Makes a new communicator to which topology information has been attached
    MPI_Comm comm_cart;
    MPI_Cart_create(MPI_COMM_WORLD, 1, &size, &false, 1, &comm_cart);
    MPI_Cart_shift(comm_cart, 0, 1, &rank_source, &rank_dest);

    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(comm_cart, &rank);
    // Set the size of the group in the new communicator
    MPI_Comm_rank(comm_cart, &size);

    int recieved_number = 0;
    if (rank == 0) {
        printf("Enter number: \n");
        fflush(stdout);
        int ret = scanf("%d", &user_input_integer);
        if (ret < 0) { printf("Error reading input"); return 1; }
        MPI_Send(&user_input_integer, 1, MPI_INT, rank_dest, tag, comm_cart);
    } else {
        MPI_Recv(&recieved_number, 1, MPI_INT, rank_source, tag, comm_cart,
            MPI_STATUS_IGNORE);
        printf("Process %d received number %d \n", rank, recieved_number);
        MPI_Send(&recieved_number, 1, MPI_INT, rank_dest, tag, comm_cart);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}
#endif
