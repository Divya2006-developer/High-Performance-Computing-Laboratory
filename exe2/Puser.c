#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>


long long get_nanos(struct timespec start, struct timespec end) {
    return (long long)(end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
}

int determinant(int mat[10][10], int n) {
    int det = 0;
    int submat[10][10];
    if (n == 1) return mat[0][0];
    if (n == 2) return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);

    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                submat[subi][subj] = mat[i][j];
                subj++;
            }
            subi++;
        }
        det += (x % 2 == 0 ? 1 : -1) * mat[0][x] * determinant(submat, n - 1);
    }
    return det;
}

int main() {
    int m, n, p, q, i, j, k;
    struct timespec start, end;
    

    int sid = shmget(IPC_PRIVATE, 4 * sizeof(long long), IPC_CREAT | 0666);
    if (sid < 0) { perror("shmget"); exit(1); }
    long long *max_times = (long long *)shmat(sid, NULL, 0);

    printf("Enter no. of rows and cols in A: ");
    scanf("%d %d", &m, &n);
    int matA[m][n];
    printf("Enter elements of A:\n");
    for(i=0; i<m; i++) for(j=0; j<n; j++) scanf("%d", &matA[i][j]);

    printf("Enter no. of rows and cols in B: ");
    scanf("%d %d", &p, &q);
    int matB[p][q];
    printf("Enter elements of B:\n");
    for(i=0; i<p; i++) for(j=0; j<q; j++) scanf("%d", &matB[i][j]);

    if(fork() == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        printf("\n[Child 1] Addition:\n");
        for(i=0; i<m; i++) {
            for(j=0; j<n; j++) printf("%d ", matA[i][j] + matB[i][j]);
            printf("\n");
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        max_times[0] = get_nanos(start, end);
        exit(0);
    }

    if(fork() == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        printf("\n[Child 2] Subtraction:\n");
        for(i=0; i<m; i++) {
            for(j=0; j<n; j++) printf("%d ", matA[i][j] - matB[i][j]);
            printf("\n");
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        max_times[1] = get_nanos(start, end);
        exit(0);
    }

    if(fork() == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        printf("\n[Child 3] Multiplication:\n");
        if(n != p) { printf("Not possible\n"); }
        else {
            for(i=0; i<m; i++) {
                for(j=0; j<q; j++) {
                    int res = 0;
                    for(k=0; k<n; k++) res += matA[i][k] * matB[k][j];
                    printf("%d ", res); 
                }
                printf("\n");
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        max_times[2] = get_nanos(start, end);
        exit(0);
    }

    if(fork() == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        printf("\n[Child 4] Determinants:\n");
        if(m == n && m <= 10) {
            int tA[10][10];
            for(i=0; i<m; i++) for(j=0; j<n; j++) tA[i][j] = matA[i][j];
            printf("Det A: %d\n", determinant(tA, m));
        }
        if(p == q && p <= 10) {
            int tB[10][10];
            for(i=0; i<p; i++) for(j=0; j<q; j++) tB[i][j] = matB[i][j];
            printf("Det B: %d\n", determinant(tB, p));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        max_times[3] = get_nanos(start, end);
        exit(0);
    }

    for(i=0; i<4; i++) wait(NULL);

    long long max_t = max_times[0];
    for(i=1; i<4; i++) if(max_times[i] > max_t) max_t = max_times[i];

    printf("\nMaximum time taken: %lld ns\n", max_t);
    shmdt(max_times);
    shmctl(sid, IPC_RMID, NULL);
    return 0;
}

