#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef struct line
{
    double k,b;
}line;
line arr[200000];
int  n=0;
int cmp(const void*e1,const void* e2)
{
    if(((line*)e1)->k>((line*)e2)->k)
    return 1;
    if(((line*)e1)->k==((line*)e2)->k)
    {
        return (((line*)e1)->b-((line*)e2)->k);
    }
    return -1;
}
int main()
{
    int  res;
    for(int x1=0;x1<20;x1++)
    for(int y1=0;y1<21;y1++)
    for(int x2=0;x2<20;x2++)
    for(int y2=0;y2<21;y2++)
    {
        if(x1!=x2)
        {
            double k1=(double)(y2-y1)/(x2-x1);
            double b1=y1-k1*x1;
            line temp = {.k=k1,.b=b1};
            arr[n++]=temp;
        }
    }
    qsort(arr,n,sizeof(arr[0]),cmp);
    for(int i=1;i<n;i++)
    {
        res=1;
        if(fabs(arr[i].k-arr[i-1].k)>1e-8||fabs(arr[i].b-arr[i-1].b)>1e-8)
       {
           printf("%ld %ld\n",arr[i].k,arr[i].b);
           res++;
       }
    }
   printf("%d\n",n);
    printf("%d\n",res+20);
}