#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<errno.h>
int count;
pthread_rwlock_t rwlock;
void*th_write(void*argv)
{
    int t;
    int i=(int)argv;
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        t=count;
        usleep(2000000);
        printf("------write %d:%lu:count=%d ++count=%d\n",i,pthread_self(),t,++count);
        pthread_rwlock_unlock(&rwlock);
        usleep(10000);
    }
    return NULL;
}
void*th_read(void*argv)
{
    int i=(int)argv;
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("------read %d:%lu:%d\n",i,pthread_self(),count);
        pthread_rwlock_unlock(&rwlock);
        usleep(20000);
    }
    return NULL;
}
int main()
{
    int i;
    pthread_t tid[8];
    pthread_rwlock_init(&rwlock,NULL);
    for(i=0;i<3;i++)
    {
        pthread_create(&tid[i],NULL,th_write,(void*)i);
    }   
    for(i=0;i<5;i++)
    {
        pthread_create(&tid[i+3],NULL,th_read,(void*)i);
    }
    for(i=0;i<8;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_rwlock_destroy(&rwlock);
}