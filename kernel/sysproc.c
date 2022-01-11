#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void) //可以发现大多数的系统调用都是通过 sys_系统调用 包装系统调用?
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 
sys_sysinfo(void)
{
  printf("test\n"); 
  return 0;
}

uint64
sys_trace(void) //实现的函数
{

  //struct proc *p=myproc();

  //printf("%d:syscall %s -> %d\n",p->pid,"open",syscall); //发现使用 getpid 也一样能够做到
  //printf("%d:syscall %s -> %d\n",sys_getpid(),"open",sysindex); //发现这里 getpid 无法直接使用，所以只能使用 sys_getpid
  //代码应该在一个循环中
  //int num =1;
  //for(;syscall!=1;)
  //{
  //  syscall=syscall/2;
  //  num++;
  //}
  //while (syscall !=32) {
  //  syscall=syscall/2;
  //  num++;
  //}
  //printf("%d\n",num);
  //char path[MAXPATH];//,*argv[MAXARG];
  //uint64 uargv;//,uarg;
  //if(argstr(0,path,MAXPATH)<0 || argaddr(1,&uargv)<0){
  //  return -1;
  //}
  int sysindex;
  if(argint(0,&sysindex)<0) //虽然不知道什么用，模仿上面的kill 函数，获取到了传入的值
    return -1;
  int i=0;
  for(;sysindex !=1;)
  {
    sysindex=sysindex/2;
    i++;
  }
  sysindex=i;
  printf("process is: %d\n",i); 
  char path[MAXPATH], *argv[MAXARG];
  //int i;
  uint64 uargv;//, uarg;

  if(argstr(2, path, MAXPATH) < 0 || argaddr(1, &uargv) < 0){
    return -1;
  }
	printf("%s\n",path);
  printf("%d\n",uargv);
  if(argstr(3, path, MAXPATH) < 0 || argaddr(4, &uargv) < 0){
    return -1;
  }
	printf("%s\n",path);
  printf("%d\n",uargv);
  if(argstr(4, path, MAXPATH) < 0 || argaddr(5, &uargv) < 0){
    return -1;
  }
	printf("%s\n",path);
  printf("%d\n",uargv);
  memset(argv, 0, sizeof(argv)); 
  //简单记录下目前思考的内容
  //命令:trace 32 grep hello README
  //追踪 1<<SYS_read(5)
  //如果输出这种格式
  //3: syscall read -> 1023
  //3: syscall read -> 966
  //3: syscall read -> 70
  //3: syscall read -> 0
 
  //命令: trace 2147483647 grep hello README
  //追踪 这个实际上是追踪所有的系统调用
  //2147483647 将低31位全部设置为 1
  //4: syscall trace -> 0
  //4: syscall exec -> 3
  //4: syscall open -> 3
  //4: syscall read -> 1023
  //4: syscall read -> 966
  //4: syscall read -> 70
  //4: syscall read -> 0
  //4: syscall close -> 0
  //
  //
  //命令:trace 2 usertests forkforkfork 
  //usertests starting //这一句是 usertests 打印的
  //test forkforkfork: 407: syscall fork -> 408 //这里开始最终后面fork 中的所有进程
  //408: syscall fork -> 409 
  //409: syscall fork -> 410
  //410: syscall fork -> 411
  //409: syscall fork -> 412
  //410: syscall fork -> 413
  //409: syscall fork -> 414
  //411: syscall fork -> 415

  //格式是这样的
  //进程id：syscall 系统调用名称 -> 返回值
  //该命令对调用它的进程及派生子进程的追踪，不影响进程
  //第一个问题，获取 进程 ID,getpid 就可以解决 
  //第二个问题，需要通过掩码来获取对应的系统调用
  //每个比特位表示一个系统调用，从第2位开始，依次是fork、exit、wait 等系统调用
  //依次判断每个比特位，如果为1，表示需要追踪
  //
  //得到了系统调用之后，需要思考如何追踪
  //获取命令行中的参数，然后组合到系统调用中，获取返回值，输出
  //while((int n=系统调用)>0)
  //{
  //  printf(n)；
  //} 
  //类似这样的逻辑
  //这里的困难在于，参数实际上来源于二进制文件中，无法获取源码
  //如果fork 
  //提示中说到，在proc中添加新的变量中来记住参数
  //从用户空间检索系统调用参数函数在 kernel/syscall.c
  //kernel/sysproc 能够看到一些使用的例子
  //需要修改fork()，参考 kernel/proc.c用来跟踪掩码从父进程到子进程
  //修改kernel/syscall.c中的syscall()函数用来打印跟踪输出
  //需要添加一组系统调用名称对其进行索引.
  //第三个问题，如果获取系统调用的返回值
  //:
  return 0;
}
