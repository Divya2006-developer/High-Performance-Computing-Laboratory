include<stdlib.h>
#include<pthread.h>
#define MAX 20
int arr[MAX];
int n,sum=0;
void *calculateSum(void *arg){
   int i;
   for(i=0;i<n;i++){
      sum+=arr[i];
   }
   pthread_exit(NULL);
}
int main(){
   pthread_t t1;
   int i;
   printf("Enter no.of elements :");
   scanf("%d",&n);
   printf("Enter %d elements :\n",n);
   for(i=0;i<n;i++){
      scanf("%d",&arr[i]);
   }
   pthread_create(&t1,NULL,calculateSum,NULL);
   pthread_join(t1,NULL);
   printf("\nSum of %d elements = %d\n",n,sum);
   return 0;
}
