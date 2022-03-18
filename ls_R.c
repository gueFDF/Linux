#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
void ls_R(char path[]);
void mode_to_lerrers(int mode,char str[]);
void mode_to_letters(int mode,char str[]);
void addname(char*path);
char* catanames[1000];
int number=0;
int main()
{
  ls_R("t1");
  return 0;
}
void ls_R(char path[])
{
  DIR*dir_ptr;
  struct dirent*direntp;
  printf("%s:\n",path);
  if((dir_ptr=opendir(path))==NULL)//打开目录
    fprintf(stderr,"lsl:cannot open %s\n",path);
  else 
  {
    while((direntp=readdir(dir_ptr))!=NULL)//读取当前目录的文件
    {
      printf("%s  ",direntp->d_name);
      if(direntp->d_name[0]=='.')
        continue;
      struct stat info;
      char temp[256];
      sprintf(temp,"%s/%s",path,direntp->d_name);
      if(stat(temp,&info)==-1)
        perror(temp);
      char modestr[11];
      mode_to_letters(info.st_mode,modestr);   
      if(modestr[0]=='d') //判断是否为目录,若为目录将其存到特定数组里
      {
         addname(temp);
      }
    }
  }
  while(number--)
  {
    ls_R(catanames[number-1]);
  }
}
void mode_to_letters(int mode,char str[])
{
  strcpy(str,"----------");
  if(S_ISDIR(mode))str[0]='d';
  if(S_ISCHR(mode))str[0]='c';
  if(S_ISBLK(mode))str[0]='b';
  if(mode&S_IRUSR)str[1]='r';
  if(mode&S_IWUSR)str[2]='w';
  if(mode&S_IXUSR)str[3]='x';

  if(mode&S_IRGRP)str[4]='r';
  if(mode&S_IWGRP)str[5]='w';
  if(mode&S_IXGRP)str[6]='x';

  if(mode&S_IROTH)str[7]='r';
  if(mode&S_IWOTH)str[8]='w';
  if(mode&S_IXOTH)str[9]='x';
}
void addname(char* path)
{
  catanames[number++]=path;
  return;
}
