#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

typedef struct {
    int r1, c1, r2, c2;
    double cpu_times[1000];   
    int data[];             
} shared_data;

int main() {
    int r1, c1, r2, c2;

    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &r1, &c1);

    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2) {
        printf("Matrix multiplication not possible!\n");
        return 1;
    }

 
    int sizeA = r1 * c1;
    int sizeB = r2 * c2;
    int sizeC = r1 * c2;

    int shm_size = sizeof(shared_data) +
                   (sizeA + sizeB + sizeC) * sizeof(int);

    int sid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0666);
    if (sid < 0) {
        perror("shmget failed");
        exit(1);
    }

    shared_data *shm = (shared_data *)shmat(sid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    shm->r1 = r1; shm->c1 = c1;
    shm->r2 = r2; shm->c2 = c2;

    int *matA = shm->data;
    int *matB = matA + sizeA;
    int *matC = matB + sizeB;

    srand(time(NULL));


    for (int i = 0; i < sizeA; i++)
        matA[i] = rand() % 10;   

    for (int i = 0; i < sizeB; i++)
        matB[i] = rand() % 10;


    for (int i = 0; i < r1; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            struct timespec start, end;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

            for (int j = 0; j < c2; j++) {
                matC[i * c2 + j] = 0;
                for (int k = 0; k < c1; k++) {
                    matC[i * c2 + j] +=
                        matA[i * c1 + k] * matB[k * c2 + j];
                }
            }

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

            shm->cpu_times[i] =
                (end.tv_sec - start.tv_sec) +
                (end.tv_nsec - start.tv_nsec) / 1e9;

            exit(0);
        }
    }


    for (int i = 0; i < r1; i++)
        wait(NULL);


    if (r1 <= 10 && c2 <= 10) {
        printf("\nResultant Matrix:\n");
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) {
                printf("%d ", matC[i * c2 + j]);
            }
            printf("\n");
        }
    }

 
    double max_time = 0;
    printf("\nCPU Time for each process:\n");
    for (int i = 0; i < r1; i++) {
        if (shm->cpu_times[i] > max_time)
            max_time = shm->cpu_times[i];
    }

    printf("Maximum CPU Time: %f seconds\n", max_time);

 
    shmdt(shm);
    shmctl(sid, IPC_RMID, NULL);

    return 0;
}
