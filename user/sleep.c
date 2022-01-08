#include "kernel/types.h"
#include "user/user.h"

// pipe2.c: communication between two processes

int
main(int argc,char *argv[])
{
   if(argc<=1){
    exit(0);
   }
   else{
    sleep(atoi(argv[1]));
   exit(0); 
   }
  exit(0);
}
