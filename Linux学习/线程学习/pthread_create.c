#include<stdio.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
void* tfn(void*arg)
{
    printf("phread:pid = %d,tid = %lu\n",getpid(),pthread_self());
}
int main()
{
    pthread_t tid;
    printf("mian : pid = %d ,tid = %lu\n",getpid(),pthread_self());
    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0)
    {
        perror("pthread");
    }
    sleep(1);
    return 0;
}