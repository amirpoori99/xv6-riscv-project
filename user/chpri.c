#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc != 3){
    fprintf(2, "Usage: chpri pid priority\n");
    exit(1);
  }
  
  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);
  
  if (priority < 0 || priority > 100) {
    fprintf(2, "Priority must be between 0 and 100\n");
    exit(1);
  }
  
  if(setpriority(pid, priority) < 0) {
    fprintf(2, "chpri failed: invalid pid\n");
    exit(1);
  }
  
  exit(0);
}
