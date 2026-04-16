#include<stdio.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<time.h>
int main(){
   int m,n,p,i;
   printf("Enter the size of M, N, P :");
   scanf("%d %d %d",&m,&n,&p);
   int **matA=(int **)malloc(m*sizeof(int*));
   for(i=0;i<n;i++){
            matA[i]=(int *)malloc(n*sizeof(int));
   }
   int **matB=(int **)malloc(n*sizeof(int*));
   for(i=0;i<p;i++){
            matB[i]=(int *)malloc(p*sizeof(int));
   }
   int **result=(int **)malloc(m*sizeof(int *));
   for(i=0;i<p;i++){
            result[i]=(int *)malloc(p*sizeof(int));
   }
   for(int i=0;i<m;i++){
      for(int j=0;j<n;j++){
         matA[i][j]=rand()%10;
      }
   }
   
   for(int i=0;i<n;i++){
      for(int j=0;j<p;j++){
         matB[i][j]=rand()%10;
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
    printf("Result of %d X %d matrix execution\n",m,p);
   double time=((double) (end-start)/CLOCKS_PER_SEC);
   printf("Time take for serial execution : %f",time);
   return 0;
}
