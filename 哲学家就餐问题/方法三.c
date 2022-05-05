#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<semaphore.h>
#define N 5
pthread_cond_t cond;
pthread_mutex_t lock;
sem_t chopsticks[N];//五个筷子的状态
int philosophers[N+1] = {0,1,2,3,4};//代表5个哲学家的编号
void*phlios(void*argv)
{
  while(1)
  {
        int i=*(int*)argv;
        int left=(i==0)?N-1:i-1;
        int right=i;
        if((i+1)%2==0)//偶数号哲学家,进入等待
        {
            pthread_cond_wait(&cond,&lock);
        }
        printf("哲学家%d正在思考问题\n",i+1);
        sleep(rand()%3);
        printf("哲学家%d饿了\n",i+1);
        sem_wait(&chopsticks[left]);
        printf("哲学家%d拿起了左边的筷子%d,目前只有一只筷子无法就餐\n",i+1,left+1);
        sem_wait(&chopsticks[right]);
        printf("哲学家%d拿起了右边的筷子%d,开始就餐\n",i+1,right+1);
        if((i+1)%2!=0)//奇数号哲学家
        {
            pthread_cond_signal(&cond);
        }
        sem_post(&chopsticks[left]);
        printf("哲学家%d放下了左边的筷子%d\n",i+1,left+1);
        sem_post(&chopsticks[right]);
        printf("哲学家%d放下了右边的筷子%d\n",i+1,right+1);
  }
}
int main()
{
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
   for(int i=0;i<5;i++)
   {
       sem_init(&chopsticks[i],0,1);
   }

	pthread_t philo[N];
    for(int i=0;i<5;i++)//循环创建五个子线程
    {
        pthread_create(&philo[i],NULL,phlios,&philosophers[i]);
    }
    for(int i=0;i<5;i++)//循环回收五个子线程
    {
        pthread_join(philo[i],NULL);
    }
    for(int i=0;i<5;i++)
    {
        sem_destroy(&chopsticks[i]);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}
