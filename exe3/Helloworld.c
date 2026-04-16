#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {
    char message[100];
    int i, comm_str, my_rank;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_str);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank != 0) {
        sprintf(message, "Hello World from process %d",my_rank);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
	printf("Hello World from process %d\n",my_rank);
        for (i = 1; i < comm_str; i++) {
            MPI_Recv(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", message);
        }
    }

    MPI_Finalize();
    return 0;
}
