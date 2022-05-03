#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<semaphore.h>
int i=0;//产品序号
sem_t product_num,blank_num;
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
        sem_wait(&blank_num);
        t=head->next;
        head->next=temp;
        temp->next=t;
        sem_post(&product_num);
        sleep(rand()%2);
    }
    return NULL;
}
void*consumer(void*arg)
{
    SPSCQueue*temp;
    while(1)
    {
        sem_wait(&product_num);
        temp=head;
        while(temp->next->next!=NULL)//找到尾节点的上一个
        {
            temp=temp->next;
        }
        printf("消费一个产品%d:%d\n",i--,temp->next->ret);
        head->count--;
        free(temp->next);
        temp->next=NULL;
        sem_post(&blank_num);
        sleep(rand()%5);
    }
    return NULL;
}

int main()
{
    sem_init(&product_num,0,0);
    sem_init(&blank_num,0,8);
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
    return 0;
}