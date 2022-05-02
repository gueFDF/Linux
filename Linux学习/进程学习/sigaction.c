#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void sig_catch(int signo)
{
    printf("catch you ! %d\n",signo);
    sleep(3);

} 
int main()
{
    struct sigaction act,oldact;
    act.sa_handler = sig_catch;//set callback function name
    sigemptyset(&act.sa_mask);//清零set mask when sig_catch working
    act.sa_flags=0;            //usually use

    int ret=sigaction(SIGINT,&act,&oldact);
    if(ret==-1)
    {
        perror("error");
        exit(1);
    }
    while(1);
    return 0;
}