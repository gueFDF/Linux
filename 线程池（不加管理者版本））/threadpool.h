#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

//任务队列
typedef struct task
{
    void(*run)(void*arg);
    void*arg;
    struct task *next;
}task;
//线程池
typedef struct threadpool
{
    task*first;
    task*end;
    int threadNUM;//线程数量
    int tasksize;//任务数量
    pthread_mutex_t mutexpool;//锁整个线程池
    pthread_cond_t notempty;//任务队列是不是空
    int shutdown;//是不是要销毁线程池，销毁为1，不销毁为0
}threadpool;
//创建线程池并初始化
threadpool*threadpoolinit(int nmberu);
//销毁线程池
int threadpooldestroy(threadpool*pool);
//给线程池添加任务
void threadpoolAdd(threadpool*pool,void(*run)(void*),void*arg);
void*worker(void*arg);