#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
void catch_child(int signo)
{
    pid_t wpid;
    int status;
    while((wpid=waitpid(-1,&status,0))!=-1)   //循环回收子进程，防止僵尸进程出现
    {
        if(WIFEXITED(status))
        printf("------------------catch child id %d ,ret = %d\n",wpid,WIFEXITED(status));
    }
 
}
int main()
{
    pid_t pid;
    //阻塞
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);
    int i;
    for(i=0;i<5;i++)
        if((pid=fork())==0)
        break;
        if(i==5)
        {   
            struct sigaction act;
            act.sa_handler=catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags=0;
            sigaction(SIGCHLD,&act,NULL);
            printf("l am parent , pid= %d \n",getpid());
            sigprocmask(SIG_UNBLOCK,&set,NULL);//接触阻塞,如果没有接触阻塞，回调函数就没有执行的机会
            while(1);//模拟执行后续代码
        }
        else
        {
            printf("l am child %d , pid = %d\n",i,getpid());
        }
    
    return 0;
}