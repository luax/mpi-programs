#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#ifdef CODE_1

/* Reads an integer from the terminal and distributes it
 * to all MPI processes.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    // The size of the group associated with a communicator
    int size = 0;
    // An integer from the user
    int given_number = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    // Set the size of the group
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Read until negative integer is given
    while (given_number >= 0) {
        // Read integer from terminal at process 0
        if (rank == 0) {
            printf("Enter number: \n");
            fflush(stdout);
            int ret = scanf("%d", &given_number);
            if (ret < 0) { printf("Error reading input"); return 1; }
        }

        // Broadcasts a message from the process with rank 0 to
        // all other processes of the communicator
        MPI_Bcast(&given_number, 1, MPI_INT, 0, MPI_COMM_WORLD);

        printf("Process %d received number %d \n", rank, given_number);
        fflush(stdout);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}
#endif
#ifdef CODE_2

typedef struct {
    int x;
    double y;
} Numbers;

/* Reads two numbers from the terminal and distributes it
 * to all MPI processes using a broadcast.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;
    // The size of the group associated with a communicator
    int size = 0;
    // User input
    int user_input_integer = 0;
    double user_input_double = 0;

    // Struct to be broadcasted
    Numbers numbers;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    MPI_Datatype numbers_type;
    // Number of blocks
    int count = 2;
    // Number of elements in each block
    int blocklengths[2] = { 1, 1 };
    // Byte displacement in each block
    MPI_Aint displacements[2] = { offsetof(Numbers, x), offsetof(Numbers, y)} ;
    // Type of element in each block
    MPI_Datatype types[2] = { MPI_INT, MPI_DOUBLE };

    // Create the MPI datatype
    MPI_Type_create_struct(count, blocklengths, displacements, types,
        &numbers_type);
    // Type can now be used for communication
    MPI_Type_commit(&numbers_type);

    // Set the size of the group
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Read until negative number is given
    while (user_input_integer >= 0 && user_input_double >= 0) {
        // Read integer from terminal at process 0
        if (rank == 0) {
            printf("Enter number: \n");
            fflush(stdout);
            int ret = scanf("%d", &numbers.x);
            if (ret < 0) { printf("Error reading input"); return 1; }
            ret = scanf("%lf", &numbers.y);
            if (ret < 0) { printf("Error reading input"); return 1; }
        }

        // Broadcasts a message from the process with rank 0 to
        // all other processes of the communicator
        MPI_Bcast(&numbers, 1, numbers_type, 0, MPI_COMM_WORLD);

        printf("Process %d received numbers %d and %lf \n", rank, numbers.x,
            numbers.y);
        fflush(stdout);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}

#endif
#ifdef CODE_3

/* Reads two numbers from the terminal and distributes it
 * to all MPI processes using a broadcast with MPI_Pack.
 */
int main(int argc, char **argv) {
    // The Rank of the calling process in the communicator
    int rank = 0;

    // User input
    int user_input_integer = 0;
    double user_input_double = 0;

    // Initialize the MPI execution environment
    MPI_Init(&argc, &argv);

    int position = 0;
    int upper_bound = 0;
    int size = 0;

    // Create buffer with size of an int and double
    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &upper_bound);
    size += upper_bound;
    MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &upper_bound);
    size += upper_bound;
    char buffer[size];

    // Set the rank of the calling process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Read until negative number is given
    while (user_input_integer >= 0 && user_input_double >= 0) {
        // Read integer from terminal at process 0
        if (rank == 0) {
            printf("Enter number: \n");
            fflush(stdout);
            int ret = scanf("%d", &user_input_integer);
            if (ret < 0) { printf("Error reading input"); return 1; }
            ret = scanf("%lf", &user_input_double);
            if (ret < 0) { printf("Error reading input"); return 1; }

            position = 0;
            MPI_Pack(&user_input_integer, 1, MPI_INT, buffer, size, &position,
                MPI_COMM_WORLD);
            MPI_Pack(&user_input_double, 1, MPI_DOUBLE, buffer, size, &position,
                MPI_COMM_WORLD);
        }

        // Send the pack
        MPI_Bcast(buffer, size, MPI_PACKED, 0, MPI_COMM_WORLD);

        position = 0;
        MPI_Unpack(buffer, size, &position, &user_input_integer, 1,
            MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, size, &position, &user_input_double, 1,
            MPI_DOUBLE, MPI_COMM_WORLD);
        printf("Process %d received numbers %d and %lf \n", rank,
            user_input_integer, user_input_double);
        fflush(stdout);
    }

    // Terminate the MPI execution environment
    MPI_Finalize();
}

#endif
