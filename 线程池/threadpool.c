#include "threadpool.h"
//一次扩容数量
int NUMBER=2;
void threadexit(threadpool*pool)
{
    pthread_t tid=pthread_self();
    for(int i=0;i<pool->maxNUM;++i)
    {
        if(pool->threadIDs[i]==tid)
        {
            pool->threadIDs[i]=0;
        }
    }
    pthread_exit(NULL);
}
void *manager(void *arg)
{
    threadpool*pool=(threadpool*)arg;
    while(!pool->shutdown)
    {
        //每隔三秒测一次
        pthread_mutex_lock(&pool->mutexpool);
        int tasksize=pool->tasksize;
        int liveNUM=pool->liveNUM;
        int busyNUM=pool->busyNUM;
        int maxNUM=pool->maxNUM;
        int minNUM=pool->minNUM;
        pthread_mutex_unlock(&pool->mutexpool);
        
       //添加线程(任务个数大于存活线程数&&存活线程数<最大线程数)
       if(tasksize>liveNUM&&liveNUM<maxNUM)
       {
           pthread_mutex_lock(&pool->mutexpool);
           int counter=0;
           for(int i=0;i<maxNUM,counter<NUMBER,pool->liveNUM<maxNUM;i++)
           {
               if(pool->threadIDs[i]==0)
               {
                   pthread_create(&pool->threadIDs[i],NULL,worker,pool);
                   counter++;
                   pool->liveNUM++;
               }
           }
           pthread_mutex_unlock(&pool->mutexpool);
       }
       //销毁线程(工作的线程个数的二倍小于存活线程个数&&存活线程数大于最小线程数)
       if(busyNUM*2>liveNUM&&liveNUM>minNUM)
       {
           pthread_mutex_lock(&pool->mutexpool);
           pool->exitNUM=NUMBER;//一次销毁NUMBER个
           pthread_mutex_unlock(&pool->mutexpool);
           //让工作线程自杀
           for(int i=0;i<NUMBER;++i)
           {
               pthread_cond_signal(&pool->notempty);//唤醒没事干的线程
           }

       }
    }


}
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
            //判断是否要销毁线程
            if(pool->exitNUM!=0)
            {
                pool->exitNUM--;
                pool->liveNUM--;
                pthread_mutex_unlock(&pool->mutexpool);//解锁防止出现死锁
                threadexit(pool);
            }
        }
        //判断当前线程池是否关闭
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexpool);
            threadexit(pool);
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
threadpool *threadpoolinit(int min, int max)
{
    threadpool *pool = (threadpool *)malloc(sizeof(threadpool));
    if (pool == NULL)
    {
        printf("malloc error\n");
        return NULL;
    }
    pool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t) * max);
    if (pool->threadIDs == NULL)
    {
        printf("malloc error\n");
        free(pool);
        return NULL;
    }
    memset(pool->threadIDs,0,max);
    pool->maxNUM = max;
    pool->minNUM = min;
    pool->exitNUM = 0;
    pool->liveNUM = min;
    pool->busyNUM = 0;
    pool->tasksize=0;
    pool->first = NULL;
    pool->end = NULL;
    //锁和条件变量初化
    pthread_mutex_init(&pool->mutexBusy, NULL);
    pthread_mutex_init(&pool->mutexpool, NULL);
    pthread_cond_init(&pool->notempty, NULL);
    //创建线程
    pthread_create(&pool->managerID, NULL, manager, pool);
    for (int i = 0; i < min; i++)
    {
        pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        
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
    pthread_cond_signal(&pool->notempty);
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
    //阻塞回收管理者线程
    pthread_join(pool->managerID,NULL);
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
    free(pool->threadIDs);
    free(pool);
    pool=NULL;
    return 0;
}