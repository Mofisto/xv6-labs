#include "kernel/types.h"
#include "user/user.h"

int prime(int p);
int
main(int argc,char *argv[])
{
    int fds[2];
    int buf;
    int pid;

    pipe(fds);
    pid=fork();
    if(pid ==0)//clild
    {
	while(read(fds[0],&buf,sizeof(buf)) > 0)
	{
	  printf("prime %d\n",buf);
	}

    }
    else //parrent
    {
      for(int i=0;i<35;++i)
      {
          if(prime(i) ==1)
          {
	    buf=i;
            write(fds[1],&buf,4); 
          }
      }
    }
    exit(0);
}

int prime(int p)
{
  int isPrime =1;
  if(p<=1)
  {
    isPrime = 0;
  }
  else{
    for(int i=2;i<=p/2;++i)
    {
      if(p%i==0)
      {
        isPrime =0; 
	break;
      }
    }
  }

  return isPrime;
}
