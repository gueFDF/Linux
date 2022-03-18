#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
void ls_R(char  dirname[],char path[]);
void mode_to_letters(int mode,char str[]);
char cataname[1000][256];
int number;
int main(int argc,char*argv[])
{
  ls_R("t1","t1");
  return 0;
}
void ls_R(char dirname[],char path[])
{
  struct stat info;
  if(stat(path,&info)==-1)
      perror(path);
  else 
  {
    char modestr[11];
    mode_to_letters(info.st_mode,modestr);
    if(modestr[0]=='d') //判断是否为目录
    {
      printf("\n%s\n",path);
      DIR*dir_ptr;
      struct dirent*direntp;
      if((dir_ptr=opendir(path))==NULL) //打开目录
        fprintf(stderr,"lsl:cannot open %s\n",dirname);
      else 
      {
        while((direntp=readdir(dir_ptr))!=NULL) //读取该目录的文件
        {
          if(direntp->d_name[0]=='.')
            continue;
          char temp[256];
          sprintf(temp,"%s/%s",path,direntp->d_name);
          ls_R(direntp->d_name,temp);
        }
      }
    }
    else     //若不为目录
    {
      printf("%s ",dirname);
    }
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
