#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
pthread_mutex_t mutex;//定义互斥锁
void*tfn(void*argv)
{
    srand(time(NULL));
    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
        printf("hello ");
        sleep(rand()%3);
        printf("world\n");
        pthread_mutex_unlock(&mutex);//解锁
        sleep(rand()%3);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    srand(time(NULL));
    int ret=pthread_mutex_init(&mutex,NULL);//初始化互斥锁
    if(ret!=0)
    {
        fprintf(stderr,"mutex_init_error:%s\n",strerror(ret));
        exit(1);
    }
    ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0)
    {
        fprintf(stderr,"create error:%s\n",strerror(ret));
        exit(1);
    }
    while(1)
    {
        pthread_mutex_lock(&mutex);
        printf("HELLO ");
        sleep(rand()%3);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
    pthread_join(tid,NULL);
    pthread_mutex_destroy(&mutex);//销毁互斥锁
    return 0;
}