#include<signal.h>
#include<stdio.h>
#include<unistd.h>
void sig_catch(int signo)
{
    printf("catch you ! %d\n",signo);
    sleep(3);
} 
int main()
{
    signal(SIGINT,sig_catch);
    while(1);
    return 0;
}
