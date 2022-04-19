#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wait.h>
#include <fcntl.h>
//读取命令使用记录，上一次运行的记录也会存在
extern void add_history PARAMS((const char *)); //解决
extern int read_history PARAMS((const char *));
extern int write_history PARAMS((const char *));
extern HIST_ENTRY **history_list PARAMS((void));//用来执行history命令的声明
#define RL_PROMPT_START_IGNORE '\001'
#define RL_PROMPT_END_IGNORE '\002'
char arr[1000]; //用来保存路径
//打印前面那一段提示符
void printname();
#define MAX 128
//解析参数
void commodAnalsy(char *argv[], int number);
int isdo(char *argv[], int count);
// cd命令
void mycd(char *argv[]);
//输出重定向'>'
void mydup(char *argv[]);
//输出重定向'>>'
void mydup2(char *argv[]);
//输入重定向'<'
void mydup3(char *argv[]);
//管道'|'
void mypipe(char *argv[], int count);
//实现多重管道'|'
void callCommandWithPipe(char *argv[], int count);
void callCommandwithRedi(char *argv[], int left, int right);
int pass=0;//标记是否有&
void ShowHistory();
int main()
{
  read_history(NULL);
  signal(SIGINT, SIG_IGN);
  signal(SIGHUP, SIG_IGN);
  // char commod[MAX];
  while (1)
  {
    char *argv[MAX] = {NULL};
    //打印提示符
    printname();
    // commod[0]=0;
    // fgets(commod,MAX,stdin);
    //fflush(stdout);
    char *commod = readline(" ");
    if (commod == NULL) //屏蔽掉ctrl d 出现死循环的情况
    {
      printf("\n");
      continue;
    }
    add_history(commod);
    write_history(NULL);
    // commod[strlen(commod)-1]=0;
    if (strlen(commod) == 0) //屏蔽掉ctrl d 出现死循环的情况
    {
      // printf("\n");
      continue;
    }
    const char *mark = " "; //分割标识符,用strtok函数以空格为分割标识对字符串commod进行分割,将每个指令取出来.
    int i = 1;
    argv[0] = strtok(commod, mark);
    while (argv[i] = strtok(NULL, mark))
    {
      i++;
    }
    // free(commod);
    commodAnalsy(argv, i);
     free(commod);
  }
}
void printname()
{
  char *name1 = "gty@gty-Lenovo-Legion";
  printf("\033[1m\033[32m%s\033[0m", name1);
  printf(":");
  getcwd(arr,sizeof(arr));
  printf("\033[1m\033[34m%s\033[0m", arr);
  printf("$ ");
  fflush(stdout); //清空缓冲区,默认为行缓冲，提示符不是以\n结尾的
}
void commodAnalsy(char *argv[], int number)
{
  int flag = isdo(argv, number);
  if(pass==1)
  {
    number--;
  }
  if (flag == 1)
  {
    mycd(argv);
  }
  else if (strcmp(argv[0], "history") == 0)
  {
    ShowHistory();
  }
  else if (strcmp(argv[0], "exit") == 0)
  {
    printf("exit\n");
    printf("有停止的任务\n");
    exit(0);
  }
  else if (flag == 2) //输出重定向'>'
  {
    mydup(argv);
  }
  else if (flag == 3) //管道'|'
  {
    callCommandWithPipe(argv, number);
  }
  else if (flag == 4)
  {
    mydup2(argv);
  }
  else if (flag == 5)
  {
    mydup3(argv);
  }
  else if (flag == 10) //需要子进程进行执行的第三方函数
  {
    if (strcmp(argv[0], "ll") == 0)
    {
      strcpy(argv[0], "ls");
      argv[number++] = "-l";
    }
    if (strcmp(argv[0], "ls") == 0)
      argv[number++] = "--color=auto";
    pid_t pid = fork();
    if (pid < 0)
    {
      perror("fork");
      exit(1);
    }
    else if (pid == 0) //子进程
    {
      execvp(argv[0], argv); //执行命令
      perror("commod");
      exit(1);
    }
    else if (pid > 0) //父进程
    {
      if(pass==1)
      {
        pass=0;
        printf("%d\n",pid);
        return;
      }
      waitpid(pid, NULL, 0);
    }
  }
}
char strpwd[MAX]; //用来存放上一次的路劲  实现 cd -
void mycd(char *argv[])
{
  if (argv[1] == NULL)
  {
    getcwd(strpwd, sizeof(strpwd));
    chdir("/home");
  }
  else if (strcmp(argv[1], "-") == 0)
  {
    char strpwd1[MAX];
    getcwd(strpwd1, sizeof(strpwd));
    chdir(strpwd);
    printf("%s\n", strpwd);
    strcpy(strpwd, strpwd1);
  }
  else if (strcmp(argv[1], "~") == 0)
  {
    getcwd(strpwd, sizeof(strpwd));
    chdir("/home/gty");
  }
  else
  {
    getcwd(strpwd, sizeof(strpwd));
    chdir(argv[1]);
  }
}
void mydup(char *argv[])
{
  char *strc[MAX] = {NULL};
  int i = 0;
  while (strcmp(argv[i], ">"))
  {
    strc[i] = argv[i];
    i++;
  }
  i++;
  //出现 echo "adcbe" > test.c  这种情况
 // int fdout = dup(1);                                   //让标准输出获取一个新的文件描述符
  int fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC); //只写模式|表示如果指定文件不存在，则创建这个文件|表示截断，如果文件存在，并且以只写、读写方式打开，则将其长度截断为0。
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork");
    exit(1);
  }
  else if (pid == 0) //子进程
  {
     dup2(fd, 1);
    execvp(strc[0], strc);
  }
  else if (pid > 0)
  {
     if(pass==1)
      {
        pass=0;
        printf("%d\n",pid);
        return;
      }
    waitpid(pid, NULL, 0);
  }
  //dup2(fdout, 1); //
}
void mydup2(char *argv[])
{
  char *strc[MAX] = {NULL};
  int i = 0;
  while (strcmp(argv[i], ">>"))
  {
    strc[i] = argv[i];
    i++;
  }
  i++;
 // int fdout = dup(1);                                    //让标准输出获取一个新的文件描述符
  int fd = open(argv[i], O_WRONLY | O_CREAT | O_APPEND); //只写模式|表示如果指定文件不存在，则创建这个文件|表示追加，如果原来文件里面有内容，则这次写入会写在文件的最末尾。
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork");
    exit(1);
  }
  else if (pid == 0) //子进程
  {
     dup2(fd, 1);
    execvp(strc[0], strc);
  }
  else if (pid > 0)
  {
     if(pass==1)
      {
        pass=0;
        printf("%d\n",pid);
        return;
      }
    waitpid(pid, NULL, 0);
  }
  //dup2(fdout, 1); //
}
void mydup3(char *argv[])
{
  char *strc[MAX] = {NULL};
  int i = 0;
  while (strcmp(argv[i], "<"))
  {
    strc[i] = argv[i];
    i++;
  }
  i++;
 // int fdin = dup(0);                //让标准输出获取一个新的文件描述符
  int fd = open(argv[i], O_RDONLY); //只读模式
  pid_t pid = fork();
  if (pid < 0)
  {
     dup2(fd, 0);
     if(pass==1)
      {
        pass=0;
        printf("%d\n",pid);
        return;
      }
    perror("fork");
    exit(1);
  }
  else if (pid == 0) //子进程
  {
    execvp(strc[0], strc);
  }
  else if (pid > 0)
  {
    waitpid(pid, NULL, 0);
  }
  //dup2(fdin, 0);
}
void callCommandWithPipe(char *argv[], int count)
{
   pid_t pid;
  int ret[10];//存放每个管道的下标
  int number=0;//统计管道个数
  for(int i=0;i<count;i++)
  {
    if(!strcmp(argv[i],"|"))
    {
      ret[number++]=i;
    }
  }
  int cmd_count=number+1;//命令个数
  char* cmd[cmd_count][10];
  for(int i=0;i<cmd_count;i++)//将命令以管道分割存放组数组里
  {
    if(i==0)//第一个命令
    {
      int n=0;
      for(int j=0;j<ret[i];j++)
      {
        cmd[i][n++]=argv[j];
      }
      cmd[i][n]=NULL;
    }
    else if(i==number)//最后一个命令
    {
      int n=0;
      for(int j=ret[i-1]+1;j<count;j++)
      {
        cmd[i][n++]=argv[j];
      }
      cmd[i][n]=NULL;
    }
    else 
    {
       int n=0;
      for(int j=ret[i-1]+1;j<ret[i];j++)
      {
        cmd[i][n++]=argv[j];
      }
      cmd[i][n]=NULL;
    }
  }//经过上述操作，我们已经将指令以管道为分隔符分好,下面我们就可以创建管道了
  int fd[number][2];  //存放管道的描述符
  for(int i=0;i<number;i++)//循环创建多个管道
  {
    pipe(fd[i]);
  }
  int i=0;
  for(i=0;i<cmd_count;i++)//父进程循环创建多个并列子进程
  {
    pid=fork();
    if(pid==0)//子进程直接退出循环，不参与进程的创建
    break;
  }
  if(pid==0)//子进程
  {
    if(number)
    {
      if(i==0)//第一个子进程
      {
        dup2(fd[0][1],1);//绑定写端`  
        close(fd[0][0]);//关闭读端
        //其他进程读写端全部关闭
        for(int j=1;j<number;j++)
        {
          close(fd[j][1]);
          close(fd[j][0]);
        }
      }
      else if(i==number)//最后一个进程
      {
        dup2(fd[i-1][0],0);//打开读端
        close(fd[i-1][1]);//关闭写端
         //其他进程读写端全部关闭
        for(int j=0;j<number-1;j++)
        {
          close(fd[j][1]);
          close(fd[j][0]);
        }
      }
      else //中间进程
      {
        dup2(fd[i-1][0],0);//前一个管道的读端打开
        close(fd[i-1][1]);//前一个写端关闭
        dup2(fd[i][1],1);//后一个管道的写端打开
        close(fd[i][0]);//后一个读端关闭
        //其他的全部关闭
        for(int j=0;j<number;j++)
        {
             if(j!=i&&j!=(i-1))
             {
               close(fd[j][0]);
               close(fd[j][1]);
             }
        }
      }
    }

    execvp(cmd[i][0],cmd[i]);//执行命令
    perror("execvp");
    exit(1);
  }
  //父进程什么都不干，把管道的所有口都关掉
    for(i=0;i<number;i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);//父进程端口全部关掉

    }
     if(pass==1)
      {
        pass=0;
        printf("%d\n",pid);
        return;
      }
  for(int j=0;j<cmd_count;j++)//父进程等待子进程
  wait(NULL);
}

int isdo(char *argv[], int count)
{
  int flag = 10, i;
  if (argv[0] == NULL)
    return 0;
  if (strcmp(argv[0], "cd") == 0)
  {
    flag = 1;
  }
  for (i = 0; i < count; i++)
  {
    if (strcmp(argv[i], ">") == 0)
      flag = 2;
    if (strcmp(argv[i], "|") == 0)
      flag = 3;
    if (strcmp(argv[i], ">>") == 0)
      flag = 4;
    if (strcmp(argv[i], "<") == 0)
      flag = 5;
    if (strcmp(argv[i], "<<") == 0)
      flag = 6;
    if (strcmp(argv[i], "&") == 0)
    {
        pass = 1;
        argv[i]=NULL;
    }
  }
  return flag;
}
void ShowHistory()
{
  int i = 0;
  HIST_ENTRY **his;
  his = history_list();
  while (his[i] != NULL)
    printf("%-3d   %s\n", i, his[i++]->line);
}