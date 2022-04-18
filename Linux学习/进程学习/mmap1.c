#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<unistd.h>
int var=100;
int main()
{
    int *p;
    pid_t pid;
    int fd;
    fd=open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd<0)
    {
    lseek(fd,SEEK_END,20);
    write(fd,'a',1);
    p=(int*)mmap(NULL,20,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//创建映射区
    pid=fork();
    if(pid<0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid>0)//父进程
    {
        p="abcdef";
    }
    else //子进程
    {
        printf("%s\n",p);
    }
    return 0;
}