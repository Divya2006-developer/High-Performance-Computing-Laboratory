#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


    clock_gettime(CLOCK_MONOTONIC, &start);

    printf("\nAddition:\n");
    for(i=0; i<m; i++) {
        for(j=0; j<n; j++) printf("%d ", matA[i][j] + matB[i][j]);
        printf("\n");
    }

    printf("\nSubtraction:\n");
    for(i=0; i<m; i++) {
        for(j=0; j<n; j++) printf("%d ", matA[i][j] - matB[i][j]);
        printf("\n");
    }

    printf("\nMultiplication:\n");
    if(n != p) { 
        printf("Not possible\n"); 
    } else {
        for(i=0; i<m; i++) {
            for(j=0; j<q; j++) {
                int res = 0;
                for(k=0; k<n; k++) res += matA[i][k] * matB[k][j];
                printf("%d ", res);
            }
            printf("\n");
        }
    }

    printf("\nDeterminants:\n");
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
    long long diff_ns = (long long)(end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    printf("\nSerial execution time: %lld ns\n", diff_ns);
    
    return 0;
}
