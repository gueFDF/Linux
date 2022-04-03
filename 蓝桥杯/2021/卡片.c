#include<stdio.h>
#include<string.h>
int arr[10]={2021,2021,2021,2021,2021,2021,2021,2021,2021,2021};//用来存1到9的卡片

int main()
{
    int i=1;
    int p;
   // memset(arr,2021,sizeof(int)*10);
    while(1)
    {
        p=i;
        while(p)
        {
            if(arr[p%10])
            {
               arr[p%10]--;
               p/=10;
            }
            else
            {
                printf("%d\n",i-1);
                return 0;
            }
        }
        i++;
    }
    return 0;
}