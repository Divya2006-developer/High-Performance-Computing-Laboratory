#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
   int m,thread_count;
   printf("Enter size of a square matrix :");
   scanf("%d",&m);
   int arr[m][m],res[m][m];
   int i,j;
   for(i=0;i<m;i++){
      for(j=0;j<m;j++){
	 arr[i][j]=rand()% 10;
	 printf("%d ",arr[i][j]);
      }
      printf("\n");
   }
   printf("Enter no.of threads:");
   scanf("%d",&thread_count);
#pragma omp parallel for num_threads(thread_count)
   for(i=0;i<m;i++){
      for(j=0;j<m;j++){
	 res[i][j]=arr[i][m-1-j];
      }
   }
   printf("Resultant matrix : \n");
   for(i=0;i<m;i++){
      for(j=0;j<m;j++){
	 printf("%d ",res[i][j]);
      }
      printf("\n");
   }
   printf("\n");
   return 0;
}
