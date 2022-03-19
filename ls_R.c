#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
void ls_R(char path[]);
void restored_ls(struct dirent*);
void error_handle(const char*);
//字典序
void swap(char** s1,char** s2);
int compare(char* s1,char* s2);
int partition(char** filenames,int start,int end);
void sort(char** filenames,int start,int end);
//文件名字颜色
int get_color(struct stat buf);
void printf_name2(char *name,int color);
//颜色参数
#define WHITE 0
#define BLUE  1
#define GREEN 2
#define RED   3
#define LBLUE 4
#define YELLOW 5
int has_a=0;
int has_l=0;
int has_al=0;
int has_i=0;
int has_ial=0;
int has_il=0;
int has_ai=0;
//存放数组名的数组
char *filenames[4096];		
int file_cnt = 0;	
int main()
{
  ls_R(".");
  return 0;
}
void ls_R(char path[])
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
      restored_ls(direntp);
    }
       sort(filenames,0,file_cnt-1);                          
    int j=0;
    int i=0;
    for(j=0;j<file_cnt;++j)
    {
      struct stat info;
      char temp1[PATH_MAX];
      sprintf(temp1,"%s/%s",path,filenames[j]);
      if(stat(temp1,&info)==-1)
        perror(temp1);
      int color=get_color(info);
      printf_name2(filenames[j],color);
    }
  }
  printf("\n");
  printf("\n");
  file_cnt=0;
  closedir(dir_ptr);
  if((dir_ptr=opendir(path))==NULL)//打开目录
    fprintf(stderr,"lsl:cannot open %s\n",path);
  else 
  {
    while((direntp=readdir(dir_ptr))!=NULL)
    {
      if(strcmp(direntp->d_name,".")==0||strcmp(direntp->d_name,"..")==0)             
        continue;
      struct stat info;
      char temp[PATH_MAX];
      sprintf(temp,"%s/%s",path,direntp->d_name);
      if(stat(temp,&info)==-1)
        perror(temp);
      if(S_ISDIR(info.st_mode))//判断是否为目录，如果是目录就进入递归
      {
        ls_R(temp);
      }
      
    }
  }
}
//交换两字符串
void swap(char** s1,char** s2)
{
	char* tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

//比较两字符串的字典序
//s1靠前，返回负数，s1靠后，返回正数
//s1和s2完全一样，返回0
int compare(char* s1,char* s2)
{
  if(*s1=='.')
    s1++;
  if(*s2=='.')
    s2++;
	while(*s1 && *s2 && *s1 == *s2)
  {
		++s1;
		++s2;
    if(*s1=='.')
      s1++;
    if(*s2=='.')
      s2++;
	}
	return *s1 - *s2;
}

int partition(char** filenames,int start,int end){
	if(!filenames)	return -1;
	char* privot = filenames[start];
	while(start < end){
		while(start < end && compare(privot,filenames[end]) < 0)
			--end;
		swap(&filenames[start],&filenames[end]);
		while(start < end && compare(privot,filenames[start]) >= 0)
			++start;
		swap(&filenames[start],&filenames[end]);
	}
	return start;
}

void sort(char** filenames,int start,int end){
	if(start < end){
		int position = partition(filenames,start,end);
		sort(filenames,start,position - 1);
		sort(filenames,position + 1,end);
	}
}
void restored_ls(struct dirent* cur_item){
	char* result = cur_item->d_name;
	filenames[file_cnt++] = cur_item->d_name;
}

void error_handle(const char* dir_name){
	perror(dir_name);
	exit(1);
}
//对不同的文件类型给不同的颜色
int get_color(struct stat buf)
{
    int color = 0;
    if(S_ISLNK(buf.st_mode))
    {
        color = LBLUE;
    }
    else if(S_ISDIR(buf.st_mode))
    {
        color = BLUE;
    }
    else if(S_ISCHR(buf.st_mode) ||S_ISBLK(buf.st_mode) )
    {
        color = YELLOW;
    }
    else if(buf.st_mode & S_IXUSR)
    {
        color = GREEN;
    }
    return color;
}

void printf_name2(char *name,int color)
{
    if(color == GREEN)
    {
        printf("\033[1m\033[32m%s\033[0m  ",name);
    }
    else if(color == BLUE)
    {
        printf("\033[1m\033[34m%s\033[0m  ",name);
    }
    else if(color == WHITE)
    {
        printf("%s  ",name);
    }
    else if(color == LBLUE)
    {
        printf("\033[1m\033[36m%s\033[0m  ",name);
    }
    else if(color == YELLOW)
    {
        printf("\033[1m\033[33m%s\033[0m  ",name);
    }
}
