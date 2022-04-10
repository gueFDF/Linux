#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
//定义颜色
/*#define GREEN "\e[1;32m"
#define BLUE "\e[1;34m"
#define RED "\e[1;31m"
#define WHITE "\e[0m"*/

//mypwd系列函数的实现,用来打印路径
char arr[1000];//用来保存路径
ino_t get_inode(char*);
void printpathto(ino_t);
void inum_to_name(ino_t,char*,int );
//上面三个函数就是pwd完整

#define MAX 128
void commodAnalsy(char*argv[],int number);
int main()
{
    char commod[MAX];
    char *name1="gty@gty-Lenovo-Legion";
    while(1)
    {
        char*argv[MAX]={NULL};
        //打印提示符
        printf("\033[1m\033[32m%s\033[0m",name1);
        printf(":");
        printpathto(get_inode("."));
        printf("\033[1m\033[34m%s\033[0m", arr);
        char arr2[1000]={0};
        sprintf(arr2,"%s%s","/home",arr);
        chdir(arr2);
        memset(arr,0,sizeof(arr));//清空数组
        printf("$ ");
        fflush(stdout);//清空缓冲区,默认为行缓冲，提示符不是以\n结尾的
        commod[0]=0;
        fgets(commod,MAX,stdin);
        fflush(stdout);
        commod[strlen(commod)-1]='0';
        const char* mark=" ";//分割标识符,用strtok函数以空格为分割标识对字符串commod进行分割,将每个指令取出来.
        int i=1;
        argv[0]=strtok(commod,mark);
        while(argv[i]=strtok(NULL,mark))
        {
            i++;
        }
        //commodfAnalsy(argv,i);
    }
}   
/*void commodAnalsy(char*argv[],int number)
{
    if(argv[0]==)
}*/
void printpathto(ino_t this_inode)
{
  ino_t my_inode;
  char its_name[BUFSIZ];
  if (get_inode("..") != this_inode)
  {
    chdir("..");
    inum_to_name(this_inode, its_name, BUFSIZ);
    my_inode = get_inode(".");
    printpathto(my_inode);
    arr[strlen(arr)]='/';
    strcat(arr,its_name);
    //printf("\033[1m\033[34m/%s\033[0m", its_name);
  }
}
void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
  DIR *dir_ptr;
  struct dirent *direntp;
  dir_ptr = opendir(".");
  if (dir_ptr == NULL)
  {
    perror(".");
    exit(1);
  }
  while ((direntp = readdir(dir_ptr)) != NULL)
    if (direntp->d_ino == inode_to_find)
    {
      strncpy(namebuf, direntp->d_name, buflen);
      namebuf[buflen - 1] = '\0';
      closedir(dir_ptr);
      return;
    }
  fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
  exit(1);
}
ino_t get_inode(char *fname)
{
  struct stat info;
  if (stat(fname, &info) == -1)
  {
    fprintf(stderr, "Cannot stat");
    perror(fname);
    exit(1);
  }
  return info.st_ino;
}
