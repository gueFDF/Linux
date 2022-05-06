#include "threadpool.h"
void *worker(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    while (1)
    {
        pthread_mutex_lock(&pool->mutexpool);
        pool->liveNUM++;
        //当前任务队列是否为空
        while (pool->first == NULL && !pool->shutdown)
        {
            //阻塞工作线程
            pthread_cond_wait(&pool->notempty, &pool->mutexpool);
        }
        //判断当前线程池是否关闭
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexpool);
            pthread_exit(NULL);
        }
        //从任务队列中取出任务
        task *t = pool->first;
        pool->first = t->next;
        pthread_mutex_unlock(&pool->mutexpool);//解锁让其他线程去执行其他任务
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNUM++;
        pool->tasksize--;
        pthread_mutex_unlock(&pool->mutexBusy);
        t->run(t->arg);
        free(t);
        t=NULL;
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNUM--;
        pthread_mutex_unlock(&pool->mutexBusy);


    }
    return NULL;
}
threadpool *threadpoolinit(int number)
{
    threadpool *pool = (threadpool *)malloc(sizeof(threadpool));
    if (pool == NULL)
    {
        printf("malloc error\n");
        return NULL;
    }
    pool->threadNUM = number;
    pool->exitNUM = 0;
    pool->busyNUM = 0;
    pool->tasksize=0;
    pool->first = NULL;
    pool->end = NULL;
    //锁和条件变量初化
    pthread_mutex_init(&pool->mutexBusy, NULL);
    pthread_mutex_init(&pool->mutexpool, NULL);
    pthread_cond_init(&pool->notempty, NULL);
    //创建线程
    for (int i = 0; i < number; i++)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, worker, pool);
    }
    pool->shutdown=0;
    return pool;
}
void threadpoolAdd(threadpool*pool,void(*run)(void*),void*arg)
{
    if(pool->shutdown)//线程池已经被关闭
    {
        return;
    }
    task*t=(task*)malloc(sizeof(task));
    t->arg=arg;
    t->run=run;
    t->next=NULL;
    pthread_mutex_lock(&pool->mutexpool);
    if(pool->first==NULL)//第一个任务
    {
        pool->first=t;
        pool->end=t;
    }
    else
    {
        pool->end->next=t;
        pool->end=t;
    }
    pool->tasksize++;
    pthread_mutex_unlock(&pool->mutexpool);
}
int threadpoolBusyNUM(threadpool*pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNUM= pool->busyNUM;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNUM;
}
int threadpoolliveNUM(threadpool*pool)
{
    pthread_mutex_lock(&pool->mutexpool);
    int liveNUM=pool->liveNUM;
    pthread_mutex_unlock(&pool->mutexpool);
    return liveNUM;
}
int threadpooldestroy(threadpool*pool)
{
    if(pool==NULL)
    {
        return -1;
    }
    pool->shutdown=1;//关闭线程池.管理者线程退出
    //唤醒阻塞的消费者线程顺便销毁
    for(int i=0;i<pool->liveNUM;i++)
    {
        pthread_cond_signal(&pool->notempty);
    }
    //回收锁和条件变量
    pthread_mutex_destroy(&pool->mutexpool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notempty);
    //释放线程池
    free(pool);
    pool=NULL;
    return 0;
}