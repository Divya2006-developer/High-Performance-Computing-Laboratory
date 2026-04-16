#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>

void toUpperCase(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int isPalindrome(char str[]) {
    int i = 0;
    int j = strlen(str) - 1;
    while (i < j) {
  
        if (tolower(str[i]) != tolower(str[j]))
            return 0;
        i++;
        j--;
    }
    return 1;
}

int main(int argc, char** argv) {
    char message[100];
    int i, comm_size, my_rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank != 0) {
        if (my_rank == 1) {
            sprintf(message, "Hello");
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else if (my_rank == 2) {
            sprintf(message, "Malayalam");
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        } else if (my_rank == 3) {
            sprintf(message, "Welcome");
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else if (my_rank == 4) {
            sprintf(message, "Hiii");
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        }
    } else {

        for (i = 0; i < comm_size-1; i++) {
            MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int sender = status.MPI_SOURCE;
            int tag = status.MPI_TAG;

            if (tag == 1) {
                toUpperCase(message);
                printf("From process %d (Type 1 - Uppercase): %s\n", sender, message);
            } else if (tag == 2) {
                if (isPalindrome(message)) {
                    printf("From process %d (Type 2): %s is a palindrome\n", sender, message);
                } else {
                    printf("From process %d (Type 2): %s is NOT a palindrome\n", sender, message);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
