#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[1024];

void
xargs(char * cmd,char*argv[])
{
  int pid,status;
  pid=fork();
  if(pid == 0)//childe
  {
    exec(cmd,argv);
    printf("exec failed!\n");
    exit(1);
  }
  else//parent
  {
    wait(&status);
  }

  return;
}
int
main(int argc, char *argv[])
{

  int m,n; //m 记录buf 中读取的位置,n记录读取的字节数
  char *p,*q;//p指针记录标准输入中读取的参数
  m=0;

  char* nargv[MAXARG]; //对buf进行字符分隔，形成新的参数数组

  int j=0;
  while((n=read(0,buf+m,sizeof(buf)-m-1))>0) //从标准输入中读取到 buf 中
  {
    m +=n;
    //buf[m] ='\0';
    p=buf;
    while((q=strchr(p,'\n'))!=0)//q指针指向找到 \n字符的位置
    {
      *q = 0; //为了处理换行，如果不处理，会多出来换行
      nargv[j]=p;//用来构造nargv 数组
      //printf("%s\n",nargv[i]);
      p=q+1; //跳过\n字符
      j++;
    }
    //这里无法处理大于buf 的标准输入,因为构造的指针就会被破环
  }

  char* cmd;//指向 cmd
  char* uargv[MAXARG];//构造新的 char* argv[] 数组,这个数组是下面 exec 的参数 

  if(argc<2)
  {
    cmd="echo";
    uargv[0]="echo"; //程序名称
    int i;
    for(i=1;i<argc;i++) //循环添加将nargv 添加到 uargv
    {
      uargv[i]=nargv[i-1];
    }
    uargv[i-1]=0; //参数末尾赋值为0
    xargs(cmd,uargv);
  }
  else
  {
    cmd=argv[1];
    uargv[0]=argv[1];
    int i;
    for(i=1;i<argc;i++)//先构造原来的参数
    {
      uargv[i]=argv[i+1];//xargs command 之后才是真正的参数
    }
    //跳出这个循环是，i已经加了一次
    
    int m;
    for(m=0;m<j;m++)//循环次数不对
    {
      uargv[i-1]=nargv[m]; //追加参数到末尾，并且每一行执行一次
      uargv[i]=0;
      xargs(cmd,uargv);
    }
  }
  exit(0);
}
