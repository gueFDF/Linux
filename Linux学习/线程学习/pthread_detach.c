#include<stdio.h>
#include<pthread.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void*tfn(void*arg)
{
    printf("thread : pid = %d, tid = %lu\n",getpid(),pthread_self());
    return NULL;
}
int main()
{
    pthread_t tid;
    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0)
    {
        perror("create error");
        exit(1);
    }
    sleep(2);
    ret=pthread_detach(tid);
    if(ret!=0)
    {
        fprintf(stderr,"pthread_deach error:%s\n",strerror(ret));
        exit(1);
    }
    ret=pthread_join(tid,NULL);
    if(ret!=0)
    {
        fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
        exit(1);
    }
    return 0;
}