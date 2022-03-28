#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
void do_ls(char[]);// -al
void do_ls1(char[]);// -l
void do_ls2(char[]);// -a
void do_ls3(char[]);// ls
void do_ls4(char[]);// ls
void do_ls5(char[]);// ls -i
void do_ls6(char[]);// ls -ial
void ls_R(char path[]);//ls -R
//void dostat(char*);
//void show_file_info(char*,struct stat*);
void mode_to_letters(int ,char[]);
//用来适配ls -R
void dostat(char*,char*);
void show_file_info(char*,char*,struct stat*);
char*uid_to_name(uid_t);
char*gid_to_name(gid_t);
void match(int argc,char*argv[]);
void restored_ls(struct dirent*);
void error_handle(const char*);
//字典序
void swap(char** s1,char** s2);
int compare(char* s1,char* s2);
int partition(char** filenames,int start,int end);
void sort(char** filenames,int start,int end);
//文件名字颜色
int get_color(struct stat buf);
void printf_name(char *name,int color);
void printf_name1(char *name,int color);
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
int has_R=0;
int has_aR=0;
int has_Rl=0;
int has_iR=0;
int has_aRl=0;
int has_r=0;
int has_ar=0;
int has_s=0;
//存放数组名的数组
char *filenames[4096];		
int file_cnt = 0;			//目录中文件个数
int main(int argc,char* argv[])
{
   match(argc,argv);
  if(argc==1)
  {
    do_ls2(".");
  }
  else 
  {
    char* name=".";
     int i=0;
     for(i=1;i<argc;i++)
     {
       if(argv[i][0]!='-')
       {
         name=argv[i];
         break;
       }
     }
   // while(--argc)
   // {
      if((has_a==1&&has_l!=1)||has_ar==1)
      {
        do_ls3(name);
      }
      else if(has_r==1)
      {
        do_ls2(name);
      }
      else if(has_a!=1&&has_l==1)
      {
        do_ls(name);
      }
      else if(has_al==1)
      {
        do_ls4(name);
      }
      else if(has_i==1&&has_a!=1&&has_l!=1&&has_al!=1)
      {
        do_ls5(name);
      }
      else if(has_ial==1)
      {
        do_ls4(name);
      }
      else if(has_il==1)
      {
        do_ls4(name);
      }
      else if(has_ai==1)
      {
        do_ls5(name);
      }
      else if(has_aR==1)
      {
        ls_R(name);
      }
      else if(has_R==1)
      {
        ls_R(name);
      }
      else if(has_aRl==1||has_Rl==1)
      {
        ls_R(name);
      }
      else if(has_s==1)
      {
        do_ls2(name);
      }
      else 
      {
         printf("%s:\n",*++argv);
         do_ls2(*argv);
      }
 //  }
  }
  return 0;
  
}
void do_ls(char dirname[])
{
  DIR*dir_ptr;
  struct dirent*direntp;
  if((dir_ptr=opendir(dirname))==NULL)
    fprintf(stderr,"lsl:cannot open %s\n",dirname);
  else 
  {   
    //读取目录并显示信息
          //将文件名存入数组
          while((direntp = readdir(dir_ptr)))
          {
            restored_ls(direntp);
          }
          sort(filenames,0,file_cnt-1);
          int j = 0;
          for(j = 0;j < file_cnt;++j)
          {
               if(filenames[j][0]=='.')
                 continue;
                   char temp1[PATH_MAX];
            sprintf(temp1,"%s/%s" ,dirname,filenames[j]);
            dostat(temp1,filenames[j]);

          }

    closedir(dir_ptr);
  }
}
 void do_ls4(char dirname[])
    {
      DIR*dir_ptr;
      struct dirent*direntp;
      if((dir_ptr=opendir(dirname))==NULL)
        fprintf(stderr,"lsl:cannot open %s\n",dirname);
      else 
      { 
         
                  //读取目录并显示信息
          //将文件名存入数组
          while((direntp = readdir(dir_ptr)))
          {
            restored_ls(direntp);
          }
          sort(filenames,0,file_cnt-1);
          int j = 0;
          for(j = 0;j < file_cnt;++j)
          {
            if(has_il==1)
            {
              if(filenames[j][0]=='.')
                continue;
            }
              char temp1[PATH_MAX];
            sprintf(temp1,"%s/%s",dirname,filenames[j]);
            dostat(temp1,filenames[j]);

         }

        closedir(dir_ptr);
      }
    }

