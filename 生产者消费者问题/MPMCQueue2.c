#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<semaphore.h>
int i=0;//产品序号
sem_t product_num;
sem_t blank_num;
#define MAX 8//最大容量
void err_thread(int ret,const char*str)//检查错误
{
    if(ret!=0)
    {
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        exit(1);
    }
}
typedef struct MPMCQueue
{
    int ret;//产品内容
    struct MPMCQueue*next;
    int count;
}MPMCQueue;
MPMCQueue* head;//头节点
void*producer(void*arg)
{
    MPMCQueue*temp;
    while(1)
    {
        temp=(MPMCQueue*)malloc(sizeof(MPMCQueue));
        temp->ret=rand()%1000+1;
        sem_wait(&blank_num);
        head->count++;
        printf("产生一个产品%d:%d\n",++i,temp->ret);
        MPMCQueue*t;
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
    MPMCQueue*temp;
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
        sleep(rand()%2);
    }
    return NULL;
}

int main()
{
    sem_init(&product_num,0,0);//刚开始产品数量为0
    sem_init(&blank_num,0,8);//刚开始空格为8
    head=(MPMCQueue*)malloc(sizeof(MPMCQueue));
    head->next=NULL;
    head->count=0;
    pthread_t pid1,pid2,pid3,cid1,cid2,cid3;
    int ret;
    ret=pthread_create(&pid1,NULL,producer,NULL);//生产者1
    err_thread(ret,"create error");
    ret=pthread_create(&pid2,NULL,producer,NULL);//生产者2
    err_thread(ret,"create error");
    ret=pthread_create(&pid3,NULL,producer,NULL);//生产者3
    err_thread(ret,"create error");
    ret=pthread_create(&cid1,NULL,consumer,NULL);//消费者1
      err_thread(ret,"create error");
    ret=pthread_create(&cid2,NULL,consumer,NULL);//消费者2
      err_thread(ret,"create error");
    ret=pthread_create(&cid3,NULL,consumer,NULL);//消费者3
    err_thread(ret,"create error");
    ret=pthread_join(pid1,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(pid2,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(pid3,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(cid1,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(cid2,NULL);
    err_thread(ret,"join error");
    ret=pthread_join(cid3,NULL);
    err_thread(ret,"join error");
    return 0;
}