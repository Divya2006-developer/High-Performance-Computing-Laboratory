#include<stdio.h>
#include<omp.h>
int main(){
   int m,n,thread_count;
   printf("No.of Elements in Arrays A and B : ");
   scanf("%d",&m);
   printf("No.of threads: ");
   scanf("%d",&thread_count);
   int matA[m];
   int matB[m];
   int result[m];
   int i,j;
   printf("Array A :\n");
   for(i=0;i<m;i++){
	 matA[i]=rand()%10;
	 printf("%d ",matA[i]);
   }
   printf("\n");
   printf("Array B :\n");
   for(i=0;i<m;i++){
      matB[i]=rand()%10;
      printf("%d ",matB[i]);
   }
   printf("\n");
   #pragma omp parallel for num_threads(thread_count)
   for(i=0;i<m;i++){
	 result[i]=matA[i]+matB[i];
      }
   printf("Resultant Array :\n");
   for(i=0;i<m;i++){
	 printf("%d ",result[i]);
   }
   printf("\n");
   return 0;
}
