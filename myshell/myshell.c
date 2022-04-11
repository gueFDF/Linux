#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<wait.h>
#include<fcntl.h>
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


//打印前面那一段提示符
void printname();
#define MAX 128
//解析参数
void commodAnalsy(char*argv[],int number);
int isdo(char*argv[],int count);
//cd命令
void mycd(char*argv[]);
//输出重定向'>'
void mydup(char*argv[]);
//输出重定向'>>'
void mydup2(char*argv[]);

int main()
{
    char commod[MAX];
    while(1)
    {
        char*argv[MAX]={NULL};
        //打印提示符
        printname();
        commod[0]=0;
        fgets(commod,MAX,stdin);
        fflush(stdout);
        commod[strlen(commod)-1]=0;
        const char* mark=" ";//分割标识符,用strtok函数以空格为分割标识对字符串commod进行分割,将每个指令取出来.
        int i=1;
        argv[0]=strtok(commod,mark);
        while(argv[i]=strtok(NULL,mark))
        {
            i++;
        }
        commodAnalsy(argv,i);
    }
}   
void printname()
{
        char *name1="gty@gty-Lenovo-Legion";
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
}
void commodAnalsy(char*argv[],int number)
{
    int flag=isdo(argv,number);
    if(flag==1)
    {
      mycd(argv);
    }
    else if(flag==2)//输出重定向'>'
    {
      mydup(argv);
    }
    else if(flag==4)
    {
      mydup2(argv);
    }
    else if(flag==10) //需要子进程进行执行的第三方函数
    {
        pid_t pid=fork();
      if(pid<0)
      {
        perror("fork");
        exit(1);
      }
      else if(pid==0)//子进程
      {
        execvp(argv[0],argv);//执行命令
        perror("commod");
        exit(1);
      }
      else if(pid>0)//父进程
      {
        int*status;
        wait(status);
      }
    }
    
}
void mycd(char*argv[])
{
  if(argv[1])
  {
    chdir(argv[1]); 
  }
  else
  {
    chdir("/home/gty");
  }
}
void mydup(char*argv[])
{
    //出现 echo "adcbe" > test.c  这种情况
    int fdout=dup(1);//让标准输出获取一个新的文件描述符
    int fd=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC); //只写模式|表示如果指定文件不存在，则创建这个文件|表示截断，如果文件存在，并且以只写、读写方式打开，则将其长度截断为0。
    dup2(fd,1);
    pid_t pid=fork();
    if(pid<0)
    {
      perror("fork");
      exit(1);
    }
    else if(pid==0)//子进程
    {
      execlp(argv[0],argv[0],argv[1],NULL);
    }
    else if(pid>0)
    {
      int*status;
      wait(status);
    }
      dup2(fdout,1);//
}
void mydup2(char*argv[])
{
  int fdout=dup(1);//让标准输出获取一个新的文件描述符
    int fd=open(argv[3],O_WRONLY|O_CREAT|O_APPEND); //只写模式|表示如果指定文件不存在，则创建这个文件|表示追加，如果原来文件里面有内容，则这次写入会写在文件的最末尾。
    dup2(fd,1);
    pid_t pid=fork();
    if(pid<0)
    {
      perror("fork");
      exit(1);
    }
    else if(pid==0)//子进程
    {
      execlp(argv[0],argv[0],argv[1],NULL);
    }
    else if(pid>0)
    {
      int*status;
      wait(status);
    }
      dup2(fdout,1);//
}
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
int isdo(char*argv[],int count)
{
  int flag = 10,i;
  if(argv[0]==NULL)
  return 0;
  if( strcmp( argv[0],"cd") == 0 ) 
  {
    flag = 1;
  }
  for( i = 0 ; i < count ; i++ ) 
  {
    if( strcmp(argv[i],">") == 0 )
      flag = 2;
    if( strcmp(argv[i],"|") == 0 )
      flag = 3;
    if( strcmp(argv[i],">>") == 0 )
      flag = 4;
       if( strcmp(argv[i],"<") == 0 )
      flag = 5;
       if( strcmp(argv[i],"<<") == 0 )
      flag = 6;
  }
  return flag;
}