#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

int main() {
  struct pinfo pi;
  
  if (getpinfo(&pi) < 0) {
    printf("getpinfo failed\n");
    exit(1);
  }
  
  printf("PID\tSTATE\tSIZE\tNAME\n");
  for (int i = 0; i < pi.nproc; i++) {
    printf("%d\t%d\t%d\t%s\n", pi.pid[i], pi.state[i], pi.sz[i], pi.name[i]);
  }
  
  exit(0);
}
