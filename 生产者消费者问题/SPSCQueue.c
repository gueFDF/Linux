#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
int i = 0;                                        //产品序号
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   //互斥量
#define MAX 8
void err_thread(int ret, const char *str) //检查错误
{
    if (ret != 0)
    {
        fprintf(stderr, "%s:%s\n", str, strerror(ret));
        exit(1);
    }
}
typedef struct SPSCQueue
{
    int ret; //产品内容
    struct SPSCQueue *next;
    int count;
} SPSCQueue;
SPSCQueue *head; //头节点
SPSCQueue *SPSCQueueInit(int capacity)
{
    head = (SPSCQueue *)malloc(sizeof(SPSCQueue) * capacity);
    head->next = NULL;
    head->count = 0;
    return head;
}
void SPSCQueuePush(SPSCQueue *queue, int s)
{
    SPSCQueue *temp;
    temp = (SPSCQueue *)malloc(sizeof(SPSCQueue));
    temp->ret = s;
    printf("产生一个产品%d:%d\n", ++i, temp->ret);
    head->count++;
    SPSCQueue *t;
    t = head->next;
    head->next = temp;
    temp->next = t;
}
void *SPSCQueuePop(SPSCQueue *queue)
{
    SPSCQueue *temp;
    temp = head;
    while (temp->next->next != NULL) //找到尾节点的上一个
    {
        temp = temp->next;
    }
    printf("消费一个产品%d:%d\n", i--, temp->next->ret);
    head->count--;
    free(temp->next);
    temp->next = NULL;
}
void SPSCQueueDestory(SPSCQueue *head)
{
    SPSCQueue *temp = head;
    while (temp)
    {
        SPSCQueue *next;
        next = temp->next;
        free(temp);
        temp = next;
    }
}
void *producer(void *arg)
{

    while (1)
    {
        pthread_mutex_lock(&lock); //加锁
        if (head->count == MAX)    //若果已到达最大容量，停止生产等待消费者消费
        {
            pthread_cond_wait(&cond, &lock);
        }
        SPSCQueuePush(head, rand() % 1000 + 1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
    }
    return NULL;
}
void *consumer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock); //加锁
        if (head->next == NULL)
        {
            pthread_cond_wait(&cond, &lock); //如果没有产品，等待生产者生产
        }
        SPSCQueuePop(head);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

int main()
{
    head = SPSCQueueInit(1);
    pthread_t pid, cid;
    int ret;
    ret = pthread_create(&pid, NULL, producer, NULL);
    err_thread(ret, "create error");
    ret = pthread_create(&cid, NULL, consumer, NULL);
    err_thread(ret, "create error");
    ret = pthread_join(pid, NULL);
    err_thread(ret, "join error");
    ret = pthread_join(cid, NULL);
    err_thread(ret, "join error");
    ret = pthread_mutex_destroy(&lock);
    err_thread(ret, "mutex destroy error");
    ret = pthread_cond_destroy(&cond);
    err_thread(ret, "cond destroy error");
    SPSCQueueDestory(head);
    return 0;
}