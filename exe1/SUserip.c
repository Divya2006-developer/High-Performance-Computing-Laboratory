#include<stdio.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<time.h>
int main(){
   int m,n,p;
   printf("Enter the size of M, N, P :");
   scanf("%d %d %d",&m,&n,&p);
   int matA[m][n];
   int matB[n][p];
   int result[m][p];
   printf("Enter the elements in matrix A (%d x %d) :",m,n);
   for(int i=0;i<m;i++){
      for(int j=0;j<n;j++){
         scanf("%d",&matA[i][j]);
      }
   }
   printf("Enter the elements in matrix B (%d x %d) :",n,p);
   for(int i=0;i<n;i++){
      for(int j=0;j<p;j++){
         scanf("%d",&matB[i][j]);
      }
   }
   clock_t start=clock();
   for(int i=0;i<m;i++){
      for(int j=0;j<p;j++){
         result[i][j]=0;
         for(int k=0;k<n;k++){
            result[i][j]+=matA[i][k]*matB[k][j];
         }
      }
   }
   clock_t end=clock();
   printf("Resultant Matrix :\n");
   for(int i=0;i<m;i++){
      for(int j=0;j<p;j++){
         printf(" %d",result[i][j]);
      }
      printf("\n");
   }
   double time=((double) (end-start)/CLOCKS_PER_SEC);
   printf("Time take for serial execution : %f",time);
   return 0;
}
