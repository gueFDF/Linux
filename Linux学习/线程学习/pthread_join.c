#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
typedef struct pdf
{
    int var;
    char arr[100];
}pdf;
void*tnf(void*arg)
{
    struct pdf* p1;
    p1=(pdf*)malloc(sizeof(pdf));
    p1->var=100;
    strcpy(p1->arr,"hello world");
    return (void*)p1;
}
int main()
{
    pthread_t tid;
    pdf* p1;
    int ret =pthread_create(&tid,NULL,tnf,NULL);
    pthread_join(tid,(void**)&p1);
    printf("tar=%d,arr=%s\n",p1->var,p1->arr);
    return 0;
}