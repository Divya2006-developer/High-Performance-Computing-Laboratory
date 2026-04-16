#include<stdio.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include<sys/shm.h>

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

    printf("Enter no. of rows and cols in A: ");
    scanf("%d %d", &m, &n);
    int **matA=(int **)malloc(m*sizeof(int *));
    for(i=0;i<m;i++){
	  matA[i]=(int *)malloc(n*sizeof(int));
    }
    for(i=0; i<m; i++) for(j=0; j<n; j++) matA[i][j]=rand()%10;
    printf("Enter no. of rows and cols in B: ");
    scanf("%d %d", &p, &q);
    int **matB=(int **)malloc(p*sizeof(int *));
    for(i=0;i<p;i++){
       matB[i]=(int *)malloc(q*sizeof(int));
    }
    
    for(i=0; i<p; i++) for(j=0; j<q; j++) matB[i][j]=rand()%10;
    clock_t start = clock();
    int c[m][n];
    int d[m][n];
     
        for(i=0; i<m; i++) {
            for(j=0; j<n; j++) c[i][j]= matA[i][j] + matB[i][j];
        }
 
        for(i=0; i<m; i++) {
            for(j=0; j<n; j++) d[i][j]= matA[i][j] - matB[i][j];
            
        }

        if(n != p) { printf("Not possible\n"); }
        else {
            for(i=0; i<m; i++) {
                for(j=0; j<q; j++) {
                    int res = 0;
                    for(k=0; k<n; k++) res += matA[i][k] * matB[k][j];
                    
                }
                
            }
        }

        if(m == n && m <= 10) {
            int tA[10][10];
            for(i=0; i<m; i++) for(j=0; j<n; j++) tA[i][j] = matA[i][j];
            int p=determinant(tA, m);
        }
        if(p == q && p <= 10) {
            int tB[10][10];
            for(i=0; i<p; i++) for(j=0; j<q; j++) tB[i][j] = matB[i][j];
            int q= determinant(tB, p);
        }
	clock_t end=clock();
	double time=(double)(end-start)*1000/CLOCKS_PER_SEC;
	printf("Serial execution time %f ms.",time);
    
    return 0;
}
