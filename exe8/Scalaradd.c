#include<stdio.h>
#include<omp.h>
int main(){
   int m,s,thread_count,i;
   printf("Enter the no.of elements :");
   scanf("%d",&m);
   int arr[m];
   for(i=0;i<m;i++){
      arr[i]=rand()%10;
      printf("%d ",arr[i]);
   }
   printf("\n");
   printf("Enter the constant to add : ");
   scanf("%d",&s);
   printf("Enter no.of threads :");
   scanf("%d",&thread_count);
#pragma omp parallel for num_threads(thread_count)
   for(i=0;i<m;i++){
      arr[i]=arr[i]+s;
   }
   printf("Resultant array :\n");
   for(i=0;i<m;i++){
      printf("%d ",arr[i]);
   }
   printf("\n");
   return 0;
}

