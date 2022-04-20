#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<errno.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork");
        exit(1);
    }
    else  if(pid==0)//子进程
    {
        printf("child pid = %d ,ppid = %d\n",getpid(),getppid());
        sleep(2);
        kill(getppid(),SIGKILL);
    }
    else//父进程
    {
        printf("parent pid = %d\n",getpid());
        while(1);
    }



    return 0;
}