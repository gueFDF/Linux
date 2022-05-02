#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<errno.h>
#include<unistd.h>
int main()
{
    int i=0;
    alarm(2);
    while(1)
    printf("%d\n",i++);
    return 0;
}
