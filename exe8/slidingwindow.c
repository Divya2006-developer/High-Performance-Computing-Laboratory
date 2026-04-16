#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
    int m,n,thread_count;
    printf("Enter number of elements :\n");
    scanf("%d",&m);

    int i,j;
    int arr[m];

    for(i=0;i<m;i++){
        arr[i]=rand()%10;
        printf("%d ",arr[i]);
    }

    int sarr[m];

    printf("\nEnter sliding window size : \n");
    scanf("%d",&n);

    printf("Enter number of threads : \n");
    scanf("%d",&thread_count);

#pragma omp parallel for num_threads(thread_count)
    for(i=0;i<m;i++){
        sarr[i] = 0;  

        for(j=i;j<i+n && j<m;j++){  
            sarr[i]+=arr[j];
        }
    }

    printf("\nSliding window sums:\n");
    for(i=0;i<m;i++){
        printf("%d ",sarr[i]);
    }
    printf("\n");
    return 0;
}
