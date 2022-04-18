#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
int var=100;
int main()
{
    char *p;
    pid_t pid;
    int fd;
    fd=open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd<0)
    {
        perror("open error");
        exit(1);
    }
    lseek(fd,20,SEEK_END);
    write(fd,"\0",1);
    int len =lseek(fd,0,SEEK_END);//获取文件大小
    p=(char*)mmap(NULL,20,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//创建映射区
    pid=fork();
    if(pid<0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid>0)//父进程
    {
        strcpy(p,"hello world");
    }
    else //子进程
    {
        printf("%s\n",p);
    }
    int ret=munmap(p,len);//释放映射区
    return 0;
}