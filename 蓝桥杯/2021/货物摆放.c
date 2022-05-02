#include<stdio.h>
long long yue[100000];
int n=0;
int main()
{
    int cnt=0;
    long long a=2021041820210418;
    for(long long i=1;i*i<=a;i++)
    {
        if(a%i==0)
        {
            yue[n++]=i;
            if(i*i!=a)
            {
                yue[n++]=a/i;
            }
        
        }
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if((n%(yue[i]*yue[j]))==0)
            cnt++;
        }
    }
    printf("%d %d\n",n,cnt);
}