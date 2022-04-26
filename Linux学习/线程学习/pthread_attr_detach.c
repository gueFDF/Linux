#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
void*tfn(void*argv)
{
    printf("thread : pid=%d,tid=%lu\n",getpid(),pthread_self());
    return NULL;
}
int main()
{
    pthread_attr_t attr;
    pthread_t tid;
    int ret=pthread_attr_init(&attr);   //初始化
    if(ret!=0)
    {
        fprintf(stderr,"attr_init error:%s\n",strerror(ret));
        exit(1);
    }
    ret=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置线程属性为分离属性
    if(ret!=0)
    {
        fprintf(stderr,"attr_setdeatachstate error:%s\n",strerror(ret));
        exit(1);
    }
    ret=pthread_create(&tid,&attr,tfn,NULL);
    if(ret!=0)
    {
        fprintf(stderr,"attr_create error:%s\n",strerror(ret));
        exit(1);
    }
    ret=pthread_attr_destroy(&attr);//销毁
    if(ret!=0)
    {
        fprintf(stderr,"attr_destroy error:%s\n",strerror(ret));
        exit(1);
    }
    sleep(1);
    ret=pthread_join(tid,NULL);//测试是否成功分离线程
    if(ret!=0)
    {
        fprintf(stderr,"create error:%s\n",strerror(ret));
        exit(1);
    }
       return 0;
}