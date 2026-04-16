#include<stdio.h>
#include<omp.h>
int main(){
   int n,i;
   printf("Enter the no.of elements :\n");
   scanf("%d",&n);
   int ar[n];
   printf("Enter the elements :\n");
   for(i=0;i<n;i++){
      scanf("%d",&ar[i]);
   }
   int count,target,thread_count;
   printf("Enter the target :\n");
   scanf("%d",&target);
   count=0;
   printf("Enter the thread count :\n");
   scanf("%d",&thread_count);
#pragma omp parallel for num_threads(thread_count) reduction(+:count)
   for(i=0;i<n;i++){
      if(ar[i]<target){
	 count++;
      }
   }
   printf("No of elements less than the target %d is %d\n",target,count);
   return 0;
}
