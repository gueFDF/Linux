#include<stdio.h>
#include<signal.h>
void print(sigset_t set)
{
    int i=0;
    for(i=0;i<32;i++)
    {
        if(sigismember(&set,i))
        putchar('1');
        else
        putchar('0');
    }
    printf("\n");
}
int main()
{
    sigset_t set;
    sigset_t oldset;
    sigset_t pedset;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oldset);
    sigpending(&pedset);
    print(pedset);
    while(1);
    return 0;
}