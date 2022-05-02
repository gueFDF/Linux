#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
int main()
{
    pid_t pid,wpid;
    int  status;
    pid =fork();
    if(pid==0)
    {
        printf("---child,my  parent =%d,going to sleep 10s----\n",getppid());
        sleep(10);
        printf("----child die---------\n");
        return 73;
    }
    else if(pid>0)
    {
        printf("-----my child = %d\n",pid);
          //wpid=wait(NULL)   //不关心子进程结束原因
            wpid=wait(&status);//如果子进程未终止，父进程阻塞在这个函数上      //一次wait或waitpid只能回收一个子进程
            if(wpid==-1)
            {
                perror("wait error");
                exit(1);
            }
            if(WIFEXITED(status))//为真，说明子进程正常终止
            {
                printf("child exit with %d\n",WEXITSTATUS(status));  //获取子进程的返回值
            }
            if(WIFSIGNALED(status))//为真，说明子进程是被信号终止
            {
                printf("child exit with %d\n",WTERMSIG(status));  //获取子进程的返回值
            }
        printf("-------parent wait finish--\n");
    }
    else
    {
        perror("fork");
        return 1;
    }

    return 0;
}