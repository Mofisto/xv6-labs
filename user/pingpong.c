
// pipe1.c: communication over a pipe

#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int fds[2];
  char buf[1];
  int n;
  int pid;

  // create a pipe, with two FDs in fds[0], fds[1].
  pipe(fds);
  pid=fork();

  if(pid ==0)//child
  {
    n=read(fds[0],buf,sizeof(buf));//
    if(n>0)
    {
      printf("%d: received pong\n",getpid());
      sleep(10);
    }
  }
  else //parrent
  {
    write(fds[1]," ",1);
    printf("%d: received ping\n",getpid());
    sleep(10);
  }
  
  exit(0);
}
