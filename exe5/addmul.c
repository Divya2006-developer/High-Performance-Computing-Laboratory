#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10
int A[MAX][MAX], B[MAX][MAX], ADD[MAX][MAX], MUL[MAX][MAX];
int r1, c1, r2, c2;

void* addition(void *arg) {
    if (r1 != r2 || c1 != c2) {
        printf("\nAddition not possible: Dimensions mismatch.\n");
        pthread_exit(NULL);
    }
    for(int i=0; i<r1; i++)
        for(int j=0; j<c1; j++)
            ADD[i][j] = A[i][j] + B[i][j];
    pthread_exit(NULL);
}

void* multiplication(void *arg) {
    if (c1 != r2) {
        printf("\nMultiplication not possible: c1 must equal r2.\n");
        pthread_exit(NULL);
    }
    for(int i=0; i<r1; i++) {
        for(int j=0; j<c2; j++) {
            MUL[i][j] = 0;
            for(int k=0; k<c1; k++)
                MUL[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    printf("Enter rows and columns of A: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter elements of A:\n");
    for(int i=0; i<r1; i++)
        for(int j=0; j<c1; j++)
            scanf("%d", &A[i][j]);

    printf("Enter rows and columns of B: ");
    scanf("%d %d", &r2, &c2);
    printf("Enter elements of B:\n");
    for(int i=0; i<r2; i++)
        for(int j=0; j<c2; j++)
            scanf("%d", &B[i][j]);

    pthread_create(&t1, NULL, addition, NULL);
    pthread_create(&t2, NULL, multiplication, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    if (r1 == r2 && c1 == c2) {
        printf("\nAddition Result:\n");
        for(int i=0; i<r1; i++) {
            for(int j=0; j<c1; j++) printf("%d ", ADD[i][j]);
            printf("\n");
        }
    }

    if (c1 == r2) {
        printf("\nMultiplication Result:\n");
        for(int i=0; i<r1; i++) {
            for(int j=0; j<c2; j++) printf("%d ", MUL[i][j]);
            printf("\n");
        }
    }

    return 0;
}
