#include<stdio.h>
#include<sys/time.h>
int main()
{
    int i=0;
    struct itimerval new_t;
    struct itimerval old_t;
    new_t.it_interval.tv_sec=0;//周期定时  //秒
    new_t.it_interval.tv_usec=0;         //微妙
    new_t.it_value.tv_sec=1;//定时时长    //秒
    new_t.it_value.tv_usec=0;           //微妙
    if(setitimer(ITIMER_REAL,&new_t,&old_t)==-1)
    {
        perror("setitimer error");
        return -1;
    }
    while(1)
    printf("%d\n",++i);
    return 0;
}
