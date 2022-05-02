#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
void*tnf(void*arg)
{   while(1)
  {
    printf(" thread: pid = %d,tid = %lu\n",getpid(),pthread_self());
    sleep(1); 
  }
}
int main()
{
    pthread_t tid;
    int ret =pthread_create(&tid,NULL,tnf,NULL);
    if(ret<0)
    {
        perror("create");
        exit(1);
    }
    sleep(3);
    ret=pthread_cancel(tid);
    while(1);
    return 0;
}