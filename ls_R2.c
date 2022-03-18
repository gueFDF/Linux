#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
void ls_R(char dirname[],char path[]);
int main()
{
  ls_R("/","/");
  return 0;
}
void ls_R(char dirname[],char path[])
{
  printf("%s:\n",path);
  DIR*dir_ptr;
  struct dirent*direntp;
  if((dir_ptr=opendir(path))==NULL)//打开目录
    fprintf(stderr,"lsl:cannot open %s\n",path);
  else 
  {
    while((direntp=readdir(dir_ptr))!=NULL)//读取当前目录文件
    {
      printf("%s  ",direntp->d_name);
    }
  }
  printf("\n");
  closedir(dir_ptr);
  if((dir_ptr=opendir(path))==NULL)//打开目录
    fprintf(stderr,"lsl:cannot open %s\n",path);
  else 
  {
    while((direntp=readdir(dir_ptr))!=NULL)
    {
      if(direntp->d_name[0]=='.')
        continue;
      struct stat info;
      char temp[PATH_MAX];
      sprintf(temp,"%s/%s",path,direntp->d_name);
      if(stat(temp,&info)==-1)
        perror(temp);
      if(S_ISDIR(info.st_mode))//判断是否为目录，如果是目录就进入递归
      {
        ls_R(direntp->d_name ,temp);
      }
      
    }
  }
}
