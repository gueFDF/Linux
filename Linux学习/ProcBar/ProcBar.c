#include"ProcBar.h"
void process_bar()
{
   char bar[NUM];
   const char*lable="|/-\\";
   memset(bar,'\0',sizeof(bar));
   int i=0;
   while(i<=100)
   {
     printf("[%-100s][%d%%][%c]\r",bar,i,lable[i%4]);
     fflush(stdout);
     bar[i++]='#';
     usleep(50000);
   }
    printf("\n");

}