void do_ls1(char dirname[])
    {                                                                                                                                                                                        
      DIR*dir_ptr;
      struct dirent*direntp;
      if((dir_ptr=opendir(dirname))==NULL)
        fprintf(stderr,"lsl:cannot open %s\n",dirname);
      else 
      { 
               //读取目录并显示信息
       //将文件名存入数组
       while((direntp = readdir(dir_ptr)))
       {
         restored_ls(direntp);
       }
       sort(filenames,0,file_cnt-1);
       int j = 0;
       for(j = 0;j < file_cnt;++j)
       {
                char temp1[PATH_MAX];
            sprintf(temp1,"%s/%s",dirname,filenames[j]);
            dostat(temp1,filenames[j]);
       }
        closedir(dir_ptr);
      }
    }
void do_ls2(char dirname[])
{
  int i=0;
  DIR*dir_ptr;
  struct dirent*direntp;
  if((dir_ptr=opendir(dirname))==NULL)     //打开失败
  {
    fprintf(stderr,"lsl:cannot open %s\n",dirname);
  }
  else                                   //打开成功
  {
      
           //读取目录并显示信息
		//将文件名存入数组
		while((direntp = readdir(dir_ptr)))
    {
			restored_ls(direntp);
    }
    sort(filenames,0,file_cnt-1);
    int j = 0;
    if(has_r==1)
    {
      for(j=file_cnt-1;j>=0;--j)
      {
          if(filenames[j][0]=='.')
          continue;
        struct stat info;
        if(stat(filenames[j],&info)==-1)
          perror(filenames[j]);
        int color=get_color(info);
           if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
            printf_name(filenames[j],color);
            i++;
            if(i==4)
            {
              printf("\n");
              i=0;
            }
      }
      printf("\n");
      return;
    }
		for(j = 0;j < file_cnt;++j)
    {
        if(filenames[j][0]=='.')
          continue;
        struct stat info;
        if(stat(filenames[j],&info)==-1)
          perror(filenames[j]);
        int color=get_color(info);
           if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
            printf_name(filenames[j],color);
            i++;
            if(i==4)
            {
              printf("\n");
              i=0;
            }
     }
      }
     printf("\n");
    closedir(dir_ptr);
    
}
void do_ls3(char dirname[])
{
  int i=0;
  int len=0;
  DIR*dir_ptr;
  struct dirent*direntp;
  if((dir_ptr=opendir(dirname))==NULL)     //打开失败
  {
    fprintf(stderr,"lsl:cannot open %s\n",dirname);
  }
  else                                   //打开成功
  {
     while((direntp = readdir(dir_ptr)))
       {
         restored_ls(direntp);
       }
       sort(filenames,0,file_cnt-1);
       int j = 0;
       if(has_ar==1)
       {
          for(j = file_cnt-1;j >=0;--j)
       {
            struct stat info;
           if(stat(filenames[j],&info)==-1)
             perror(filenames[j]);
           int color=get_color(info);
              if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
           printf_name(filenames[j],color);
    
               i++;
               if(i==4)
               {
                 printf("\n");
                 i=0;
               }
        }
				  printf("\n");
				return;
       }
       for(j = 0;j < file_cnt;++j)
       {
            struct stat info;
           if(stat(filenames[j],&info)==-1)
             perror(filenames[j]);
           int color=get_color(info);
              if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
           printf_name(filenames[j],color);
    
               i++;
               if(i==4)
               {
                 printf("\n");
                 i=0;
               }
        }

     
  printf("\n");
    closedir(dir_ptr);
  
  }
}

