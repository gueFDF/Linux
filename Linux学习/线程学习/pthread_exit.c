#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
void func()
{
      pthread_exit(NULL);
}
void*tfn(void*argv)
{
    int i=(int)argv;
    sleep(i);
    if(i==2)//第三个线程退出
    {
       // exit(1);//表示退出进程
       //return NULL;//表示返回到函数调用者那里
       func();
      
    }
   printf("I am %d ,pthread: pid = % d,tid = %lu\n",i+1,getpid(),pthread_self());
}
int main()
{
    int i=0;
    int ret;
    pthread_t tid;
    for(i=0;i<5;i++)
    {
        ret=pthread_create(&tid,NULL,tfn,(void*)i);
        if(ret!=0)
        {
            perror("pthread_create");
            exit(1);
        }
    }
    sleep(1);
    printf("i am main\n");
    //return 0;
    pthread_exit(NULL);
}