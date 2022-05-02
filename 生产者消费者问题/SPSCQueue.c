#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
int i=0;//产品序号
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;//互斥量
#define MAX 8
void err_thread(int ret,const char*str)//检查错误
{
    if(ret!=0)
    {
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        exit(1);
    }
}
typedef struct SPSCQueue
{
    int ret;//产品内容
    struct SPSCQueue*next;
    int count;
}SPSCQueue;
SPSCQueue* head;//头节点
void*producer(void*arg)
{
    SPSCQueue*temp;
    while(1)
    {
        temp=(SPSCQueue*)malloc(sizeof(SPSCQueue));
        temp->ret=rand()%1000+1;
        head->count++;
        printf("产生一个产品%d:%d\n",++i,temp->ret);
        SPSCQueue*t;
        pthread_mutex_lock(&lock);//加锁
        if(head->count==MAX)//若果已到达最大容量，停止生产等待消费者消费
        {
            pthread_cond_wait(&cond,&lock);
        }
        t=head->next;
        head->next=temp;
        temp->next=t;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
        sleep(rand()%2);
    }
    return NULL;
}
void*consumer(void*arg)
{
    SPSCQueue*temp;
    while(1)
    {
        pthread_mutex_lock(&lock);//加锁
        if(head->next==NULL)
        {
            pthread_cond_wait(&cond,&lock);//如果没有产品，等待生产者生产
        }
        temp=head;
        while(temp->next->next!=NULL)//找到尾节点的上一个
        {
            temp=temp->next;
        }
        printf("消费一个产品%d:%d\n",i--,temp->next->ret);
        head->count--;
        free(temp->next);
        temp->next=NULL;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
        sleep(rand()%5);
    }
    return NULL;
}

int main()
{
    head=(SPSCQueue*)malloc(sizeof(SPSCQueue));
    head->next=NULL;
    head->count=0;
    pthread_t pid,cid;
    int ret;
    ret=pthread_create(&pid,NULL,producer,NULL);
    err_thread(ret,"create error");
    ret=pthread_create(&cid,NULL,consumer,NULL);
    err_thread(ret,"create error");
    ret=pthread_join(pid,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(cid,NULL);
    err_thread(ret,"join error");
    ret=pthread_mutex_destroy(&lock);
    err_thread(ret,"mutex destroy error");
    ret=pthread_cond_destroy(&cond);
    err_thread(ret,"cond destroy error");
    return 0;
}