void do_ls5(char dirname[])
{
  int i=0;
  DIR*dir_ptr;
  struct dirent*direntp;
  if((dir_ptr=opendir(dirname))==NULL)
    fprintf(stderr,"ls1:cannot open %s\n",dirname);
  else 
  {
     while((direntp = readdir(dir_ptr)))
          {
            restored_ls(direntp);
          }
          sort(filenames,0,file_cnt-1);                                                                                                                              
          int j = 0;
          for(j = 0;j < file_cnt;++j)
          {
            if(has_ai!=1)
            {
                if(filenames[j][0]=='.')
                  continue;
            }
                struct stat info;
                if(stat(filenames[j],&info)==-1)
                  perror(filenames[j]);
                  printf("%d  ",info.st_ino); 
                 int color=get_color(info);
                    if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
                  printf_name(filenames[j],color);
                  i++;
                  if(i==4)
                  {
                    printf("\n");
                    i=0;
                  }
           }

  closedir(dir_ptr);
  printf("\n");

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
//用来适配-R
void dostat(char*path,char*filename)
{
  struct stat info;
  if(stat(path,&info)==-1)
    perror(path);
  else 
    show_file_info(path,filename,&info);
}
void show_file_info(char*path,char*filename,struct stat*info_p)
{
  char*uid_to_name(),*ctime(),*git_to_name(),*filemode();
  void mode_to_letters();
  char modestr[11];
      struct stat info;
       if(stat(path,&info)==-1)
       perror(path);
       int color=get_color(info);
          if(has_s==1)
          {                       
             long long size=info_p->st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
  mode_to_letters(info_p->st_mode,modestr);
  if(has_ial==1||has_il==1)
  printf("%ul ",info_p->st_ino);
  printf("%s ",modestr);
  printf("%4d ",(int)info_p->st_nlink);
  printf("%-8s ",uid_to_name(info_p->st_uid));
  printf("%-8s ",gid_to_name(info_p->st_gid));
  printf("%8ld ",(long)info_p->st_size);
  printf("%.12s ",4+ctime(&info_p->st_ctim));
  printf_name1(filename,color);
  printf("\n");
}
char*uid_to_name(gid_t uid)
{
  struct passwd*getpwuid();
  struct passwd*pw_ptr;
  static char numstr[10];
  if((pw_ptr=getpwuid(uid))==NULL)
  {
    sprintf(numstr,"%d",uid);
    return numstr;
  }
  else 
  {
    return pw_ptr->pw_name;
  }
}
char*gid_to_name(gid_t gid)
{
  struct group*getgrgid(),*grp_ptr;
  static char numstr[10];
  if((grp_ptr=getgrgid(gid))==NULL)
  {
    sprintf(numstr,"%d",gid);
    return numstr;
  }
  else 
  {
    return grp_ptr->gr_name;
  }
}
void match(int argc,char*argv[])
{
  int i=1;
  for(i=1;i<argc;i++)
  {
    if(strcmp(argv[i],"-a")==0)
      has_a=1;
    if(strcmp(argv[i],"-l")==0)
      has_l=1;
    if(strcmp(argv[i],"-R")==0)
      has_R=1;
    if(strcmp(argv[i],"-al")==0||strcmp(argv[i],"-la")==0||(has_a==1&&has_l==1))
    {
     has_al=1;
     has_a=0;
     has_l=0;
    }
    if(strcmp(argv[i],"-r")==0)
      has_r=1;
		if(strcmp(argv[i],"-ra")==0||strcmp(argv[i],"-ar")==0||(has_a==1&&has_r==1))
    {
      has_ar=1;
      has_a=0;
      has_r=0;
    }
    if(strcmp(argv[i],"-i")==0)
      has_i=1;
    if(strcmp(argv[i],"-ail")==0||strcmp(argv[i],"-ial")==0||strcmp(argv[i],"-lia")==0||strcmp(argv[i],"-ali")==0||strcmp(argv[i],"ila")==0||strcmp(argv[i],"lai")==0||(has_a==1&&has_l==1&&has_i==1)||(has_il==1&&has_a==1)||(has_ai==1&&has_l==1))
    {
      has_ial=1;
      has_i=0;
      has_a=0;
      has_l=0;
      has_il=0;
      has_al=0;
    }
    if(strcmp(argv[i],"-il")==0||strcmp(argv[i],"-li")==0||(has_i==1&&has_l==1))
    {
      has_il=1;
      has_i=0;
      has_l=0;
    }
     if(strcmp(argv[i],"-Rl")==0||strcmp(argv[i],"-lR")==0||(has_l==1&&has_R==1))
    {
      has_Rl=1;
      has_R=0;
      has_l=0;
    }
     if(strcmp(argv[i],"-aR")==0||strcmp(argv[i],"-Ra")==0||(has_a==1&&has_l==1))
    {
      has_aR=1;
      has_R=0;
      has_a=0;
    }
    if(strcmp(argv[i],"-aRl")==0||strcmp(argv[i],"-alR")==0||strcmp(argv[i],"-laR")==0||strcmp(argv[i],"-lRa")==0||strcmp(argv[i],"-Ral")==0||strcmp(argv[i],"-Rla")==0||(has_a==1&&has_l==1&&has_R==1)||(has_aR==1&&has_l==1)||(has_Rl==1&&has_a==1)||(has_al==1&&has_R==1))
    {
      has_aRl=1;
      has_a=0;
      has_R=0;
      has_l=0;
      has_aR=0;
      has_Rl=0;
      has_al=0;
    }
    if(strcmp(argv[i],"-s")==0)
    {
      has_s=1;
    }
    if(strcmp(argv[i],"-as")==0||strcmp(argv[i],"-sa")==0)
   {
      has_a=1;
      has_s=1;
   }
    if(strcmp(argv[i],"-Rs")==0||strcmp(argv[i],"-sR")==0)
    {
      has_R=1;
      has_s=1;
    }
    if(strcmp(argv[i],"-ls")==0||strcmp(argv[i],"-sl")==0)
    {
      has_s=1;
      has_l=1;
    }
    if(strcmp(argv[i],"-Rs")==0||strcmp(argv[i],"-sR")==0)
    {
      has_s=1;
      has_R=1;    
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

void printf_name(char *name,int color)
{
    if(color == GREEN)
    {
        printf("\033[1m\033[32m%-22s\033[0m",name);
    }
    else if(color == BLUE)
    {
        printf("\033[1m\033[34m%-22s\033[0m",name);
    }
    else if(color == WHITE)
    {
        printf("%-22s",name);
    }
    else if(color == LBLUE)
    {
        printf("\033[1m\033[36m%-22s\033[0m",name);
    }
    else if(color == YELLOW)
    {
        printf("\033[1m\033[33m%-22s\033[0m",name);
    }
}
void printf_name1(char *name,int color)
{
    if(color == GREEN)
    {
        printf("\033[1m\033[32m%s\033[0m",name);
    }
    else if(color == BLUE)
    {
        printf("\033[1m\033[34m%s\033[0m",name);
    }
    else if(color == WHITE)
    {
        printf("%s",name);
    }
    else if(color == LBLUE)
    {
        printf("\033[1m\033[36m%s\033[0m",name);
    }
    else if(color == YELLOW)
    {
        printf("\033[1m\033[33m%s\033[0m",name);
    }
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
    int j=0;;
    for(j=0;j<file_cnt;++j)
    {
      if(has_aRl==1||has_Rl==1)
      {
          char temp1[PATH_MAX];
         sprintf(temp1,"%s/%s",path,filenames[j]);
         dostat(temp1,filenames[j]);
         continue;
      }
      if(filenames[j][0]=='.'&&(has_aR!=1&&has_aRl!=1))
        continue;
      struct stat info;
      char temp1[PATH_MAX];
      sprintf(temp1,"%s/%s",path,filenames[j]);
      if(lstat(temp1,&info)==-1)
        perror(temp1);
      int color=get_color(info);
         if(has_s==1)
          {                       
             long long size=info.st_size/1024;                              
               if(size<=4)           
                  printf("4   ");                
               else         
                  printf("%-4lld",size);                                                                                                                                                    
           }                     
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
      if(has_R==1)
      {
        if(direntp->d_name[0]=='.')
          continue;
      }
      struct stat info;
      char temp[PATH_MAX];
      sprintf(temp,"%s/%s",path,direntp->d_name);
      if(lstat(temp,&info)==-1)
        perror(temp);
      if(S_ISLNK(info.st_mode))
          {
            continue;
          }
      if(S_ISDIR(info.st_mode)&&(!S_ISLNK(info.st_mode)))//判断是否为目录，如果是目录就进入递归
      {
        ls_R(temp);
      }
      
    }
  }
}
