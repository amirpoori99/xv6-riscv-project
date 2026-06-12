#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pinfo.h" 
#include "user/user.h"

const char* state_names[] = {
  [0] "UNUSED  ",
  [1] "USED    ",
  [2] "SLEEPING",
  [3] "RUNNABLE",
  [4] "RUNNING ",
  [5] "ZOMBIE  "
};

int main() {
  struct pinfo info;
  if(getpinfo(&info) < 0) {
    fprintf(2, "Error: ps failed.\n");
    exit(1);
  }

  printf("PID\tSTATE\t\tSIZE\tPRIO\tTICKETS\tNAME\n");
  printf("------------------------------------------------------------\n");
  for(int i = 0; i < info.num_processes; i++) {
    printf("%d\t%s\t%d\t%d\t%d\t%s\n", 
           info.pids[i], 
           state_names[info.states[i]], 
           (int)info.sizes[i], 
           info.priorities[i], 
           info.tickets[i], 
           info.names[i]);
  }
  exit(0);
}

